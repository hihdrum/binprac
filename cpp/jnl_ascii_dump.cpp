#include <iostream>
#include <fstream>
#include <cstring>

#include "jnl.h"

int main(int argc, char *argv[])
{
  if(argc == 1)
  {
    Jnl::record::proc(std::cin, std::cout);
  }
  else if(argc == 2 && 0 == strcmp("-h", argv[1]))
  {
    std::cerr << "Usage : " << argv[0] << "ファイル名" << std::endl;
  }
  else
  {
    for(int i = 1; i < argc; ++i)
    {
      std::ifstream ifs{argv[i]};
      Jnl::record::proc(ifs, std::cout);
    }
  }

  return 0;
}
