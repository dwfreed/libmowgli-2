/*
 * Copyright (c) 2012 Elizabeth J. Myers. All rights reserved. 
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __MOWGLI_VIO_H__
#define __MOWGLI_VIO_H__

#include "vio/vio-types.h"

extern mowgli_vio_t * mowgli_vio_create(void *userdata);
extern void mowgli_vio_init(mowgli_vio_t *vio, void *userdata);
extern int mowgli_vio_pollable_create(mowgli_vio_t *vio, mowgli_eventloop_t *eventloop);
extern void mowgli_vio_pollable_destroy(mowgli_vio_t *vio);
extern void mowgli_vio_destroy(mowgli_vio_t *vio);

extern int mowgli_vio_default_socket(mowgli_vio_t *vio, int family, int type, int proto);
extern int mowgli_vio_default_listen(mowgli_vio_t *vio, int backlog);
extern int mowgli_vio_default_accept(mowgli_vio_t *vio, mowgli_vio_t *newvio);
extern int mowgli_vio_default_connect(mowgli_vio_t *vio);
extern int mowgli_vio_default_read(mowgli_vio_t *vio, void *buffer, size_t len);
extern int mowgli_vio_default_write(mowgli_vio_t *vio, void *buffer, size_t len);
extern int mowgli_vio_default_error(mowgli_vio_t *vio);
extern int mowgli_vio_default_close(mowgli_vio_t *vio);

extern int mowgli_vio_openssl_setssl(mowgli_vio_t *vio);
/* These are void ptr's so they can be null ops if SSL isn't available */
extern void * mowgli_vio_openssl_getsslhandle(mowgli_vio_t *vio);
extern void * mowgli_vio_openssl_getsslcontext(mowgli_vio_t *vio);

extern mowgli_vio_ops_t mowgli_vio_default_ops;

#define mowgli_vio_set_op(vio, operation, func) (vio)->ops.operation = func;

static inline int mowgli_vio_socket(mowgli_vio_t *vio, int family, int type, int flags)
{
	return vio->ops.socket(vio, family, type, flags);
}

static inline int mowgli_vio_listen(mowgli_vio_t *vio, int backlog)
{
	return vio->ops.listen(vio, backlog);
}

static inline int mowgli_vio_accept(mowgli_vio_t *vio, mowgli_vio_t *newvio)
{
	return vio->ops.accept(vio, newvio);
}

static inline int mowgli_vio_connect(mowgli_vio_t *vio)
{
	return vio->ops.connect(vio);
}

static inline int mowgli_vio_read(mowgli_vio_t *vio, void *buffer, size_t len)
{
	return vio->ops.read(vio, buffer, len);
}

static inline int mowgli_vio_write(mowgli_vio_t *vio, void *buffer, size_t len)
{
	return vio->ops.write(vio, buffer, len);
}

static inline int mowgli_vio_error(mowgli_vio_t *vio)
{
	return vio->ops.error(vio);
}

static inline int mowgli_vio_close(mowgli_vio_t *vio)
{
	return vio->ops.close(vio);
}

#endif
