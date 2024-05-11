#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "jnl.h"
#include "jnl_ascii_dump_imp.h"

extern const int bufferSize;
extern char *buffer;

int main(int argc, char *argv[])
{
  buffer = malloc(bufferSize);
  if(NULL == buffer)
  {
    perror("malloc異常");
    exit(1);
  }

  if(argc == 1)
  {
    dump_stream(stdin);
  }
  else if(argc == 2 && 0 == strcmp("-h", argv[1]))
  {
    fprintf(stderr, "Usage : %s ファイル名\n", argv[0]);
    exit(1);
  }
  else
  {
    char **names = argv + 1;
    int num = argc - 1;

    JnlFile_ProcFiles(names, num, dump_stream);
  }

  return 0;
}
