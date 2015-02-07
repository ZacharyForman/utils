#ifndef NET_ERROR_H
#define NET_ERROR_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <errno.h>

namespace utils {

const int PORT_MAX = 65535;

enum Error {
  OK = 0,
  BAD,
  BAD_PORT,
  IN_USE,
  ALREADY_CONNECTED,
  CONNECTION_REFUSED,
  BAD_SOCKET,
  INTERRUPTED,
  NO_NETWORK,
  TIMED_OUT,
  NO_MEMORY,
  NO_FDS,
  INVALID_ARGS,
  CONN_TERMINATED
};

Error net_err();

} // utils

#endif // NET_ERROR_H
