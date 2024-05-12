#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/* 1バイトデータのデータ長len指定の書込み */
void asciiDump(char *pc, int len)
{
  char *tpc = pc;
  for(int i = 0; i < len; i++, tpc++)
  {
    if(!isprint(*tpc))
    {
      *tpc = '.';
    }
  }

  size_t retFwrite = fwrite(pc, sizeof(char), len, stdout);
  if(retFwrite != len)
  {
    perror("fwrite異常");
    exit(1);
  }
}
