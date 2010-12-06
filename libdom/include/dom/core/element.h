/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef dom_core_element_h_
#define dom_core_element_h_

#include <stdbool.h>

#include <dom/core/exceptions.h>
#include <dom/core/node.h>

struct dom_attr;
struct dom_nodelist;
struct dom_string;
struct dom_type_info;

typedef struct dom_element dom_element;

/* The DOMElement vtable */
typedef struct dom_element_vtable {
	struct dom_node_vtable base;

	dom_exception (*dom_element_get_tag_name)(struct dom_element *element,
			struct dom_string **name);
	dom_exception (*dom_element_get_attribute)(struct dom_element *element,
			struct dom_string *name, struct dom_string **value);
	dom_exception (*dom_element_set_attribute)(struct dom_element *element,
			struct dom_string *name, struct dom_string *value);
	dom_exception (*dom_element_remove_attribute)(
			struct dom_element *element, struct dom_string *name);
	dom_exception (*dom_element_get_attribute_node)(
			struct dom_element *element, struct dom_string *name, 
			struct dom_attr **result);
	dom_exception (*dom_element_set_attribute_node)(
			struct dom_element *element, struct dom_attr *attr, 
			struct dom_attr **result);
	dom_exception (*dom_element_remove_attribute_node)(
			struct dom_element *element, struct dom_attr *attr, 
			struct dom_attr **result);
	dom_exception (*dom_element_get_elements_by_tag_name)(
			struct dom_element *element, struct dom_string *name,
			struct dom_nodelist **result);
	dom_exception (*dom_element_get_attribute_ns)(
			struct dom_element *element, 
			struct dom_string *namespace,
			struct dom_string *localname,
			struct dom_string **value);
	dom_exception (*dom_element_set_attribute_ns)(
			struct dom_element *element,
			struct dom_string *namespace, struct dom_string *qname,
			struct dom_string *value);
	dom_exception (*dom_element_remove_attribute_ns)(
			struct dom_element *element,
			struct dom_string *namespace, 
			struct dom_string *localname);
	dom_exception (*dom_element_get_attribute_node_ns)(
			struct dom_element *element,
			struct dom_string *namespace, 
			struct dom_string *localname, struct dom_attr **result);
	dom_exception (*dom_element_set_attribute_node_ns)(
			struct dom_element *element, struct dom_attr *attr, 
			struct dom_attr **result);
	dom_exception (*dom_element_get_elements_by_tag_name_ns)(
			struct dom_element *element, 
			struct dom_string *namespace, 
			struct dom_string *localname, 
			struct dom_nodelist **result);
	dom_exception (*dom_element_has_attribute)(struct dom_element *element,
			struct dom_string *name, bool *result);
	dom_exception (*dom_element_has_attribute_ns)(
			struct dom_element *element,
			struct dom_string *namespace, 
			struct dom_string *localname, bool *result);
	dom_exception (*dom_element_get_schema_type_info)(
			struct dom_element *element, 
			struct dom_type_info **result);
	dom_exception (*dom_element_set_id_attribute)(
			struct dom_element *element, struct dom_string *name, 
			bool is_id);
	dom_exception (*dom_element_set_id_attribute_ns)(
			struct dom_element *element, 
			struct dom_string *namespace, 
			struct dom_string *localname, bool is_id);
	dom_exception (*dom_element_set_id_attribute_node)(
			struct dom_element *element,
			struct dom_attr *id_attr, bool is_id);
} dom_element_vtable;

static inline dom_exception dom_element_get_tag_name(
		struct dom_element *element, struct dom_string **name)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_get_tag_name(element, name);
}
#define dom_element_get_tag_name(e, n) dom_element_get_tag_name( \
		(dom_element *) (e), (struct dom_string **) (n))

static inline dom_exception dom_element_get_attribute(
		struct dom_element *element, struct dom_string *name, 
		struct dom_string **value)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_get_attribute(element, name, value);
}
#define dom_element_get_attribute(e, n, v) dom_element_get_attribute( \
		(dom_element *) (e), (struct dom_string *) (n), \
		(struct dom_string **) (v))

static inline dom_exception dom_element_set_attribute(
		struct dom_element *element, struct dom_string *name, 
		struct dom_string *value)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_set_attribute(element, name, value);
}
#define dom_element_set_attribute(e, n, v) dom_element_set_attribute( \
		(dom_element *) (e), (struct dom_string *) (n), \
		(struct dom_string *) (v))

static inline dom_exception dom_element_remove_attribute(
		struct dom_element *element, struct dom_string *name)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_remove_attribute(element, name);
}
#define dom_element_remove_attribute(e, n) dom_element_remove_attribute( \
		(dom_element *) (e), (struct dom_string *) (n))

static inline dom_exception dom_element_get_attribute_node(
		struct dom_element *element, struct dom_string *name, 
		struct dom_attr **result)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_get_attribute_node(element, name, result);
}
#define dom_element_get_attribute_node(e, n, r)  \
		dom_element_get_attribute_node((dom_element *) (e), \
		(struct dom_string *) (n), (struct dom_attr **) (r))

static inline dom_exception dom_element_set_attribute_node(
		struct dom_element *element, struct dom_attr *attr, 
		struct dom_attr **result)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_set_attribute_node(element, attr, result);
}
#define dom_element_set_attribute_node(e, a, r) \
		dom_element_set_attribute_node((dom_element *) (e), \
		(struct dom_attr *) (a), (struct dom_attr **) (r))

static inline dom_exception dom_element_remove_attribute_node(
		struct dom_element *element, struct dom_attr *attr, 
		struct dom_attr **result)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_remove_attribute_node(element, attr, 
			result);
}
#define dom_element_remove_attribute_node(e, a, r) \
		dom_element_remove_attribute_node((dom_element *) (e), \
		(struct dom_attr *) (a), (struct dom_attr **) (r))


