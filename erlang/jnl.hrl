-record(jnl_header, {
          year,
          month,
          day,
          hour,
          minute,
          second,
          msecond,
          kind,
          data_len
         }).

-record(jnl_record, {
          header,
          data
         }).
