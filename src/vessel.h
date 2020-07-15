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

#ifndef VESSEL_H
#define VESSEL_H

#include <stdint.h>
#include <openssl/ssl.h>
#include "list.h"


#define MAX_EVENTS	  64


typedef struct client Client;
typedef struct client Server;

typedef struct reply Reply;

struct client {
    const char *addr;
    int fd;
    int epollfd;
    int (*ctx_accept)(Client *);
    int (*ctx_in)(Client *);
    int (*ctx_out)(Client *);
    union {
        Reply *reply;
        void *ptr;
    };
    SSL_CTX *ssl_ctx;
    SSL *ssl;
};


struct socks {
    int epollfd;
    int serversock;
};


struct reply {
    int fd;
    uint8_t *data;
};


typedef struct config {
    /* Epoll events count */
    int epoll_events;
    /* Epoll workers count */
    int epoll_workers;
    const char *addr;
    const char *port;
    int use_ssl;
    const char *certfile;
    const char *keyfile;
    int (*acc_handler)(Client *);
    int (*req_handler)(Client *);
    int (*rep_handler)(Client *);
} Config;


struct server_conf {
    /* Eventfd to break the epoll_wait loop in case of signals */
    int event_fd;
    /* Epoll workers count */
    int epoll_workers;
    /* Epoll max number of events */
    int epoll_max_events;
    /* List of connected clients */
    List *clients;
    /* Certificate file path on the filesystem */
    const char *certfile;
    /* Key file path on the filesystem */
    const char *keyfile;
    /* Encryption flag */
    int encryption;
};

/*! Global instance configuration */
extern struct server_conf instance;

/*! Start the server, based on the configuration parameters passed blocking
   call */
int start_server(Config *);

/*! Stop the server running by using epoll_workers number of eventfd call  this
   way it will stop all running threads */
void stop_server();

/*! Add a connected client to the global instance configuration */
void add_client(Client *);

/*! Run the serveri instanc accept addr port and a Client structure pointer */
int server(const char *, const char *, Client *);


#endif
