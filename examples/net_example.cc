#include "../net.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <future>

#define BUF_LEN 4096

int main(int argc, char **argv)
{
  int port = 8080;
  if (argc > 1) {
    port = atoi(argv[1]);
  }

  const auto handler = [](utils::Socket s) {
    char buf[BUF_LEN];
    int len;
    len = s.read(buf, BUF_LEN - 1);
    buf[len] = 0;
    printf("Got: %s\n", buf);
    s.write(buf, len);
  };

  std::future<utils::Error> f = utils::listen_and_serve(port, 5, handler);
  printf("Result was: %d\n", f.get());
}
