#include "socket.h"

#include <arpa/inet.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string.h>

#include <atomic>
#include <mutex>

#if __cplusplus >= 201100L

namespace utils {

Socket::Socket(const Socket &s)
{
  sock = s.sock;
  ref = s.ref;
  if (ref != nullptr) (*ref)++;
}

Socket::Socket(const char *host, int port)
{
  struct sockaddr_in addr;
  memset((char *)&addr,0,sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(host);

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) goto err;
  if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) goto err;

  ref = new ::std::atomic<int>(1);
  return;
err:
  sock = -1;
  ref = nullptr;
}

Socket::~Socket()
{
  if (ref != nullptr) (*ref)--;
  if (ref != nullptr && *ref == 0) {
    delete ref;
    close(sock);
  }
}

int Socket::write(const char *buf, int len)
{
  ::std::lock_guard<::std::mutex> lock(m);
  return ::write(sock, buf, len);
}

int Socket::read(char *buf, int len)
{
  ::std::lock_guard<::std::mutex> lock(m);
  return ::read(sock, buf, len);
}

Socket::operator bool() const
{
  return sock > 0;
}

Socket &Socket::operator=(const Socket &s)
{
  if (ref != nullptr) (*ref)--;
  if (ref != nullptr && *ref == 0) {
    delete ref;
    close(sock);
  }

  sock = s.sock;
  ref = s.ref;
  if (ref != nullptr) (*ref)++;
}

Socket::Socket(int s)
{
  if (s < 0) goto err;
  sock = s;
  ref = new ::std::atomic<int>(1);
  return;
err:
  sock = -1;
  ref = nullptr;
}


} //utils

#endif // __cplusplus >= 201100L
