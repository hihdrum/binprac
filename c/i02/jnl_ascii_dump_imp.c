#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "jnl.h"
#include "jnl_ascii_dump_imp.h"

const int bufferSize = 20 * 1024 * 1024;
char *buffer;

void write_data(const char *data, const int len)
{
  printf("DATA:");
  const int retFwrite = fwrite(data, sizeof(char), len, stdout);
  if(retFwrite != len)
  {
    perror("データfwrite異常");
    exit(1);
  }
  putchar('\n');
}

void update_printable(char * const pc, const int len)
{
  for(int i = 0; i < len; i++)
  {
    if(!isprint(pc[i]))
      pc[i] = '.';
  }
}

void dump_stream(FILE * const fp)
{
  while(1)
  {
    struct jnl_record * const pjnr = (struct jnl_record *)buffer;
    const int retReadJnlRecord = JnlRecord_Read(pjnr, fp);
    if(0 != retReadJnlRecord)
      break;

    JnlHeader_PrintToAsciiDump(&pjnr->header, stdout);

    const int dataLen = JnlHeader_DataLen(&pjnr->header);

    update_printable(pjnr->data, dataLen);
    write_data(pjnr->data, dataLen);
  }
}

void dump_file_stream(FILE * const fp)
{
  int fd = fileno(fp);
  if(-1 == fd)
  {
    perror("fileno異常");
    exit(1);
  }

  struct stat stat;
  int retFstat = fstat(fd, &stat);
  if(0 != retFstat)
  {
    perror("fstat異常");
    exit(1);
  }

  int fileSize = stat.st_size;
  int retRead = fread(buffer, sizeof(char), fileSize, fp);
  if(retRead != fileSize)
  {
    if(feof(fp))
      return;

    perror("fread異常");
    exit(1);
  }

  struct jnl_record * pjnr = (struct jnl_record *)buffer;
  const void * pend = (void *)pjnr + fileSize;

  while((void *)pjnr < pend)
  {
    JnlHeader_PrintToAsciiDump(&pjnr->header, stdout);
    const int dataLen = JnlHeader_DataLen(&pjnr->header);
    update_printable(pjnr->data, dataLen);
    write_data(pjnr->data, dataLen);

    pjnr = (struct jnl_record *)((void *)pjnr->data + dataLen);
  }
}
