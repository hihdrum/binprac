package jnl;

import java.io.IOException;
import java.io.InputStream;

public class File
{
  public static void asciiDump(InputStream in) throws IOException
  {
    while(true)
    {
      Record Record = jnl.Record.read(in);
      Record.toDisplayableData();
      Record.printRecord();
      if(0 == in.available())
      {
        break;
      }
    }
  }
}
