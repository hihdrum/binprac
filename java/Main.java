import java.io.IOException;
import java.io.FileInputStream;

class Main
{
  public static void main(String[] args)
  {
    final int buffer_size = 20 * 1024 * 1024;
    byte[] buffer = new byte[buffer_size];

    try(FileInputStream in = new FileInputStream(args[0]))
    {
      System.out.println("ファイル(" + args[0] + ")を開きました。");

      in.read(buffer, 0, 20);
    }
    catch(IOException e)
    {
      e.printStackTrace();
    }
  }
}
