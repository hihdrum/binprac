#!/usr/bin/env escript

main(Args) ->
  lists:foreach(fun jnl:proc_jnl_file/1, Args).

