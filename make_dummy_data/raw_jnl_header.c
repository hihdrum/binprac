#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "raw_jnl_header.h"

char randomKind(void);
int randomDataLen(void);

int RawJnl_GetDataLen(const struct raw_jnl_header * const h)
{
  char buf[RAW_JNL_DATA_LEN + 1];
  memcpy(buf, h->dataLen, RAW_JNL_DATA_LEN);
  buf[RAW_JNL_DATA_LEN] = '\0';

  int ret = atoi(buf);
  return ret;
}

void RawJnl_SetMSecond(struct raw_jnl_header * const h, const int msecond)
{
  char buf[RAW_JNL_MSECOND_LEN + 1];
  snprintf(buf, RAW_JNL_MSECOND_LEN + 1, "%03d", msecond);
  memcpy(h->msecond, buf, RAW_JNL_MSECOND_LEN);
}

void RawJnl_SetKind(struct raw_jnl_header * const h, const char kind)
{
  *h->kind = kind;
}

void RawJnl_SetRandomKind(struct raw_jnl_header * const h)
{
  char kind = randomKind();
  RawJnl_SetKind(h, kind);
}

void RawJnl_SetDataLen(struct raw_jnl_header * const h, const int dataLen)
{
  char buf[RAW_JNL_DATA_LEN + 1];
  snprintf(buf, RAW_JNL_DATA_LEN + 1, "%08d", dataLen);
  memcpy(h->dataLen, buf, RAW_JNL_DATA_LEN);
}

void RawJnl_SetRandomDataLen(struct raw_jnl_header * const h)
{
  int dataLen = randomDataLen();
  RawJnl_SetDataLen(h, dataLen);
}

char randomKind(void)
{
  char kind = 'S';
  if(rand() % 2 == 0)
  {
    kind = 'R';
  }

  return kind;
}

int randomDataLen(void)
{
  return (rand() % (2 * 1024 * 1024)) + 1;
}

