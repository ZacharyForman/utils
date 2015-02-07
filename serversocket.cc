#include "serversocket.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string.h>

#include <atomic>
#include <mutex>

#if __cplusplus >= 201100L

namespace utils {

ServerSocket::ServerSocket(const ServerSocket &ss)
{
  sock = ss.sock;
  ref = ss.ref;
  if (ref != nullptr) (*ref)++;
}

ServerSocket::ServerSocket(int port, int queue_length)
{
  struct sockaddr_in addr;

  memset((char *)&addr,0,sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons((u_short)port);

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) goto err;
  if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) goto err;
  if (listen(sock, queue_length) < 0) goto err;
  ref = new ::std::atomic<int>(1);
  return;
err:
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
  s = ::accept(sock, nullptr, 0);
  return Socket(s);
}

ServerSocket::operator bool() const
{
  return sock > 0;
}

ServerSocket & ServerSocket::operator=(const ServerSocket &ss)
{
  if (ref != nullptr) (*ref)--;
  if (ref != nullptr && *ref == 0) {
    delete ref;
    close(sock);
  }

  sock = ss.sock;
  ref = ss.ref;
  if (ref != nullptr) (*ref)++;
}

} //utils

#endif // __cplusplus >= 201100L
