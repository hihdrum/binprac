-module(jnl).
-compile(export_all).

hello() -> "Hello, world!".

sample_file() -> "../make_dummy_data/data000".

read_open(FileName) -> file:open(FileName, [read, binary, raw]).

sample_read_open() -> read_open(sample_file()).

read_header(IoDevice) -> file:read(IoDevice, 26).

parse_header(<<Year:4/binary, Month:2/binary, Rest/binary>>) ->
  {{year, Year}, {month, Month}, {rest, Rest}}.
