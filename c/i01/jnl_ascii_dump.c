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

const int bufferSize = 20 * 1024 * 1024;
char *buffer;

void dumpStream(FILE *fp)
{
  while(1)
  {
    struct jnl_record * const pjnr = (struct jnl_record *)buffer;

    int retReadJnlHeader = JnlHeader_Read(&pjnr->header, fp);
    if(0 != retReadJnlHeader)
    {
      break;
    }

    PrintHeader(&pjnr->header);

    int dataLen = JnlHeader_DataLen(&pjnr->header);
    JnlRecord_ReadData(pjnr, fp);
#if 0
    int dataLen = JnlHeader_DataLen(&pjnr->header);

    int retFread = fread(pjnr->data, sizeof(char), dataLen, fp);
    if(retFread < dataLen)
    {
      perror("データfread異常");
      exit(1);
    }
#endif

    ToPrintable(pjnr->data, dataLen);
    WriteData(pjnr->data, dataLen);
  }
}

void ProcJnlFile(char *name)
{
  FILE *fp = JnlFileOpen(name);
  dumpStream(fp);
  JnlFileClose(fp);
}

void asciiDumpFiles(char *names[], int num)
{
  for(int i = 0; i < num; i++)
  {
    ProcJnlFile(names[i]);
  }
}

int main(int argc, char *argv[])
{
  buffer = malloc(bufferSize);
  if(NULL == buffer)
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
