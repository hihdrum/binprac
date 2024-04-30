#include <stdlib.h>
#include <string.h>
#include "raw_jnl_record.h"

void writeDummyData(const unsigned char uc, const size_t num, FILE * const stream);
void randomMsecondAdd(time_t *pTime, int *pMSecond);
unsigned char randomUnsignedChar(void);

int RawJnlRecord_GetTotalLen(const struct raw_jnl_record * const r)
{
  return sizeof(struct raw_jnl_header) + RawJnl_GetDataLen(&r->header);
}

int RawJnlRecord_WriteRandomRecord(struct raw_jnl_record * const r, const unsigned char uc, FILE * const stream)
{
  struct raw_jnl_header * rh = &r->header;
  int retFwrite = fwrite(rh, sizeof(struct raw_jnl_header), 1, stream);
  if(0 == retFwrite)
  {
    perror("fwrite異常");
    exit(1);
  }

  int dataLen = RawJnl_GetDataLen(rh);
  writeDummyData(uc, dataLen, stream);

  return sizeof(struct raw_jnl_record) + dataLen;
}

void writeDummyData(const unsigned char uc, const size_t num, FILE * const stream)
{
  int ret;
  for(int i = 0; i < num; i++)
  {
    ret = fputc(uc, stream);
    if(EOF == ret)
    {
      perror("fputc異常");
      exit(1);
    }
  }
}

/* srandで乱数が初期化されていること。*/
void RawJnlRecord_WriteJnlRecords(struct tm * pStartDateTime, int * pMSecond, const int maxSize, FILE *stream)
{
  time_t time = mktime(pStartDateTime);
  if(-1 == time)
  {
    perror("mktime異常");
    exit(1);
  }

  struct raw_jnl_record rjr;
  struct raw_jnl_header *prjh = &rjr.header;

  int currentSize = 0;
  while(1)
  {
    struct tm *ptime = localtime(&time);
    strftime((char *)prjh, sizeof(struct raw_jnl_header), "%Y%m%d%H%M%S", ptime);

    randomMsecondAdd(&time, pMSecond);
    RawJnl_SetMSecond(prjh, *pMSecond);

    RawJnl_SetRandomKind(prjh);
    RawJnl_SetRandomDataLen(prjh);

    int nextWriteRecordSize = RawJnlRecord_GetTotalLen(&rjr);
    if(maxSize <= currentSize + nextWriteRecordSize)
    {
      break;
    }

    unsigned char uc = randomUnsignedChar();
    int writeDataLen = RawJnlRecord_WriteRandomRecord(&rjr, uc, stream);

    currentSize += writeDataLen;
  }

  struct tm *pRetTm = localtime(&time);
  memcpy(pStartDateTime, pRetTm, sizeof(struct tm));
}

void randomMsecondAdd(time_t *pTime, int *pMSecond)
{
    int msecond_diff = rand() % 100;

    *pMSecond += msecond_diff;
    if((*pMSecond - 1000) > 0)
    {
      (*pTime)++;
      *pMSecond -= 1000;
    }
}

unsigned char randomUnsignedChar(void)
{
  return rand() % 256;
}

