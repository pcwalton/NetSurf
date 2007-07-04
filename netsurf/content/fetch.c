/*
 * This file is part of NetSurf, http://netsurf-browser.org/
 * Licensed under the GNU General Public License,
 *		  http://www.opensource.org/licenses/gpl-license
 * Copyright 2006,2007 Daniel Silverstone <dsilvers@digital-scurf.org>
 * Copyright 2007 James Bursa <bursa@users.sourceforge.net>
 * Copyright 2003 Phil Mellor <monkeyson@users.sourceforge.net>
 */

/** \file
 * Fetching of data from a URL (implementation).
 *
 * Active fetches are held in the circular linked list ::fetch_ring. There may
 * be at most ::option_max_fetchers_per_host active requests per Host: header.
 * There may be at most ::option_max_fetchers active requests overall. Inactive
 * fetchers are stored in the ::queue_ring waiting for use.
 */

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <sys/select.h>
#include <sys/stat.h>
#ifdef riscos
#include <unixlib/local.h>
#endif
#include "utils/config.h"
#ifdef WITH_SSL
#include <openssl/ssl.h>
#endif
#include "content/fetch.h"
#include "content/fetchers/fetch_curl.h"
#include "content/urldb.h"
#include "desktop/netsurf.h"
#include "desktop/options.h"
#include "render/form.h"
#undef NDEBUG
#include "utils/log.h"
#include "utils/messages.h"
#include "utils/url.h"
#include "utils/utils.h"
#include "utils/ring.h"

bool fetch_active;	/**< Fetches in progress, please call fetch_poll(). */

/** Information about a fetcher for a given scheme. */
typedef struct scheme_fetcher_s {
	char *scheme_name;		/**< The scheme. */
	fetcher_setup_fetch setup_fetch;	/**< Set up a fetch. */
	fetcher_start_fetch start_fetch;	/**< Start a fetch. */
	fetcher_abort_fetch abort_fetch;	/**< Abort a fetch. */
	fetcher_free_fetch free_fetch;		/**< Free a fetch. */
	fetcher_poll_fetcher poll_fetcher;	/**< Poll this fetcher. */
	fetcher_finalise finaliser;		/**< Clean up this fetcher. */
	int refcount;				/**< When zero, clean up the fetcher. */
	struct scheme_fetcher_s *next_fetcher;	/**< Next fetcher in the list. */
	struct scheme_fetcher_s *prev_fetcher;  /**< Prev fetcher in the list. */
} scheme_fetcher;

static scheme_fetcher *fetchers = NULL;

/** Information for a single fetch. */
struct fetch {
	fetch_callback callback;/**< Callback function. */
	bool abort;		/**< Abort requested. */
	bool stopped;		/**< Download stopped on purpose. */
	char *url;		/**< URL. */
	char *referer;		/**< Referer URL. */
	bool send_referer;	/**< Valid to send the referer */
	void *p;		/**< Private data for callback. */
	char *host;		/**< Host part of URL. */
	long http_code;		/**< HTTP response code, or 0. */
	scheme_fetcher *ops;	/**< Fetcher operations for this fetch,
				     NULL if not set. */
	void *fetcher_handle;	/**< The handle for the fetcher. */
	bool fetch_is_active;	/**< This fetch is active. */
	struct fetch *r_prev;	/**< Previous active fetch in ::fetch_ring. */
	struct fetch *r_next;	/**< Next active fetch in ::fetch_ring. */
};

static struct fetch *fetch_ring = 0;	/**< Ring of active fetches. */
static struct fetch *queue_ring = 0;	/**< Ring of queued fetches */

#define fetch_ref_fetcher(F) F->refcount++
static void fetch_unref_fetcher(scheme_fetcher *fetcher);
static void fetch_dispatch_jobs(void);
static bool fetch_choose_and_dispatch(void);
static bool fetch_dispatch_job(struct fetch *fetch);


/**
 * Initialise the fetcher.
 *
 * Must be called once before any other function.
 */

void fetch_init(void)
{
	fetch_curl_register();
	fetch_active = false;
}


/**
 * Clean up for quit.
 *
 * Must be called before exiting.
 */

void fetch_quit(void)
{
	while (fetchers != NULL) {
		if (fetchers->refcount != 1) {
			LOG(("Fetcher for scheme %s still active?!",
					fetchers->scheme_name));
			/* We shouldn't do this, but... */
			fetchers->refcount = 1;
		}
		fetch_unref_fetcher(fetchers);
	}
}


