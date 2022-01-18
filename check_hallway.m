function is_hallway_found_new = check_hallway(is_hallway_found_old, li_d, len_q, meter_hallway)
  is_hallway_found_new = is_hallway_found_old;
  if length(li_d) >= len_q
    avg_d = mean(li_d);
    is_hallway_found_new = avg_d > meter_hallway;
  endif          
endfunction
