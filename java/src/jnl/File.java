package jnl;

import java.io.IOException;
import java.io.InputStream;

public class File
{
  public static void asciiDump(InputStream in) throws IOException
  {
    final int buffer_size = 20 * 1024 * 1024;
    byte[] buffer = new byte[buffer_size];

    while(true)
    {
      Record Record = new Record(in);
      Record.toDisplayableData();
      Record.printRecord();
      if(0 == in.available())
      {
        break;
      }
    }
  }
}
