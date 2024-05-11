#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "jnl.h"
#include "jnl_ascii_dump_imp.h"

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
