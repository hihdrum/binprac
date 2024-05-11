#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "jnl.h"

const int bufferSize = 20 * 1024 * 1024;
char *buffer;

void write_data(char *data, int len)
{
  printf("DATA:");
  int retFwrite = fwrite(data, sizeof(char), len, stdout);
  if(retFwrite != len)
  {
    perror("データfwrite異常");
    exit(1);
  }
  putchar('\n');
}

void update_printable(char *pc, int len)
{
  for(int i = 0; i < len; i++)
  {
    if(!isprint(pc[i]))
      pc[i] = '.';
  }
}

void dump_stream(FILE *fp)
{
  while(1)
  {
    struct jnl_record * const pjnr = (struct jnl_record *)buffer;
    int retReadJnlRecord = JnlRecord_Read(pjnr, fp);
    if(0 != retReadJnlRecord)
      break;

    JnlHeader_PrintToAsciiDump(&pjnr->header, stdout);

    int dataLen = JnlHeader_DataLen(&pjnr->header);

    update_printable(pjnr->data, dataLen);
    write_data(pjnr->data, dataLen);
  }
}

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
