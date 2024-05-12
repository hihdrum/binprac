#ifndef JNL_ASCII_DUMP_IMP_H
#define JNL_ASCII_DUMP_IMP_H

#include <stdio.h>

void write_data(const char * const data, const int len, FILE *out);
void update_printable(char * const pc, const int len);
void dump_stream(FILE * const fp);
void dump_file_stream(FILE * const fp);

#endif
