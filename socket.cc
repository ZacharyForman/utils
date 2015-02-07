#include "net_error.h"
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
  err = s.err;
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
  err = OK;
  return;
err:
  err = net_err();
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
  len = ::write(sock, buf, len);
  if (len < 0) {
    err = net_err();
  }
  return len;
}

int Socket::read(char *buf, int len)
{
  ::std::lock_guard<::std::mutex> lock(m);
  len = ::read(sock, buf, len);
  if (len < 0) {
    err = net_err();
  }
  return len;
}

Socket::operator bool() const
{
  return !err;
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
  err = s.err;
  if (ref != nullptr) (*ref)++;
}

Socket::Socket(int s, Error e)
{
  if (s < 0) goto err;
  sock = s;
  ref = new ::std::atomic<int>(1);
  err = OK;
  return;
err:
  err = e;
  sock = -1;
  ref = nullptr;
}

Error Socket::error() const
{
  return err;
}

} //utils

#endif // __cplusplus >= 201100L
