#include "net_error.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <errno.h>

namespace utils {

Error net_err()
{
  switch(errno) {
    case EACCES:            // FALL THROUGH
    case EPERM:             // FALL THROUGH
    case EAFNOSUPPORT:      // FALL THROUGH
    case EPROTONOSUPPORT:   // FALL THROUGH
    case EFAULT:            // FALL THROUGH
    case ENOTSUP:           // FALL THROUGH
    case ENAMETOOLONG:      // FALL THROUGH
    case EINVAL:
      return INVALID_ARGS;
    case EMFILE:            // FALL THROUGH
    case ENFILE:
      return NO_FDS;
    case ENOBUFS:           // FALL THROUGH
    case ENOMEM:
      return NO_MEMORY;
    case EADDRINUSE:        // FALL THROUGH
    case EAGAIN:
      return IN_USE;
    case EBADF:             // FALL THROUGH
    case ENOTSOCK:
      return BAD_SOCKET;
    case EINTR:
      return INTERRUPTED;
    case EISCONN:
      return ALREADY_CONNECTED;
    case ENETUNREACH:
      return NO_NETWORK;
    case ETIMEDOUT:
      return TIMED_OUT;
    case EPIPE:
      return CONN_TERMINATED;
    default:
      return BAD;
  }
}

} // utils