bool fetch_add_fetcher(const char *scheme,
		  fetcher_initialise initialiser,
		  fetcher_setup_fetch setup_fetch,
		  fetcher_start_fetch start_fetch,
		  fetcher_abort_fetch abort_fetch,
		  fetcher_free_fetch free_fetch,
		  fetcher_poll_fetcher poll_fetcher,
		  fetcher_finalise finaliser)
{
	scheme_fetcher *new_fetcher;
	if (!initialiser(scheme))
		return false;
	new_fetcher = malloc(sizeof(scheme_fetcher));
	if (new_fetcher == NULL) {
		finaliser(scheme);
		return false;
	}
	new_fetcher->scheme_name = strdup(scheme);
	if (new_fetcher->scheme_name == NULL) {
		free(new_fetcher);
		finaliser(scheme);
		return false;
	}
	new_fetcher->refcount = 0;
	new_fetcher->setup_fetch = setup_fetch;
	new_fetcher->start_fetch = start_fetch;
	new_fetcher->abort_fetch = abort_fetch;
	new_fetcher->free_fetch = free_fetch;
	new_fetcher->poll_fetcher = poll_fetcher;
	new_fetcher->finaliser = finaliser;
	new_fetcher->next_fetcher = fetchers;
	fetchers = new_fetcher;
	fetch_ref_fetcher(new_fetcher);
	return true;
}


void fetch_unref_fetcher(scheme_fetcher *fetcher)
{
	if (--fetcher->refcount == 0) {
		fetcher->finaliser(fetcher->scheme_name);
		free(fetcher->scheme_name);
		if (fetcher == fetchers) {
			fetchers = fetcher->next_fetcher;
			if (fetchers)
				fetchers->prev_fetcher = NULL;
		} else {
			fetcher->prev_fetcher->next_fetcher =
					fetcher->next_fetcher;
			if (fetcher->next_fetcher != NULL)
				fetcher->next_fetcher->prev_fetcher =
						fetcher->prev_fetcher;
		}
		free(fetcher);
	}
}


/**
 * Start fetching data for the given URL.
 *
 * The function returns immediately. The fetch may be queued for later
 * processing.
 *
 * A pointer to an opaque struct fetch is returned, which can be passed to
 * fetch_abort() to abort the fetch at any time. Returns 0 if memory is
 * exhausted (or some other fatal error occurred).
 *
 * The caller must supply a callback function which is called when anything
 * interesting happens. The callback function is first called with msg
 * FETCH_TYPE, with the Content-Type header in data, then one or more times
 * with FETCH_DATA with some data for the url, and finally with
 * FETCH_FINISHED. Alternatively, FETCH_ERROR indicates an error occurred:
 * data contains an error message. FETCH_REDIRECT may replace the FETCH_TYPE,
 * FETCH_DATA, FETCH_FINISHED sequence if the server sends a replacement URL.
 *
 */

