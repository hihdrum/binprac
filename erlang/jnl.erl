-module(jnl).
-compile(export_all).
-include("jnl.hrl").

hello() -> "Hello, world!".

sample_file() -> "../make_dummy_data/data000".
sample_read_open() -> read_open(sample_file()).

% 読込みオープン
read_open(FileName) -> file:open(FileName, [read, binary, raw]).

% ジャーナルヘッダ
%------------------------------------------------------------
% ヘッダの読込み
% (読込み元) -> ジャーナルヘッダ バイト列
read_header(IoDevice) -> file:read(IoDevice, 26).

% ジャーナルヘッダバイト列 -> ジャーナルヘッダ レコード
parse_jnl_header(HeaderBin) when is_binary(HeaderBin) ->

  <<Year:4/binary, Month:2/binary, Day:2/binary,
    Hour:2/binary, Minute:2/binary, Second:2/binary, MSecond:3/binary,
    Kind:1/binary, DataLen:8/binary>> = HeaderBin,

  #jnl_header{
     year = Year, month = Month, day = Day,
     hour = Hour, minute = Minute, second = Second, msecond = MSecond,
     kind = Kind, data_len = DataLen
    }.

% ジャーナルヘッダの書込み
% (書込み先, ジャーナルヘッダ レコード) -> ok
write_jnl_header(IoDevice, HeaderRecord) ->

  #jnl_header{
     year = Year, month = Month, day = Day,
     hour = Hour, minute = Minute, second = Second, msecond = MSecond,
     kind = Kind, data_len = DataLen} = HeaderRecord,

  io:format(IoDevice,
            "HEADER,~s/~s/~s,~s:~s:~s.~s,~s,~s~n",
            [Year, Month, Day, Hour, Minute, Second, MSecond, Kind, DataLen]).

%------------------------------------------------------------
% (ジャーナルヘッダ レコード, 読込み元) ->  {ok, Data} | eof | {error, Reason}
read_data(HeaderRecord, IoDevice) ->

  DataLen = binary_to_integer(HeaderRecord#jnl_header.data_len),
  file:read(IoDevice, DataLen).

% 表示可能文字を判定する関数
% この関数をto_printableで利用したいが、ifで利用しようとした際に以下のエラーとなった。
% call to local/imported function is_print_byte/1 is illegal in guard
is_print(Byte) -> (16#21 =< Byte) and (Byte =< 16#7E).

% 表示可能文字でない場合、'.'を返す関数
to_printable(Byte) ->
  case is_print(Byte) of
    true -> Byte;
    false -> $.
  end.

% ジャーナルデータの書込み
write_jnl_data(IoDevice, DataBin) ->

  DataList = binary_to_list(DataBin),
  PrintableList = lists:map(fun to_printable/1, DataList),

  ok = file:write(IoDevice, "DATA:"),
  file:write(IoDevice, PrintableList).

% ジャーナルレコード
%------------------------------------------------------------
% (読込み元) -> ジャーナルレコード
read_record(IoDevice) ->

  % ジャーナルヘッダの読込み
  {ok, HeaderBin} = read_header(IoDevice),
  HeaderRecord = parse_jnl_header(HeaderBin),

  % データ部の読込み
  {ok, DataBin} = read_data(HeaderRecord, IoDevice),

  #jnl_record{ header = HeaderRecord, data = DataBin}.

% ジャーナルレコードの書込み
write_jnl_record(IoDevice, JnlRecord) ->

  ok = write_jnl_header(IoDevice, JnlRecord#jnl_record.header),
  write_jnl_data(IoDevice, JnlRecord#jnl_record.data).

%------------------------------------------------------------
sample_run() ->
  SampleFile = sample_file(),
  {ok, ReadFile} = file:open(SampleFile, [read, binary, raw]),
  {ok, WriteFile} = file:open("a", [write, binary]),

  JnlRecord = read_record(ReadFile),

  ok = write_jnl_record(WriteFile, JnlRecord),

  file:close(ReadFile),
  file:close(WriteFile),
  ok.
