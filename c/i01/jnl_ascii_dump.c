#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "jnl.h"

void PrintHeader(struct jnl_header * h)
{
  printf("HEADER,");
  JnlHeader_Print(h);
  putchar('\n');
}

void WriteData(char *data, int len)
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

void ToPrintable(char *pc, int len)
{
  for(int i = 0; i < len; i++)
  {
    if(!isprint(pc[i]))
    {
      pc[i] = '.';
    }
  }
}

FILE * JnlFileOpen(char *name)
{
  FILE *fp = fopen(name, "r");
  if(NULL == fp)
  {
    perror("fopen異常");
    exit(1);
  }

  return fp;
}

void JnlFileClose(FILE *fp)
{
  int retFclose = fclose(fp);
  if(0 != retFclose)
  {
    perror("fclose異常");
    exit(1);
  }
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

    PrintHeader(&jnlh);

    int dataLen = JnlHeader_DataLen(&jnlh);

    retFread = fread(dataBuffer, sizeof(char), dataLen, fp);
    if(retFread < dataLen)
    {
      perror("データfread異常");
      exit(1);
    }

    ToPrintable(dataBuffer, dataLen);
    WriteData(dataBuffer, dataLen);
  }
}

void asciiDumpFiles(char *names[], int num)
{
  for(int i = 0; i < num; i++)
  {
    FILE *fp = JnlFileOpen(names[i]);
    dumpStream(fp);
    JnlFileClose(fp);
  }
}

int main(int argc, char *argv[])
{
  dataBuffer = malloc(dataBufferSize);
  if(NULL == dataBuffer)
  {
    perror("malloc異常");
    exit(1);
  }

  if(argc == 1)
  {
    dumpStream(stdin);
  }
  else if(argc == 2 && 0 == strcmp("-h", argv[1]))
  {
    fprintf(stderr, "Usage : %s ファイル名\n", argv[0]);
    exit(1);
  }
  else
  {
    char **names = argv + 1;
    int num = argc - 1;

    asciiDumpFiles(names, num);
  }

  return 0;
}