struct fetch * fetch_start(const char *url, const char *referer,
			   fetch_callback callback,
			   void *p, bool only_2xx, const char *post_urlenc,
			   struct form_successful_control *post_multipart,
			   bool verifiable, const char *parent_url,
			   char *headers[])
{
	char *host;
	struct fetch *fetch;
	url_func_result res;
	char *ref1 = 0, *ref2 = 0;
	scheme_fetcher *fetcher = fetchers;

	fetch = malloc(sizeof (*fetch));
	if (!fetch)
		return 0;

	res = url_host(url, &host);
	if (res != URL_FUNC_OK) {
		/* we only fail memory exhaustion */
		if (res == URL_FUNC_NOMEM)
			goto failed;

		host = strdup("");
		if (!host)
			goto failed;
	}

	res = url_scheme(url, &ref1);
	if (res != URL_FUNC_OK) {
		/* we only fail memory exhaustion */
		if (res == URL_FUNC_NOMEM)
			goto failed;
		ref1 = NULL;
	}

	if (referer) {
		res = url_scheme(referer, &ref2);
		if (res != URL_FUNC_OK) {
			/* we only fail memory exhaustion */
			if (res == URL_FUNC_NOMEM)
				goto failed;
			ref2 = NULL;
		}
	}

	LOG(("fetch %p, url '%s'", fetch, url));

	/* construct a new fetch structure */
	fetch->callback = callback;
	fetch->abort = false;
	fetch->stopped = false;
	fetch->url = strdup(url);
	fetch->p = p;
	fetch->host = host;
	fetch->http_code = 0;
	fetch->r_prev = 0;
	fetch->r_next = 0;
	fetch->referer = 0;
	fetch->ops = 0;
	fetch->fetch_is_active = false;

	if (referer != NULL) {
		fetch->referer = strdup(referer);
		if (fetch->referer == NULL)
			goto failed;
		if (option_send_referer && ref1 && ref2 &&
				strcasecmp(ref1, ref2) == 0)
			fetch->send_referer = true;
	}

	if (!fetch->url)
		goto failed;

	/* Pick the scheme ops */
	while (fetcher) {
		if (strcmp(fetcher->scheme_name, ref1) == 0) {
			fetch->ops = fetcher;
			break;
		}
		fetcher = fetcher->next_fetcher;
	}

	if (fetch->ops == NULL)
		goto failed;

	/* Got a scheme fetcher, try and set up the fetch */
	fetch->fetcher_handle =
		fetch->ops->setup_fetch(fetch, url, only_2xx, post_urlenc,
					post_multipart, verifiable, parent_url,
					(const char **)headers);

	if (fetch->fetcher_handle == NULL)
		goto failed;

	/* Rah, got it, so ref the fetcher. */
	fetch_ref_fetcher(fetch->ops);

	/* these aren't needed past here */
	if (ref1) {
		free(ref1);
		ref1 = 0;
	}

	if (ref2) {
		free(ref2);
		ref2 = 0;
	}

	/* Dump us in the queue and ask the queue to run. */
	RING_INSERT(queue_ring, fetch);
	fetch_dispatch_jobs();
	return fetch;

failed:
	free(host);
	if (ref1)
		free(ref1);
	free(fetch->url);
	if (fetch->referer)
		free(fetch->referer);
	free(fetch);
	return 0;
}


/**
 * Dispatch as many jobs as we have room to dispatch.
 */
void fetch_dispatch_jobs(void)
{
	int all_active, all_queued;

	if (!queue_ring)
		return; /* Nothing to do, the queue is empty */
	RING_GETSIZE(struct fetch, queue_ring, all_queued);
	RING_GETSIZE(struct fetch, fetch_ring, all_active);

	LOG(("queue_ring %i, fetch_ring %i", all_queued, all_active));

	struct fetch *q = queue_ring;
	if (q) {
		do {
			LOG(("queue_ring: %s", q->url));
			q = q->r_next;
		} while (q != queue_ring);
	}
	struct fetch *f = fetch_ring;
	if (f) {
		do {
			LOG(("fetch_ring: %s", f->url));
			f = f->r_next;
		} while (f != fetch_ring);
	}

	while ( all_queued && all_active < option_max_fetchers ) {
		/*LOG(("%d queued, %d fetching", all_queued, all_active));*/
		if (fetch_choose_and_dispatch()) {
			all_queued--;
			all_active++;
		} else {
			/* Either a dispatch failed or we ran out. Just stop */
			break;
		}
	}
	fetch_active = (all_active > 0);
	LOG(("Fetch ring is now %d elements.", all_active));
	LOG(("Queue ring is now %d elements.", all_queued));
}


/**
 * Choose and dispatch a single job. Return false if we failed to dispatch
 * anything.
 *
 * We don't check the overall dispatch size here because we're not called unless
 * there is room in the fetch queue for us.
 */
bool fetch_choose_and_dispatch(void)
{
	struct fetch *queueitem;
	queueitem = queue_ring;
	do {
		/* We can dispatch the selected item if there is room in the
		 * fetch ring
		 */
		int countbyhost;
		RING_COUNTBYHOST(struct fetch, fetch_ring, countbyhost,
				queueitem->host);
		if (countbyhost < option_max_fetchers_per_host) {
			/* We can dispatch this item in theory */
			return fetch_dispatch_job(queueitem);
		}
		queueitem = queueitem->r_next;
	} while (queueitem != queue_ring);
	return false;
}


/**
 * Dispatch a single job
 */
