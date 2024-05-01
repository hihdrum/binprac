#include <iostream>
#include <string>
#include <fstream>
#include <cctype>

void asciiDump(char *pc, int len);

const int JNL_YEAR_LEN = 4;
const int JNL_MONTH_LEN = 2;
const int JNL_DAY_LEN = 2;

const int JNL_HOUR_LEN = 2;
const int JNL_MINUTE_LEN = 2;
const int JNL_SECOND_LEN = 2;
const int JNL_MSECOND_LEN = 3;

const int JNL_KIND_LEN = 1;
const int JNL_DATA_LEN = 8;

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

  friend std::istream& operator>>(std::istream& is, jnl_header& h)
  {
    is.read(h.year, JNL_YEAR_LEN);
    is.read(h.month, JNL_MONTH_LEN);
    is.read(h.day, JNL_DAY_LEN);
    is.read(h.hour, JNL_HOUR_LEN);
    is.read(h.minute, JNL_MINUTE_LEN);
    is.read(h.second, JNL_SECOND_LEN);
    is.read(h.msecond, JNL_MSECOND_LEN);
    is.read(h.kind, JNL_KIND_LEN);
    is.read(h.dataLen, JNL_DATA_LEN);

    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, const jnl_header& h)
  {
    os.write(h.year, JNL_YEAR_LEN) << "/";
    os.write(h.month, JNL_MONTH_LEN) << "/";
    os.write(h.day, JNL_DAY_LEN);
    os << ",";
    os.write(h.hour, JNL_HOUR_LEN) << ":";
    os.write(h.minute, JNL_MINUTE_LEN) << ":";
    os.write(h.second, JNL_SECOND_LEN) << ".";
    os.write(h.msecond, JNL_MSECOND_LEN);
    os << ",";
    os.write(h.kind, JNL_KIND_LEN);
    os << ",";
    os.write(h.dataLen, JNL_DATA_LEN);

    return os;
  }

  void print();
  size_t dataLength();

} __attribute__((packed));

void jnl_header::print()
{
  std::printf("%*.*s/%*.*s/%*.*s,%*.*s:%*.*s:%*.*s.%*.*s,%*.*s,%*.*s",
      JNL_YEAR_LEN, JNL_YEAR_LEN, year,
      JNL_MONTH_LEN, JNL_MONTH_LEN, month,
      JNL_DAY_LEN, JNL_DAY_LEN, day,
      JNL_HOUR_LEN, JNL_HOUR_LEN, hour,
      JNL_MINUTE_LEN, JNL_MINUTE_LEN, minute,
      JNL_SECOND_LEN, JNL_SECOND_LEN, second,
      JNL_MSECOND_LEN, JNL_MSECOND_LEN, msecond,
      JNL_KIND_LEN, JNL_KIND_LEN, kind,
      JNL_DATA_LEN, JNL_DATA_LEN, dataLen);
}

size_t jnl_header::dataLength()
{
  auto str = std::string{dataLen, JNL_DATA_LEN};
  return std::stoi(str);
}

#if 0
const int dataBufferSize = 20 * 1024 * 1024;
char *dataBuffer;

void dumpStream(FILE *fp)
{
  while(1)
  {
    struct jnl_header jnlh;
    size_t retFread = std::fread(&jnlh, sizeof(jnlh), 1, fp);
    if(1 != retFread)
    {
      if(std::feof(fp))
      {
        break;
      }

      std::perror("ジャーナルヘッダfread異常");
      std::exit(1);
    }

    std::printf("HEADER:");
    jnlh.print();
    std::putchar('\n');

    size_t dataLen = jnlh.dataLength();

    retFread = std::fread(dataBuffer, sizeof(char), dataLen, fp);
    if(retFread < dataLen)
    {
      std::perror("データfread異常");
      std::exit(1);
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
    FILE *fp = std::fopen(argv[i], "r");
    if(NULL == fp)
    {
      std::perror("fopen異常");
      std::exit(1);
    }

    dumpStream(fp);

    int retFclose = fclose(fp);
    if(0 != retFclose)
    {
      std::perror("fclose異常");
      std::exit(1);
    }
  }
}

/* 1文字毎の出力関数呼び出し */
void asciiDump(char *pc, int len)
{
  for(int i = 0; i < len; i++)
  {
    if(std::isprint(*pc))
    {
      std::putchar(*pc);
    }
    else
    {
      std::putchar('.');
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
    std::exit(1);
  }

  dataBuffer = (char *)std::malloc(dataBufferSize);
  if(NULL == dataBuffer)
  {
    std::perror("malloc異常");
    std::exit(1);
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
#endif

/* 1文字毎の出力関数呼び出し */
void asciiDump(char *pc, int len)
{
  for(int i = 0; i < len; i++)
  {
    if(std::isprint(*pc))
    {
      std::putchar(*pc);
    }
    else
    {
      std::putchar('.');
    }
  }
}

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    std::cerr << "Usage : " << argv[0] << "ファイル名" << std::endl;
    std::exit(1);
  }

  std::ifstream ifs{argv[1], std::ios_base::in};
  if(!ifs)
  {
    std::perror("ファイルオープン異常");
    std::exit(1);
  }

  struct jnl_header jnlh;

  while(1)
  {
    ifs >> jnlh;
    if(ifs.eof())
    {
      break;
    }

    std::cout << "HEADER" << "," << jnlh << std::endl;

    std::streamsize dataLen = jnlh.dataLength();
    constexpr std::streamsize buffer_size = 20 * 1024 * 1024;
    auto *buffer = new char[buffer_size];

    ifs.read(buffer, dataLen);
    if(ifs.gcount() < dataLen)
    {
      std::cerr << "read異常" << std::endl;
      std::exit(1);
    }

    std::cout << "DATA:";
    asciiDump(buffer, dataLen);
    std::cout << std::endl;

  }


  return 0;
}
