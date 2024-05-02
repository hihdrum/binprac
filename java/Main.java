import java.io.IOException;
import java.io.FileInputStream;

class JnlHeader
{
  final static int SIZE = 26;

  byte[] h;

  JnlHeader(byte[] h)
  {
    this.h = h;
  }

  void print()
  {
    String str = new String(h, 0, SIZE);
    System.out.printf("%s\n", str);
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
      System.out.println("ファイル(" + args[0] + ")を開きました。");

      in.read(buffer, 0, JnlHeader.SIZE);
      JnlHeader jnlHeader = new JnlHeader(buffer);
      jnlHeader.print();
    }
    catch(IOException e)
    {
      e.printStackTrace();
    }
  }
}
