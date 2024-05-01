#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/* 1文字毎の出力関数呼び出し */
void asciiDump(const char *pc, int len)
{
  for(int i = 0; i < len; i++)
  {
    if(isprint(*pc))
    {
      putchar(*pc);
    }
    else
    {
      putchar('.');
    }
  }
}
