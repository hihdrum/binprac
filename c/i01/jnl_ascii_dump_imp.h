#ifndef JNL_ASCII_DUMP_IMP_H
#define JNL_ASCII_DUMP_IMP_H

#include <stdio.h>

void write_data(char *data, int len);
void update_printable(char *pc, int len);
void dump_stream(FILE *fp);

#endif
