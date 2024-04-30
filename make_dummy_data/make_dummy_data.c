#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "raw_jnl_header.h"
#include "raw_jnl_record.h"

void makeJnlFile(const char * const namePrefix, const int num, const int fileSize, struct tm * const dateTime, int * const msecond)
{
  char fileNameBuffer[256];

  for(int i = 0; i < num; i++)
  {
    int retSnprintf = snprintf(fileNameBuffer, sizeof(fileNameBuffer), "%s%03d", namePrefix, i);
    if(0 > retSnprintf)
    {
      perror("snprintf異常");
      exit(1);
    }
    if(sizeof(fileNameBuffer) <= retSnprintf)
    {
      perror("snprintf : 切詰め");
      exit(1);
    }

    FILE *fp = fopen(fileNameBuffer, "w");
    if(NULL == fp)
    {
      perror("fopen異常");
      fprintf(stderr, "ファイル[%s]が書込みオープンできませんでした。\n", fileNameBuffer);
      exit(1);
    }

    RawJnlRecord_WriteJnlRecords(dateTime, msecond, fileSize, fp);

    int retFclose = fclose(fp);
    if(EOF == retFclose)
    {
      perror("fclose異常");
      exit(1);
    }
  }
}

struct tm dateTimeStrToTm(const char * const str)
{
  struct tm dateTime;
  memset(&dateTime, 0x00, sizeof(dateTime));
  strptime(str, "%Y/%m/%d %H:%M:%S", &dateTime);

  return dateTime;
}

int main(void)
{
  srand(0);

  struct tm dateTime = dateTimeStrToTm("2024/01/01 00:00:00");
  int msecond = 0;

  const int maxFileSize = 10 * 1024 * 1024;
  makeJnlFile("data", 10, maxFileSize, &dateTime, &msecond);

  return 0;
}
