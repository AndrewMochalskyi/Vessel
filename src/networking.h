/* 
 *
 * The MIT License
*
* Copyright (c) Andrew Mochalskyi
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
 */


#ifndef NETWORKING_H
#define NETWORKING_H

#include <openssl/ssl.h>
#include "ringbuf.h"


#define BUFSIZE 256


/*!
 * Create a non-blocking socket and make it listen on the specfied address and
 * port
 */
int make_listen(const char *, const char *);

/*! Accept a connection and add it to the right epollfd */
int accept_connection(const int);

/*! Epoll management functions. */
void add_epoll(const int, const int, void *);

/*! Modify state of an already watched FD on the EPOLL event loop */
void mod_epoll(const int, const int, const int, void *);

/*! I/O management functions */
int sendall(const int, uint8_t *, ssize_t, ssize_t *);

/*! Recv all data */
int recvall(const int, Ringbuf *, ssize_t);

/*! SSL/TLS versions */
SSL_CTX *create_ssl_context(void);

/*! Init openssl library */
void openssl_init(void);

/*! Release resources allocated by openssl library */
void openssl_cleanup(void);

/*! Load cert.pem and key.pem certfiles from filesystem */
void load_certificates(SSL_CTX *, const char *, const char *);

/*! Send data like sendall but adding encryption SSL */
int ssl_send(SSL *, uint8_t *, ssize_t, ssize_t *);

/*! Recv data like recvall but adding encryption SSL */
int ssl_recv(SSL *, Ringbuf *, ssize_t);


#endif