static inline dom_exception dom_element_get_elements_by_tag_name(
		struct dom_element *element, struct dom_string *name,
		struct dom_nodelist **result)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_get_elements_by_tag_name(element, name,
			result);
}
#define dom_element_get_elements_by_tag_name(e, n, r) \
		dom_element_get_elements_by_tag_name((dom_element *) (e), \
		(struct dom_string *) (n), (struct dom_nodelist **) (r))

static inline dom_exception dom_element_get_attribute_ns(
		struct dom_element *element, struct dom_string *namespace, 
		struct dom_string *localname, struct dom_string **value)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_get_attribute_ns(element, namespace, 
			localname, value);
}
#define dom_element_get_attribute_ns(e, n, l, v) \
		dom_element_get_attribute_ns((dom_element *) (e), \
		(struct dom_string *) (n), (struct dom_string *) (l), \
		(struct dom_string **) (v))

static inline dom_exception dom_element_set_attribute_ns(
		struct dom_element *element, struct dom_string *namespace, 
		struct dom_string *qname, struct dom_string *value)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_set_attribute_ns(element, namespace, 
			qname, value);
}
#define dom_element_set_attribute_ns(e, n, l, v) \
		dom_element_set_attribute_ns((dom_element *) (e), \
		(struct dom_string *) (n), (struct dom_string *) (l), \
		(struct dom_string *) (v))


static inline dom_exception dom_element_remove_attribute_ns(
		struct dom_element *element, struct dom_string *namespace, 
		struct dom_string *localname)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_remove_attribute_ns(element, namespace, 
			localname);
}
#define dom_element_remove_attribute_ns(e, n, l) \
		dom_element_remove_attribute_ns((dom_element *) (e), \
		(struct dom_string *) (n), (struct dom_string *) (l))


static inline dom_exception dom_element_get_attribute_node_ns(
		struct dom_element *element, struct dom_string *namespace, 
		struct dom_string *localname, struct dom_attr **result)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_get_attribute_node_ns(element, namespace, 
			localname, result);
}
#define dom_element_get_attribute_node_ns(e, n, l, r) \
		dom_element_get_attribute_node_ns((dom_element *) (e), \
		(struct dom_string *) (n), (struct dom_string *) (l), \
		(struct dom_attr **) (r))

static inline dom_exception dom_element_set_attribute_node_ns(
		struct dom_element *element, struct dom_attr *attr, 
		struct dom_attr **result)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_set_attribute_node_ns(element, attr,
			result);
}
#define dom_element_set_attribute_node_ns(e, a, r) \
		dom_element_set_attribute_node_ns((dom_element *) (e), \
		(struct dom_attr *) (a), (struct dom_attr **) (r))

static inline dom_exception dom_element_get_elements_by_tag_name_ns(
		struct dom_element *element, struct dom_string *namespace,
		struct dom_string *localname, struct dom_nodelist **result)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_get_elements_by_tag_name_ns(element, 
			namespace, localname, result);
}
#define dom_element_get_elements_by_tag_name_ns(e, n, l, r) \
		dom_element_get_elements_by_tag_name_ns((dom_element *) (e), \
		(struct dom_string *) (n), (struct dom_string *) (l), \
		(struct dom_nodelist **) (r))

static inline dom_exception dom_element_has_attribute(
		struct dom_element *element, struct dom_string *name, 
		bool *result)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_has_attribute(element, name, result);
}
#define dom_element_has_attribute(e, n, r) dom_element_has_attribute( \
		(dom_element *) (e), (struct dom_string *) (n), \
		(bool *) (r))

static inline dom_exception dom_element_has_attribute_ns(
		struct dom_element *element, struct dom_string *namespace, 
		struct dom_string *localname, bool *result)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_has_attribute_ns(element, namespace, 
			localname, result);
}
#define dom_element_has_attribute_ns(e, n, l, r) dom_element_has_attribute_ns(\
		(dom_element *) (e), (struct dom_string *) (n), \
		(struct dom_string *) (l), (bool *) (r))

static inline dom_exception dom_element_get_schema_type_info(
		struct dom_element *element, struct dom_type_info **result)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_get_schema_type_info(element, result);
}
#define dom_element_get_schema_type_info(e, r) \
		dom_element_get_schema_type_info((dom_element *) (e), \
		(struct dom_type_info **) (r))

static inline dom_exception dom_element_set_id_attribute(
		struct dom_element *element, struct dom_string *name, 
		bool is_id)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_set_id_attribute(element, name, is_id);
}
#define dom_element_set_id_attribute(e, n, i) \
		dom_element_set_id_attribute((dom_element *) (e), \
		(struct dom_string *) (n), (bool) (i))

static inline dom_exception dom_element_set_id_attribute_ns(
		struct dom_element *element, struct dom_string *namespace, 
		struct dom_string *localname, bool is_id)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_set_id_attribute_ns(element, namespace,
			localname, is_id);
}
#define dom_element_set_id_attribute_ns(e, n, l, i) \
		dom_element_set_id_attribute_ns((dom_element *) (e), \
		(struct dom_string *) (n), (struct dom_string *) (l), \
		(bool) (i))

static inline dom_exception dom_element_set_id_attribute_node(
		struct dom_element *element, struct dom_attr *id_attr, 
		bool is_id)
{
	return ((dom_element_vtable *) ((dom_node *) element)->vtable)->
			dom_element_set_id_attribute_node(element, id_attr,
			is_id);
}
#define dom_element_set_id_attribute_node(e, a, i) \
		dom_element_set_id_attribute_node((dom_element *) (e), \
		(struct dom_attr *) (a), (bool) (i))

#endif