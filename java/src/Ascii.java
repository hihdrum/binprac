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
