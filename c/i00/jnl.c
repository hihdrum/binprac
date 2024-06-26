#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jnl.h"

void JnlHeader_Print(struct jnl_header * h)
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

int JnlHeader_DataLen(struct jnl_header * h)
{
  char dataLenBuf[JNL_DATA_LEN + 1];
  memcpy(dataLenBuf, h->dataLen, JNL_DATA_LEN);
  dataLenBuf[JNL_DATA_LEN] = '\0';

  int dataLen = atoi(dataLenBuf);

  return dataLen;
}
