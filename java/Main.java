import java.io.IOException;
import java.io.FileDescriptor;
import java.io.InputStream;
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

class JnlRecord
{
  JnlHeader h;
  byte[] data;

  public JnlRecord(InputStream in) throws IOException
  {
    h = new JnlHeader(in);
    data = new byte[h.dataLen()];

    int readByte = in.readNBytes(data, 0, h.dataLen());
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
    System.out.printf("HEADER,%s,%s,%s,%s\n", h.date(), h.time(), h.kind(), h.data());
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
    if(0x21 <= b && b <=0x7E)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

class JnlFile
{
  public static void asciiDump(InputStream in) throws IOException
  {
    final int buffer_size = 20 * 1024 * 1024;
    byte[] buffer = new byte[buffer_size];

    while(true)
    {
      JnlRecord jnlRecord = new JnlRecord(in);
      jnlRecord.printRecord();
      if(0 == in.available())
      {
        break;
      }
    }
  }
}

class Main
{
  public static void main(String[] args)
  {
    if(args.length == 0)
    {
      try(InputStream in = new FileInputStream(FileDescriptor.in))
      {
        JnlFile.asciiDump(in);
      }
      catch(IOException e)
      {
        e.printStackTrace();
      }
    }
    else if(args.length == 1 && "-h".equals(args[0]))
    {
      System.err.println("Usage : ファイル名");
      System.exit(1);
    }
    else
    {
      for(int i = 0; i < args.length; i++)
      {
        try(InputStream in = new FileInputStream(args[i]))
        {
          JnlFile.asciiDump(in);
        }
        catch(IOException e)
        {
          e.printStackTrace();
        }
      }
    }
  }
}