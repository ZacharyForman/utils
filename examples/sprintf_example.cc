#include "../sprintf.h"
#include <cstdio>
#include <string>

using utils::sprintf;

int main()
{
  std::string hello_msg = "Hello world!";
  int integer = 12345;
  std::string msg = sprintf("%v: %v", hello_msg, integer);
  printf("%s\n", msg.c_str());
}
