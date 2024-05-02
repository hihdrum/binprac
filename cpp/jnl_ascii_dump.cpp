#include <iostream>
#include <fstream>
#include <cstring>

#include "jnl.h"

int main(int argc, char *argv[])
{
  constexpr int buffer_size = 20 * 1024 * 1024;

  if(argc == 1)
  {
    std::vector<char> buffer = std::vector<char>(buffer_size);
    Jnl::record::proc(buffer, std::cin, std::cout);
  }
  else if(argc == 2 && 0 == strcmp("-h", argv[1]))
  {
    std::cerr << "Usage : " << argv[0] << "ファイル名" << std::endl;
  }
  else
  {
    std::vector<char> buffer = std::vector<char>(buffer_size);

    for(int i = 1; i < argc; ++i)
    {
      std::ifstream ifs{argv[i]};
      Jnl::record::proc(buffer, ifs, std::cout);
    }
  }

  return 0;
}
