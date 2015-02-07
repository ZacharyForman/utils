#ifndef SOCKET_H
#define SOCKET_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

namespace utils {

class Socket {
public:
  friend class ServerSocket;

  Socket(const Socket &s);
  explicit Socket(const char *address, const char *port);
  ~Socket();
  int write(const char *buffer, int length);
  int read(char *buffer, int length);
  explicit operator bool() const;
  Socket &operator=(const Socket &s);

private:
  Socket(int sock);

  int *ref;
  int sock;
};

class ServerSocket {
public:
  ServerSocket(const ServerSocket &s);
  explicit ServerSocket(const char *port);
  ~ServerSocket();
  Socket accept();
  template<class Handler> void async_accept(Handler h);
  explicit operator bool() const;
  void operator=(ServerSocket) = delete;

private:
  int *ref;
  int sock;
};

} // utils

#endif // __cplusplus < 201100L

#endif // SOCKET_H
