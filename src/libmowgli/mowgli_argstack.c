/*
 * libmowgli: A collection of useful routines for programming.
 * mowgli_argstack.c: Argument stacks.
 *
 * Copyright (c) 2007 William Pitcock <nenolod -at- sacredspiral.co.uk>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "mowgli.h"

static mowgli_object_class_t klass;

static void mowgli_argstack_destroy(void *vptr)
{
	mowgli_argstack_t *self = (mowgli_argstack_t *) vptr;
	mowgli_node_t *n, *tn;

	MOWGLI_LIST_FOREACH_SAFE(n, tn, self->stack.head)
	{
		mowgli_free(n->data);

		mowgli_node_delete(n, &self->stack);
		mowgli_node_free(n);
	}

	mowgli_free(self);
}

void mowgli_argstack_init(void)
{
	mowgli_object_class_init(&klass, "mowgli_argstack_t", mowgli_argstack_destroy, FALSE);
}

mowgli_argstack_t *mowgli_argstack_new_from_va_list(const char *descstr, va_list va)
{
	mowgli_argstack_t *out = mowgli_alloc(sizeof(mowgli_argstack_t));
	mowgli_object_init(mowgli_object(out), descstr, &klass, NULL);
	const char *cp = descstr;

	if (descstr == NULL)
		mowgli_throw_exception_val(mowgli.argstack.invalid_description, NULL);

	while (*cp)
	{
		mowgli_argstack_element_t *e = mowgli_alloc(sizeof(mowgli_argstack_element_t));

		switch(*cp)
		{
		case 's':
			e->data.string = va_arg(va, char *);
			e->type = MOWGLI_ARG_STRING;
			break;
		case 'd':
			e->data.numeric = va_arg(va, int);
			e->type = MOWGLI_ARG_NUMERIC;
			break;
		case 'p':
			e->data.pointer = va_arg(va, void *);
			e->type = MOWGLI_ARG_POINTER;
			break;
		case 'b':
			e->data.boolean = va_arg(va, mowgli_boolean_t);
			e->type = MOWGLI_ARG_BOOLEAN;
			break;
		default:
			va_end(va);
			mowgli_object_unref(out);
			mowgli_throw_exception_val(mowgli.argstack.invalid_description, NULL);
			break;
		}

		mowgli_node_add(e, mowgli_node_create(), &out->stack);
		cp++;
	}

	return out;
}

mowgli_argstack_t *mowgli_argstack_new(const char *descstr, ...)
{
	va_list va;
	mowgli_argstack_t *out;

	if (descstr == NULL)
		mowgli_throw_exception_val(mowgli.argstack.invalid_description, NULL);

	va_start(va, descstr);
	out = mowgli_argstack_new_from_va_list(descstr, va);
	va_end(va);

	return out;
}

const char *mowgli_argstack_pop_string(mowgli_argstack_t *self)
{
	mowgli_node_t *n;
	mowgli_argstack_element_t *e;

	if (self == NULL)
		mowgli_throw_exception_val(mowgli.null_pointer_exception, NULL);

	n = self->stack.head;
	mowgli_node_delete(n, &self->stack);
	e = n->data;
	mowgli_node_free(n);

	return e->data.string;
}

int mowgli_argstack_pop_numeric(mowgli_argstack_t *self)
{
	mowgli_node_t *n;
	mowgli_argstack_element_t *e;

	if (self == NULL)
		mowgli_throw_exception_val(mowgli.null_pointer_exception, 0);

	n = self->stack.head;
	mowgli_node_delete(n, &self->stack);
	e = n->data;
	mowgli_node_free(n);

	return e->data.numeric;
}

mowgli_boolean_t mowgli_argstack_pop_boolean(mowgli_argstack_t *self)
{
	mowgli_node_t *n;
	mowgli_argstack_element_t *e;

	if (self == NULL)
		mowgli_throw_exception_val(mowgli.null_pointer_exception, FALSE);

	n = self->stack.head;
	mowgli_node_delete(n, &self->stack);
	e = n->data;
	mowgli_node_free(n);

	return e->data.boolean;
}

void *mowgli_argstack_pop_pointer(mowgli_argstack_t *self)
{
	mowgli_node_t *n;
	mowgli_argstack_element_t *e;

	if (self == NULL)
		mowgli_throw_exception_val(mowgli.null_pointer_exception, NULL);

	n = self->stack.head;
	mowgli_node_delete(n, &self->stack);
	e = n->data;
	mowgli_node_free(n);

	return e->data.pointer;
}