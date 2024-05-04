-module(jnl).
-compile(export_all).

hello() -> "Hello, world!".

sample_file() -> "../make_dummy_data/data000".

read_open(FileName) -> file:open(FileName, [read, binary, raw]).

sample_read_open() -> read_open(sample_file()).

read_header(IoDevice) -> file:read(IoDevice, 26).

parse_header(<<Year:4/binary, Month:2/binary, Day:2/binary,
               Hour:2/binary, Minute:2/binary, Second:2/binary, MSecond:3/binary,
               Kind:1/binary, DataLen:8/binary>>) ->
  {{year, Year}, {month, Month}, {day, Day},
   {hour, Hour}, {minute, Minute}, {second, Second}, {msecond, MSecond},
   {kind, Kind}, {data_len, DataLen}}.

print_header(Header) ->
  {{year, Year}, {month, Month}, {day, Day},
   {hour, Hour}, {minute, Minute}, {second, Second}, {msecond, MSecond},
   {kind, Kind}, {data_len, DataLen}} = parse_header(Header),

  io:format("~s/~s/~s,~s:~s:~s.~s,~s,~s", [Year, Month, Day, Hour, Minute, Second, MSecond, Kind, DataLen]).

sample_run() ->
  SampleFile = sample_file(),
  {ok, IoDevice} = read_open(SampleFile),
  {ok, HeaderBin} = read_header(IoDevice),
  Header = parse_header(HeaderBin),
  print_header(HeaderBin),
  file:close(IoDevice),
  Header.
