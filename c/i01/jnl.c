#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jnl.h"

void JnlHeader_Print(const struct jnl_header * const h)
{
  printf("%*.*s/%*.*s/%*.*s,%*.*s:%*.*s:%*.*s.%*.*s,%*.*s,%*.*s",
      JNL_YEAR_LEN, JNL_YEAR_LEN, h->year,
      JNL_MONTH_LEN, JNL_MONTH_LEN, h->month,
      JNL_DAY_LEN, JNL_DAY_LEN, h->day,
      JNL_HOUR_LEN, JNL_HOUR_LEN, h->hour,
      JNL_MINUTE_LEN, JNL_MINUTE_LEN, h->minute,
      JNL_SECOND_LEN, JNL_SECOND_LEN, h->second,
      JNL_MSECOND_LEN, JNL_MSECOND_LEN, h->msecond,
      JNL_KIND_LEN, JNL_KIND_LEN, h->kind,
      JNL_DATA_LEN, JNL_DATA_LEN, h->dataLen);
}

int JnlHeader_DataLen(const struct jnl_header * const h)
{
  char dataLenBuf[JNL_DATA_LEN + 1];
  memcpy(dataLenBuf, h->dataLen, JNL_DATA_LEN);
  dataLenBuf[JNL_DATA_LEN] = '\0';

  const int dataLen = atoi(dataLenBuf);

  return dataLen;
}

int JnlHeader_Read(struct jnl_header *pjnh, FILE *in)
{
  size_t retFread = fread(pjnh, sizeof(struct jnl_header), 1, in);
  if(1 != retFread)
  {
    if(feof(in))
    {
      return EOF;
    }

    perror("ジャーナルヘッダfread異常");
    exit(1);
  }

  return 0;
}

int JnlRecord_ReadData(struct jnl_record *pjnr, FILE *in)
{
  const int dataLen = JnlHeader_DataLen(&pjnr->header);

  int retFread = fread(pjnr->data, sizeof(char), dataLen, in);
  if(retFread < dataLen)
  {
    perror("データfread異常");
    exit(1);
  }

  return 0;
}

int JnlRecord_Read(struct jnl_record *pjnr, FILE *in)
{
  int retReadJnlHeader = JnlHeader_Read(&pjnr->header, in);
  if(0 != retReadJnlHeader)
  {
    return EOF;
  }

  JnlRecord_ReadData(pjnr, in);

  return 0;
}
