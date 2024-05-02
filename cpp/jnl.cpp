#include <fstream>
#include <vector>

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

void Jnl::record::proc(std::vector<char>& buffer, std::istream& is, std::ostream& os)
{
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
