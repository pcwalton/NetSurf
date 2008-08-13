/*
 * This file is part of Hubbub.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <assert.h>
#include <string.h>

#include "treebuilder/modes.h"
#include "treebuilder/internal.h"
#include "treebuilder/treebuilder.h"
#include "utils/utils.h"


/**
 * Handle tokens in "script collect characters" insertion mode
 *
 * \param treebuilder  The treebuilder instance
 * \param token        The token to process
 * \return True to reprocess the token, false otherwise
 */
hubbub_error handle_script_collect_characters(hubbub_treebuilder *treebuilder,
		const hubbub_token *token)
{
	hubbub_error err = HUBBUB_OK;
	bool done = false;

	switch (token->type) {
	case HUBBUB_TOKEN_CHARACTER:
	{
		int success;
		void *text, *appended;

		success = treebuilder->tree_handler->create_text(
				treebuilder->tree_handler->ctx,
				&token->data.character,
				&text);
		if (success != 0) {
			/** \todo errors */
		}

		/** \todo fragment case -- skip this lot entirely */

		success = treebuilder->tree_handler->append_child(
				treebuilder->tree_handler->ctx,
				treebuilder->context.collect.node,
				text, &appended);
		if (success != 0) {
			/** \todo errors */
			treebuilder->tree_handler->unref_node(
					treebuilder->tree_handler->ctx,
					text);
		}

		treebuilder->tree_handler->unref_node(
				treebuilder->tree_handler->ctx, appended);
		treebuilder->tree_handler->unref_node(
				treebuilder->tree_handler->ctx, text);
	}
		break;
	case HUBBUB_TOKEN_END_TAG:
	{
		element_type type = element_type_from_name(treebuilder,
				&token->data.tag.name);

		if (type != treebuilder->context.collect.type) {
			/** \todo parse error */
			/** \todo Mark script as "already executed" */
		}

		done = true;
	}
		break;
	case HUBBUB_TOKEN_EOF:
	case HUBBUB_TOKEN_COMMENT:
	case HUBBUB_TOKEN_DOCTYPE:
	case HUBBUB_TOKEN_START_TAG:
		/** \todo parse error */
		/** \todo Mark script as "already executed" */
		done = true;
		err = HUBBUB_REPROCESS;
		break;
	}

	if (done) {
		int success;
		void *appended;

		/** \todo insertion point manipulation */

		/* Scripts in "after head" should be inserted into <head> */
		/* See 8.2.5.9 The "after head" insertion mode */
		if (treebuilder->context.collect.mode == AFTER_HEAD) {
			if (!element_stack_push(treebuilder,
					HUBBUB_NS_HTML,
					HEAD,
					treebuilder->context.head_element)) {
			        /** \todo errors */
		        }
		}

		/* Append script node to current node */
		success = treebuilder->tree_handler->append_child(
				treebuilder->tree_handler->ctx,
				treebuilder->context.element_stack[
				treebuilder->context.current_node].node,
				treebuilder->context.collect.node, &appended);
		if (success != 0) {
			/** \todo errors */
		}

		if (treebuilder->context.collect.mode == AFTER_HEAD) {
			hubbub_ns ns;
			element_type otype;
			void *node;

 			if (!element_stack_pop(treebuilder, &ns, &otype,
					&node)) {
				/** \todo errors */
			}
		}

		/** \todo restore insertion point */

		treebuilder->tree_handler->unref_node(
				treebuilder->tree_handler->ctx,
				appended);

		treebuilder->tree_handler->unref_node(
				treebuilder->tree_handler->ctx,
				treebuilder->context.collect.node);
		treebuilder->context.collect.node = NULL;

		/** \todo process any pending script */

		/* Return to previous insertion mode */
		treebuilder->context.mode =
				treebuilder->context.collect.mode;
	}

	return err;
}

