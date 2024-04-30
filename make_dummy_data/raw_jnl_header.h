#ifndef RAW_JNL_HEADER_H
#define RAW_JNL_HEADER_H

#define RAW_JNL_KIND_LEN (1)
#define RAW_JNL_MSECOND_LEN (3)
#define RAW_JNL_DATA_LEN (8)

struct raw_jnl_header
{
  char year[4];
  char month[2];
  char day[2];

  char hour[2];
  char minute[2];
  char second[2];
  char msecond[RAW_JNL_MSECOND_LEN];

  char kind[RAW_JNL_KIND_LEN];
  char dataLen[RAW_JNL_DATA_LEN];

} __attribute__((packed));

int RawJnl_GetDataLen(const struct raw_jnl_header * const h);
void RawJnl_SetMSecond(struct raw_jnl_header * const h, const int msecond);
void RawJnl_SetKind(struct raw_jnl_header * const h, const char kind);
void RawJnl_SetRandomKind(struct raw_jnl_header * const h);
void RawJnl_SetDataLen(struct raw_jnl_header * const h, const int dataLen);
void RawJnl_SetRandomDataLen(struct raw_jnl_header * const h);

#endif
