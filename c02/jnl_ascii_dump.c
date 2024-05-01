#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void asciiDump(char *pc, int len);

#define JNL_YEAR_LEN (4)
#define JNL_MONTH_LEN (2)
#define JNL_DAY_LEN (2)

#define JNL_HOUR_LEN (2)
#define JNL_MINUTE_LEN (2)
#define JNL_SECOND_LEN (2)
#define JNL_MSECOND_LEN (3)

#define JNL_KIND_LEN (1)
#define JNL_DATA_LEN (8)

/*-----------------------------------------------------*/
struct jnl_header
{
  char year[JNL_YEAR_LEN];
  char month[JNL_MONTH_LEN];
  char day[JNL_DAY_LEN];

  char hour[JNL_HOUR_LEN];
  char minute[JNL_MINUTE_LEN];
  char second[JNL_SECOND_LEN];
  char msecond[JNL_MSECOND_LEN];

  char kind[JNL_KIND_LEN];
  char dataLen[JNL_DATA_LEN];

} __attribute__((packed));

void jnl_header_read(struct jnl_header *h, FILE *in)
{
  size_t retFread = fread(h, sizeof(struct jnl_header), 1, in);
  if(1 != retFread)
  {
    if(feof(in))
    {
      return;
    }

    perror("ジャーナルヘッダfread異常");
    exit(1);
  }
}

int jnl_header_dataLen(const struct jnl_header * const h)
{
  char dataLenBuf[JNL_DATA_LEN + 1];
  memcpy(dataLenBuf, h->dataLen, JNL_DATA_LEN);
  dataLenBuf[JNL_DATA_LEN] = '\0';

  int dataLen = atoi(dataLenBuf);
  return dataLen;
}

/*-----------------------------------------------------*/
struct jnl_record
{
  struct jnl_header h;
  char data[0];
} __attribute__((packed));

void jnl_record_read(struct jnl_record *r, FILE *in)
{
  jnl_header_read(&r->h, in);
  if(feof(in))
  {
    return;
  }

  int dataLen = jnl_header_dataLen(&r->h);
  size_t retFread = fread(r->data, sizeof(char), dataLen, in);
  if(retFread < dataLen)
  {
    perror("データfread異常");
    exit(1);
  }
}

/*-----------------------------------------------------*/
void jnl_record_dump_printHeader(struct jnl_record *r)
{
  struct jnl_header *h = &r->h;

  printf("HEADER,");
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
  putchar('\n');
}


const int dataBufferSize = 20 * 1024 * 1024;
char *dataBuffer;

void dumpStream(FILE *fp)
{
  while(1)
  {
    struct jnl_record *pjlr = (struct jnl_record *)dataBuffer;
    jnl_record_read(pjlr, fp);
    if(feof(fp))
    {
      break;
    }

    jnl_record_dump_printHeader(pjlr);

    int dataLen = jnl_header_dataLen(&pjlr->h);
    printf("DATA:");
    asciiDump(pjlr->data, dataLen);
    putchar('\n');
  }
}

void asciiDumpArgs(int argc, char *argv[])
{
  for(int i = 1; i < argc; i++)
  {
    FILE *fp = fopen(argv[i], "r");
    if(NULL == fp)
    {
      perror("fopen異常");
      exit(1);
    }

    dumpStream(fp);

    int retFclose = fclose(fp);
    if(0 != retFclose)
    {
      perror("fclose異常");
      exit(1);
    }
  }
}

int main(int argc, char *argv[])
{
  int isUseStdin = 0;
  if(argc == 1)
  {
    isUseStdin = 1;
  }
  else if(argc == 2 && 0 == strcmp("-h", argv[1]))
  {
    fprintf(stderr, "Usage : %s ファイル名\n", argv[0]);
    exit(1);
  }

  dataBuffer = malloc(dataBufferSize);
  if(NULL == dataBuffer)
  {
    perror("malloc異常");
    exit(1);
  }

  if(isUseStdin)
  {
    dumpStream(stdin);
  }
  else
  {
    asciiDumpArgs(argc, argv);
  }

  return 0;
}
