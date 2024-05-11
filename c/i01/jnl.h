#ifndef JNL_H
#define JNL_H

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

void JnlHeader_Print(const struct jnl_header * const h, FILE *out);
void JnlHeader_PrintToAsciiDump(const struct jnl_header * const h, FILE *out);
int JnlHeader_DataLen(const struct jnl_header * const h);
int JnlHeader_Read(struct jnl_header *pjnh, FILE *in);

struct jnl_record
{
  struct jnl_header header;
  char data[0];
};

int JnlRecord_ReadData(struct jnl_record *pjnr, FILE *in);
int JnlRecord_Read(struct jnl_record *pjnr, FILE *in);

FILE *JnlFile_Open(char *name);
void JnlFile_Close(FILE *fp);
void JnlFile_Proc(char *name, void (*proc)(FILE *in));

#endif
