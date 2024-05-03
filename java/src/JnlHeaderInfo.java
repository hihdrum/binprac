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
