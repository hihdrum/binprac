-module(jnl).
-compile(export_all).
-include("jnl.hrl").

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

  io:format("~s/~s/~s,~s:~s:~s.~s,~s,~s",
            [Year, Month, Day, Hour, Minute, Second, MSecond, Kind, DataLen]).

parse_jnl_header(HeaderBin) ->
  <<Year:4/binary, Month:2/binary, Day:2/binary,
    Hour:2/binary, Minute:2/binary, Second:2/binary, MSecond:3/binary,
    Kind:1/binary, DataLen:8/binary>> = HeaderBin,

  #jnl_header{
     year = Year, month = Month, day = Day,
     hour = Hour, minute = Minute, second = Second, msecond = MSecond,
     kind = Kind, data_len = DataLen
    }.

write_jnl_header(IoDevice, HeaderRecord) ->
  #jnl_header{
     year = Year, month = Month, day = Day,
     hour = Hour, minute = Minute, second = Second, msecond = MSecond,
     kind = Kind, data_len = DataLen} = HeaderRecord,

  io:format(IoDevice,
            "HEADER,~s/~s/~s,~s:~s:~s.~s,~s,~s~n",
            [Year, Month, Day, Hour, Minute, Second, MSecond, Kind, DataLen]).

read_data(HeaderRecord, IoDevice) ->
  DataLen = binary_to_integer(HeaderRecord#jnl_header.data_len),
  file:read(IoDevice, DataLen).

% 表示可能文字を判定する関数
% この関数をto_printableで利用したいが、以下のエラーとなった。
% call to local/imported function is_print_byte/1 is illegal in guard
is_print(Byte) -> (16#21 =< Byte) and (Byte =< 16#7E).

% 表示可能文字でない場合、'.'を返す関数
to_printable(Byte) ->
  case is_print(Byte) of
    true -> Byte;
    false -> $.
  end.

% ジャーナルレコードの読込み
read_record(IoDevice) ->
  % ジャーナルレコードの読込み
  {ok, HeaderBin} = read_header(IoDevice),
  HeaderRecord = parse_jnl_header(HeaderBin),
  {ok, DataBin} = read_data(HeaderRecord, IoDevice),
  {HeaderRecord, DataBin}.

write_jnl_data(IoDevice, DataBin) ->
  DataList = binary_to_list(DataBin),
  PrintableList = lists:map(fun to_printable/1, DataList),
  ok = file:write(IoDevice, "DATA:"),
  file:write(IoDevice, PrintableList).

write_jnl_record(IoDevice, HeaderRecord, DataBin) ->
  ok = write_jnl_header(IoDevice, HeaderRecord),
  write_jnl_data(IoDevice, DataBin).

sample_run() ->
  SampleFile = sample_file(),
  {ok, ReadFile} = file:open(SampleFile, [read, binary, raw]),
  {ok, WriteFile} = file:open("a", [write, binary]),

  {HeaderRecord, DataBin} = read_record(ReadFile),

  ok = write_jnl_record(WriteFile, HeaderRecord, DataBin),

  file:close(ReadFile),
  file:close(WriteFile),
  ok.
