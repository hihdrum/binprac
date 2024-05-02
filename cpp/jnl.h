#pragma once

#include <iostream>
#include <vector>

/* ジャーナルヘッダメンババイト長 */
/*------------------------------------------------------------*/
namespace Jnl::HdrLen
{
  const int YEAR = 4;
  const int MONTH = 2;
  const int DAY = 2;

  const int HOUR = 2;
  const int MINUTE = 2;
  const int SECOND = 2;
  const int MSECOND = 3;

  const int KIND = 1;
  const int DATA = 8;
}

namespace Jnl
{
  /* ジャーナルヘッダ */
  /*------------------------------------------------------------*/
  struct header
  {
    char year[HdrLen::YEAR];
    char month[HdrLen::MONTH];
    char day[HdrLen::DAY];

    char hour[HdrLen::HOUR];
    char minute[HdrLen::MINUTE];
    char second[HdrLen::SECOND];
    char msecond[HdrLen::MSECOND];

    char kind[HdrLen::KIND];
    char dataLen[HdrLen::DATA];

    /* 関数 */
    static header read(std::istream& is);
    static header* read(char *buffer, std::istream& is);
    std::ostream& write(std::ostream& os);

    size_t len() const;

  } __attribute__((packed));
}

namespace Jnl
{
  /* ジャーナルレコード */
  /*------------------------------------------------------------*/
  struct record
  {
    header h;
    char data[0];

    //static record* read(char *buffer, std::istream& is);
    static record& read(char *buffer, std::istream& is);
    static void proc(std::vector<char>& buffer, std::istream& is, std::ostream& os);

    void dumpData(std::ostream& os);
    std::ostream& write(std::ostream& os);

  } __attribute__((packed));
}

