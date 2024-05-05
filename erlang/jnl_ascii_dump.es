#!/usr/bin/env escript

main(Args) ->
  if
    [] == Args ->
      ok = io:setopts(standard_io, [{binary, true}]),
      jnl:read_write_loop(standard_io, standard_io);

    (length(Args) == 1) and ("-h" == hd(Args)) ->
      io:format(standard_error, "Usage : ファイル名~n", []);

    true -> lists:foreach(fun jnl:proc_jnl_file/1, Args)
  end.
