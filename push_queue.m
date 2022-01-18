function q_new = push_queue(q_old, val, capa)
  if length(q_old) >= capa
    q_new = [q_old(2 : end); val]
  else
    q_new = [q_old; val]
  endif 
endfunction