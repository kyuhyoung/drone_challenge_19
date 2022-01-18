function direc = check_dimple(li_d_left, li_d_right, d_thres)
  d_jump_l = calc_distance_jump(li_d_left);
  d_jump_r = calc_distance_jump(li_d_right);
  
  if d_jump_l > d_thres
    direc = -1;
  elseif d_jump_r > d_thres
    direc = 1;
  else
    direc = 0;
  endif  
endfunction 
