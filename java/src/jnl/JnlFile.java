package jnl;

import java.io.IOException;
import java.io.InputStream;

public class JnlFile
{
  public static void asciiDump(InputStream in) throws IOException
  {
    final int buffer_size = 20 * 1024 * 1024;
    byte[] buffer = new byte[buffer_size];

    while(true)
    {
      JnlRecord jnlRecord = new JnlRecord(in);
      jnlRecord.toDisplayableData();
      jnlRecord.printRecord();
      if(0 == in.available())
      {
        break;
      }
    }
  }
}