bool fetch_dispatch_job(struct fetch *fetch)
{
	RING_REMOVE(queue_ring, fetch);
	LOG(("Attempting to start fetch %p, fetcher %p, url %s", fetch,
	     fetch->fetcher_handle, fetch->url));
	if (!fetch->ops->start_fetch(fetch->fetcher_handle)) {
		RING_INSERT(queue_ring, fetch); /* Put it back on the end of the queue */
		return false;
	} else {
		RING_INSERT(fetch_ring, fetch);
		fetch->fetch_is_active = true;
		return true;
	}
}


/**
 * Abort a fetch.
 */

void fetch_abort(struct fetch *f)
{
	assert(f);
	LOG(("fetch %p, fetcher %p, url '%s'", f, f->fetcher_handle, f->url));
	f->ops->abort_fetch(f->fetcher_handle);
}


/**
 * Free a fetch structure and associated resources.
 */

void fetch_free(struct fetch *f)
{
	LOG(("Freeing fetch %p, fetcher %p", f, f->fetcher_handle));
	f->ops->free_fetch(f->fetcher_handle);
	fetch_unref_fetcher(f->ops);
	free(f->url);
	free(f->host);
	if (f->referer)
		free(f->referer);
	free(f);
}


/**
 * Do some work on current fetches.
 *
 * Must be called regularly to make progress on fetches.
 */

void fetch_poll(void)
{
	scheme_fetcher *fetcher = fetchers;

	fetch_dispatch_jobs();

	if (!fetch_active)
		return; /* No point polling, there's no fetch active. */
	while (fetcher != NULL) {
		/* LOG(("Polling fetcher for %s", fetcher->scheme_name)); */
		fetcher->poll_fetcher(fetcher->scheme_name);
		fetcher = fetcher->next_fetcher;
	}
}


/**
 * Check if a URL's scheme can be fetched.
 *
 * \param  url  URL to check
 * \return  true if the scheme is supported
 */

bool fetch_can_fetch(const char *url)
{
	const char *semi;
	size_t len;
	scheme_fetcher *fetcher = fetchers;

	if ((semi = strchr(url, ':')) == NULL)
		return false;
	len = semi - url;

	while (fetcher != NULL) {
		if (strlen(fetcher->scheme_name) == len &&
		    strncmp(fetcher->scheme_name, url, len) == 0)
			return true;
		fetcher = fetcher->next_fetcher;
	}

	return false;
}


/**
 * Change the callback function for a fetch.
 */

void fetch_change_callback(struct fetch *fetch,
			   fetch_callback callback,
			   void *p)
{
	assert(fetch);
	fetch->callback = callback;
	fetch->p = p;
}


/**
 * Get the HTTP response code.
 */

long fetch_http_code(struct fetch *fetch)
{
	return fetch->http_code;
}


/**
 * Get the referer
 *
 * \param fetch  fetch to retrieve referer from
 * \return Pointer to referer string, or NULL if none.
 */
const char *fetch_get_referer(struct fetch *fetch)
{
	return fetch->referer;
}

void
fetch_send_callback(fetch_msg msg, struct fetch *fetch, const void *data,
		unsigned long size)
{
	LOG(("Fetcher sending callback. Fetch %p, fetcher %p data %p size %lu",
	     fetch, fetch->fetcher_handle, data, size));
	fetch->callback(msg, fetch->p, data, size);
}


void fetch_remove_from_queues(struct fetch *fetch)
{
	int all_active, all_queued;

	/* Go ahead and free the fetch properly now */
	LOG(("Fetch %p, fetcher %p can be freed", fetch, fetch->fetcher_handle));

	if (fetch->fetch_is_active) {
		RING_REMOVE(fetch_ring, fetch);
	} else {
		RING_REMOVE(queue_ring, fetch);
	}

	RING_GETSIZE(struct fetch, fetch_ring, all_active);
	RING_GETSIZE(struct fetch, queue_ring, all_queued);

	fetch_active = (all_active > 0);

	LOG(("Fetch ring is now %d elements.", all_active));
	LOG(("Queue ring is now %d elements.", all_queued));
}


void
fetch_set_http_code(struct fetch *fetch, long http_code)
{
	LOG(("Setting HTTP code to %ld", http_code));
	fetch->http_code = http_code;
}

const char *
fetch_get_referer_to_send(struct fetch *fetch)
{
	if (fetch->send_referer)
		return fetch->referer;
	return NULL;
}
