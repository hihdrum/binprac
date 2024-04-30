#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/* データ長len指定の1回書込み */
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

  size_t retFwrite = fwrite(pc, len, 1, stdout);
  if(retFwrite != 1)
  {
    perror("fwrite異常");
    exit(1);
  }
}
