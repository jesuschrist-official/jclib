/*
 * jclib/net/net.h - General networking related stuff.
 */
#ifndef _jclib_net_net_h_
#define _jclib_net_net_h_


#include <stdbool.h>
#include <stdlib.h>

#include "jclib/string.h"


typedef int socket_t;


typedef enum net_status {
    NET_ERROR = -1,
    NET_SUCCESS = 0
} net_status_t;


#define SOCKET_ERROR    (-1)


/*
 * Set the given socket `sock` in non-blocking mode.
 * Returns `NET_SUCESS` in case of success or `NET_ERROR` on failure.
 */
net_status_t socket_set_non_blocking(socket_t sock);


/*
 * Create a new TCP server socket listening on port `port` accepting
 * `max_connections` connections.
 * Returns the socket descriptor on success, or `SOCKET_ERROR` in case of
 * failure.
 */
socket_t socket_create_server_tcp(int port, size_t max_connections);


/*
 * Create a new TCP client to the given `hostname` and `port`.
 * Returns a valid socket in case of success or `NET_ERROR` otherwise.
 */
socket_t socket_create_client_tcp(const char* hostname, int port);


/*
 * Says whereas the given socket as pending data.
 */
bool socket_has_data(socket_t sock);


/*
 * Pull data from TCP client socket, and stores it in the given string.
 * This string must be wiped after use.
 * Returns `NET_ERROR` on error.
 */
net_status_t socket_read(socket_t sock, string_t* str, const char* stop);


/*
 * Consume all data pendig on the socket input.
 */
void socket_flush(socket_t sock);


/*
 * Close the given socket.
 */
void socket_close(socket_t sock);


/*
 * Gently close the given socket:
 * - call shutdown on the socket ;
 * - flush the socket content ;
 * - close the socket ;
 */
void socket_gently_close(socket_t sock);


#endif
