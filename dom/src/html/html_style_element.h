/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 Bo Yang <struggleyb.nku.com>
 */

#ifndef dom_internal_html_style_element_h_
#define dom_internal_html_style_element_h_

#include <dom/html/html_style_element.h>

#include "html/html_element.h"

struct dom_html_style_element {
	struct dom_html_element base;
			/**< The base class */
};

/* Create a dom_html_style_element object */
dom_exception _dom_html_style_element_create(struct dom_document *doc,
		struct dom_html_style_element **ele);

/* Initialise a dom_html_style_element object */
dom_exception _dom_html_style_element_initialise(struct dom_document *doc,
		struct dom_html_style_element *ele);

/* Finalise a dom_html_style_element object */
void _dom_html_style_element_finalise(struct dom_document *doc,
		struct dom_html_style_element *ele);

/* Destroy a dom_html_style_element object */
void _dom_html_style_element_destroy(struct dom_document *doc,
		struct dom_html_style_element *ele);

/* The protected virtual functions */
dom_exception _dom_html_style_element_parse_attribute(dom_element *ele,
		struct dom_string *name, struct dom_string *value,
		struct dom_string **parsed);
void _dom_virtual_html_style_element_destroy(dom_node_internal *node);
dom_exception _dom_html_style_element_alloc(struct dom_document *doc,
		struct dom_node_internal *n, struct dom_node_internal **ret);
dom_exception _dom_html_style_element_copy(struct dom_node_internal *new,
		struct dom_node_internal *old);

#define DOM_HTML_STYLE_ELEMENT_PROTECT_VTABLE \
	_dom_html_style_element_parse_attribute

#define DOM_NODE_PROTECT_VTABLE_HTML_STYLE_ELEMENT \
	_dom_virtual_html_style_element_destroy, \
	_dom_html_style_element_alloc, \
	_dom_html_style_element_copy

#endif
