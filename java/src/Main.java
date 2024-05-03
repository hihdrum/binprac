import java.io.IOException;
import java.io.FileDescriptor;
import java.io.InputStream;
import java.io.FileInputStream;

import jnl.JnlFile;

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
