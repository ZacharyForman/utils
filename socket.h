#ifndef SOCKET_H
#define SOCKET_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include "net_error.h"

#include <atomic>
#include <mutex>

namespace utils {

class Socket {
public:
  friend class ServerSocket;

  Socket(const Socket &s);
  explicit Socket(const char *host, int port);
  ~Socket();

  int write(const char *buffer, int length);
  int read(char *buffer, int length);

  explicit operator bool() const;

  Socket &operator=(const Socket &s);

  Error error() const;

private:
  Socket(int sock, Error e = OK);

  ::std::mutex m;
  ::std::atomic<int> *ref;
  Error err;
  int sock;
};

} // utils

#endif // __cplusplus < 201100L

#endif // SOCKET_H
