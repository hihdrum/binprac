package jnl;

import java.io.IOException;
import java.io.InputStream;

class Record
{
  final Header h;
  final byte[] data;

  private Record(final Header h, final byte[] data)
  {
    this.h = h;
    this.data = data;
  }

  public static Record read(InputStream in) throws IOException
  {
    final Header h = Header.read(in);
    final byte[] data = new byte[h.dataLen()];

    int readByte = in.readNBytes(data, 0, h.dataLen());
    if(h.dataLen() != readByte)
    {
      throw new IOException("読込み長が足りませんでした。");
    }

    return new Record(h, data);
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

  public byte[] toDisplayableData()
  {
    for(int i = 0; i < h.dataLen(); i++)
    {
      if(false == Ascii.isPrint(data[i]))
      {
        data[i] = '.';
      }
    }

    return data;
  }

  public void printHeader()
  {
    System.out.printf("HEADER,%s,%s,%s,%s\n", h.date(), h.time(), h.kind(), h.data());
  }

  public void printAsciiDump()
  {
    System.out.printf("DATA:");
    System.out.write(data, 0, h.dataLen());
    System.out.println("");
  }

  public void printRecord()
  {
    printHeader();
    printAsciiDump();
  }
}
