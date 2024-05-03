import java.io.IOException;
import java.io.InputStream;

class JnlHeader
{
  final static int SIZE = 26;

  final private byte[] h;

  JnlHeader(final byte[] h)
  {
    this.h = h;
  }

  JnlHeader(InputStream in) throws IOException
  {
    byte[] buffer = new byte[SIZE];
    int readByte = in.readNBytes(buffer, 0, SIZE);

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
