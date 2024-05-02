#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <vector>
#include <cstring>

#include "jnl.h"

/* ジャーナルヘッダ */
/*------------------------------------------------------------*/
size_t Jnl::header::len() const
{
  auto str = std::string{dataLen, Jnl::HdrLen::DATA};
  return std::stoi(str);
}

Jnl::header Jnl::header::read(std::istream& is)
{
  using namespace Jnl::HdrLen;
  header h;

  is.read(h.year, YEAR);
  is.read(h.month, MONTH);
  is.read(h.day, DAY);
  is.read(h.hour, HOUR);
  is.read(h.minute, MINUTE);
  is.read(h.second, SECOND);
  is.read(h.msecond, MSECOND);
  is.read(h.kind, KIND);
  is.read(h.dataLen, DATA);

  return h;
}

Jnl::header* Jnl::header::read(char *buffer, std::istream& is)
{
  using namespace Jnl::HdrLen;
  is.read(buffer, sizeof(header));

  return (header *)buffer;
}

std::ostream& Jnl::header::write(std::ostream& os)
{
  using namespace Jnl::HdrLen;

  os.write(year, YEAR) << "/";
  os.write(month, MONTH) << "/";
  os.write(day, DAY);
  os << ",";
  os.write(hour, HOUR) << ":";
  os.write(minute, MINUTE) << ":";
  os.write(second, SECOND) << ".";
  os.write(msecond, MSECOND);
  os << ",";
  os.write(kind, KIND);
  os << ",";
  os.write(dataLen, DATA);

  return os;
}

/* ジャーナルレコード */
/*------------------------------------------------------------*/
#if 0
Jnl::record* Jnl::record::read(char *buffer, std::istream& is)
{
  Jnl::record* pJnlRecord = (Jnl::record *)buffer;
  pJnlRecord->h.read(buffer, is);

  int dataLen = pJnlRecord->h.len();
  is.read(pJnlRecord->data, dataLen);

  return pJnlRecord;
}
#endif

Jnl::record& Jnl::record::read(char *buffer, std::istream& is)
{
  Jnl::record* pJnlRecord = (Jnl::record *)buffer;
  pJnlRecord->h.read(buffer, is);

  int dataLen = pJnlRecord->h.len();
  is.read(pJnlRecord->data, dataLen);

  return *pJnlRecord;
}

void Jnl::record::dumpData(std::ostream& os = std::cout)
{
  for(size_t i = 0; i < h.len(); ++i)
  {
    if(isprint(data[i]))
    {
      os << data[i];
    }
    else
    {
      os << '.';
    }
  }
}

std::ostream& Jnl::record::write(std::ostream& os = std::cout)
{
  os << "HEADER,";
  h.write(os);
  os << std::endl;

  os << "DATA,";
  dumpData(os);
  os << std::endl;

  return os;
}

void Jnl::record::proc(std::istream& is, std::ostream& os)
{
  std::vector<char> buffer = std::vector<char>(20 * 1024 * 1024);

  while(1)
  {
    record& jnlRecord = read(buffer.data(), is);
    if(is.eof())
    {
      break;
    }

    jnlRecord.write();
  }
}

int main(int argc, char *argv[])
{
  if(argc == 1)
  {
    Jnl::record::proc(std::cin, std::cout);
  }
  else if(argc == 2 && 0 == strcmp("-h", argv[1]))
  {
    std::cerr << "Usage : " << argv[0] << "ファイル名" << std::endl;
  }
  else
  {
    for(int i = 1; i < argc; ++i)
    {
      std::ifstream ifs{argv[i]};
      Jnl::record::proc(ifs, std::cout);
    }
  }

  return 0;
}
