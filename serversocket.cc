#include "net_error.h"
#include "serversocket.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string.h>

#include <atomic>
#include <mutex>

#include <cstdio>
#include <errno.h>

#if __cplusplus >= 201100L

namespace utils {

ServerSocket::ServerSocket(const ServerSocket &ss)
{
  sock = ss.sock;
  ref = ss.ref;
  err = ss.err;
  if (ref != nullptr) (*ref)++;
}

ServerSocket::ServerSocket(int port, int queue_length)
{
  if (port < 0 || port > PORT_MAX) goto err;
  struct sockaddr_in addr;

  memset((char *)&addr,0,sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) goto err;
  if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) goto err;
  if (listen(sock, queue_length) < 0) goto err;
  err = OK;
  ref = new ::std::atomic<int>(1);
  return;
err:
  err = net_err();
  sock = -1;
  ref = nullptr;
}

ServerSocket::~ServerSocket()
{
  if (ref != nullptr) (*ref)--;
  if (ref != nullptr && *ref == 0) {
    delete ref;
    close(sock);
  }
}

Socket ServerSocket::accept()
{
  ::std::lock_guard<::std::mutex> lock(m);
  int s;
  Error e = OK;
  s = ::accept(sock, nullptr, 0);
  if (s < 0) e = net_err();
  return Socket(s, e);
}

ServerSocket::operator bool() const
{
  return !err;
}

ServerSocket & ServerSocket::operator=(const ServerSocket &ss)
{
  if (ref != nullptr) (*ref)--;
  if (ref != nullptr && *ref == 0) {
    delete ref;
    close(sock);
  }

  err = ss.err;
  sock = ss.sock;
  ref = ss.ref;
  if (ref != nullptr) (*ref)++;
}

Error ServerSocket::error() const
{
  return err;
}

} //utils

#endif // __cplusplus >= 201100L
