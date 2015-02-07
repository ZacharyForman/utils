#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include "net_error.h"
#include "socket.h"

#include <atomic>
#include <mutex>

namespace utils {
class ServerSocket {
public:
  ServerSocket(const ServerSocket &s);
  explicit ServerSocket(int port, int queue_length = 1);
  ~ServerSocket();

  Socket accept();

  explicit operator bool() const;

  ServerSocket &operator=(const ServerSocket &ss);

  Error error() const;

private:
  ::std::mutex m;
  ::std::atomic<int> *ref;
  Error err;
  int sock;
};

} // utils

#endif // __cplusplus < 201100L

#endif // SERVER_SOCKET_H
