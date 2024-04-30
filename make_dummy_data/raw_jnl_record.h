#ifndef RAW_JNL_RECORD_H
#define RAW_JNL_RECORD_H

#include <stdio.h>
#include <time.h>
#include "raw_jnl_header.h"

struct raw_jnl_record
{
  struct raw_jnl_header header;
  char data[0];

} __attribute__((packed));

int RawJnlRecord_GetTotalLen(const struct raw_jnl_record * const r);
int RawJnlRecord_WriteRandomRecord(struct raw_jnl_record * const r, const unsigned char uc, FILE * const stream);

void RawJnlRecord_WriteJnlRecords(struct tm * pStartDateTime, int * pMSecond, const int maxSize, FILE *stream);

#endif
