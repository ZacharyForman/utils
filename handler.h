#ifndef HANDLER_H
#define HANDLER_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include "net_error.h"
#include "serversocket.h"
#include <future>
#include <thread>

namespace utils {

template<typename Handler>
::std::future<Error> listen_and_serve(int port, int queue_length, Handler f)
{
  return ::std::async([=]() {
    ServerSocket ss(port, queue_length);

    if (!ss) {
      return ss.error();
    }

    for (;;) {
      Socket s = ss.accept();
      if (s) {
        ::std::thread(f, s).detach();
      }
    }
    return OK;
  });
}

} // utils

#endif // __cplusplus < 201100L

#endif // HANDLER_H
