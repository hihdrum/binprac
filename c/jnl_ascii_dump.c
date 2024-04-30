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

void printJnlHeader(struct jnl_header * h)
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

const int dataBufferSize = 20 * 1024 * 1024;
char *dataBuffer;

void dumpStream(FILE *fp)
{
  while(1)
  {
    struct jnl_header jnlh;
    size_t retFread = fread(&jnlh, sizeof(jnlh), 1, fp);
    if(1 != retFread)
    {
      if(feof(fp))
      {
        break;
      }

      perror("ジャーナルヘッダfread異常");
      exit(1);
    }

    printf("HEADER:");
    printJnlHeader(&jnlh);
    putchar('\n');

    char dataLenBuf[JNL_DATA_LEN + 1];
    memcpy(dataLenBuf, jnlh.dataLen, JNL_DATA_LEN);
    dataLenBuf[JNL_DATA_LEN] = '\0';

    int dataLen = atoi(dataLenBuf);

    retFread = fread(dataBuffer, sizeof(char), dataLen, fp);
    if(retFread < dataLen)
    {
      perror("データfread異常");
      exit(1);
    }

    printf("DATA:");
    asciiDump(dataBuffer, dataLen);
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
