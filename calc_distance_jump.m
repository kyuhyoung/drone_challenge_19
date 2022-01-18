function d_jump = calc_distance_jump(li_d)
  idx_half = round(0.5 * length(li_d))
  avg_front = mean(li_d(1 : idx_half))
  avg_rear = mean(li_d(idx_half + 1 : end))
  d_jump = avg_rear - avg_front;  
endfunction

  