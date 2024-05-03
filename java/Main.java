import java.io.IOException;
import java.io.FileInputStream;

enum JnlHeaderInfo
{
  YEAR(0, 4),
  MONTH(4, 2),
  DAY(6, 2),

  HOUR(8, 2),
  MINUTE(10, 2),
  SECOND(12, 2),
  MSECOND(14, 3),

  KIND(17, 1),
  DATA(18, 8);

  private final int offset;
  private final int len;

  private JnlHeaderInfo(int offset, int len)
  {
    this.offset = offset;
    this.len = len;
  }

  public int offset()
  {
    return offset;
  }

  public int len()
  {
    return len;
  }
}

class JnlHeader
{
  final static int SIZE = 26;

  final private byte[] h;

  JnlHeader(final byte[] h)
  {
    this.h = h;
  }

  JnlHeader(FileInputStream in) throws IOException
  {
    byte[] buffer = new byte[SIZE];
    int readByte = in.read(buffer);

    if(SIZE != readByte)
    {
      throw new IOException("読込み長が足りませんでした。");
    }

    h = buffer;
  }

  public String year()
  {
    String str = new String(h, JnlHeaderInfo.YEAR.offset(), JnlHeaderInfo.YEAR.len());
    return str;
  }

  public String month()
  {
    String str = new String(h, JnlHeaderInfo.MONTH.offset(), JnlHeaderInfo.MONTH.len());
    return str;
  }

  public String day()
  {
    String str = new String(h, JnlHeaderInfo.DAY.offset(), JnlHeaderInfo.DAY.len());
    return str;
  }

  public String hour()
  {
    String str = new String(h, JnlHeaderInfo.HOUR.offset(), JnlHeaderInfo.HOUR.len());
    return str;
  }

  public String minute()
  {
    String str = new String(h, JnlHeaderInfo.MINUTE.offset(), JnlHeaderInfo.MINUTE.len());
    return str;
  }

  public String second()
  {
    String str = new String(h, JnlHeaderInfo.SECOND.offset(), JnlHeaderInfo.SECOND.len());
    return str;
  }

  public String msecond()
  {
    String str = new String(h, JnlHeaderInfo.MSECOND.offset(), JnlHeaderInfo.MSECOND.len());
    return str;
  }

  public String kind()
  {
    String str = new String(h, JnlHeaderInfo.KIND.offset(), JnlHeaderInfo.KIND.len());
    return str;
  }

  public String data()
  {
    String str = new String(h, JnlHeaderInfo.DATA.offset(), JnlHeaderInfo.DATA.len());
    return str;
  }

  public String date()
  {
    return year() + "/" + month() + "/" + day();
  }

  public String time()
  {
    return hour() + ":" + minute() + ":" + second() + "." + msecond();
  }

  public int dataLen()
  {
    return Integer.parseInt(data());
  }

  void print()
  {
    String str = new String(h, 0, SIZE);
    System.out.printf("%s\n", str);
  }
}

class JnlRecord
{
  JnlHeader h;
  byte[] data;

  public JnlRecord(FileInputStream in) throws IOException
  {
    h = new JnlHeader(in);
    data = new byte[h.dataLen()];

    int readByte = in.read(data);
    if(h.dataLen() != readByte)
    {
      throw new IOException("読込み長が足りませんでした。");
    }
  }

  public void asciiDump()
  {
    for(int i = 0; i < h.dataLen(); i++)
    {
      if(Ascii.isPrint(data[i]))
      {
        System.out.printf("%c", data[i]);
      }
      else
      {
        System.out.print('.');
      }
    }
  }

  public void printHeader()
  {
    System.out.printf("HEADER,%s,%s,%s,%s\n", h.date(), h.time(), h.kind(), h.dataLen());
  }

  public void printAsciiDump()
  {
    System.out.printf("DATA:");
    asciiDump();
    System.out.println("");
  }

  public void printRecord()
  {
    printHeader();
    printAsciiDump();
  }
}

class Ascii
{
  public static boolean isPrint(byte b)
  {
    if(0x00 <= b && b <= 0x1f)
    {
      return false;
    }
    return true;
  }
}

class Main
{
  public static void main(String[] args)
  {

    final int buffer_size = 20 * 1024 * 1024;
    byte[] buffer = new byte[buffer_size];

    try(FileInputStream in = new FileInputStream(args[0]))
    {
      JnlRecord jnlRecord = new JnlRecord(in);
      jnlRecord.printRecord();
    }
    catch(IOException e)
    {
      e.printStackTrace();
    }
  }
}
