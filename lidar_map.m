close all; clear all;
%name_csv = 'map_data_compressed'
%name_csv = 'map_15_compressed_1'
%name_csv = 'map_data3_compressed'
name_csv = '1234_compressed'
%name_csv = 'map_log_27_compressed'
%name_csv = '9_11_rotation'

%min_d_centi = 0.5; 
%min_d_centi = 5; 
min_d_centi = 10; 
%min_d_centi = 15; 
%min_d_centi = 20; 
len_yaw_meter = 0.3
min_d_meter = min_d_centi / 100.0 
len_q_side_4_start_turning = min([4 round(50 / min_d_centi)])
len_q_side_4_stop_turning = round(40 / min_d_centi)
len_q_front_4_stop_turning = len_q_side_4_stop_turning;
i_x_drone = 2;  i_y_drone = 3;  
FRONT = 0;  LEFT = -1;  RIGHT = 1;
offset_yaw_deg = 0


if (strcmp('map_data_compressed', name_csv))
  margin_from = 139;  margin_end = 60;
  i_yaw = 0; i_front = 4;  i_left = 5; i_right = 6;
  meter_tunnel = 3;
  meter_2_front_wall = 3.5;
  min_valid_meter = 3;
elseif (strcmp('map_15_compressed_1', name_csv))
  meter_tunnel = 3;
  meter_2_front_wall = 1.9;
  margin_from = 28;  margin_end = 120;
  i_yaw = 0;  
  %i_yaw = 4;  
  i_front = 5;  i_right = 6; i_left = 7;
  min_valid_meter = 3;
  meter_hallway = 4;

elseif (strcmp('map_data3_compressed', name_csv))
  meter_tunnel = 2;
  margin_from = 1;  margin_end = 0;
  i_yaw = 0;  
  %i_yaw = 4;  
  i_front = 5;  i_right = 6; i_left = 7;
  min_valid_meter = 2;
  meter_2_front_wall = 1.7;
  meter_hallway = 4;

elseif (strcmp('1234_compressed', name_csv))
  meter_tunnel = 3;
  margin_from = 2;  margin_end = 0;
  %i_yaw = 0;  
  i_yaw = 4;  
  i_front = 5;  i_right = 7; i_left = 6;
  min_valid_meter = 3;
  %meter_2_front_wall = 1.7;
  meter_2_front_wall = 2.0;
  meter_hallway = 4;
elseif (strcmp('map_log_27_compressed', name_csv))
  meter_tunnel = 3;
  margin_from = 380;  margin_end = 0;
  %i_yaw = 0;  
  i_yaw = 4;  
  i_front = 5;  i_right = 6; i_left = 7;
  %offset_yaw_deg = 150
  min_valid_meter = 3;
  %meter_2_front_wall = 1.7;
  meter_2_front_wall = 2.2;
  meter_hallway = 3.5;

elseif (strcmp('9_11_rotation', name_csv))
  meter_tunnel = 3;
  margin_from = 150;  margin_end = 0;
  %i_yaw = 0;  
  i_yaw = 4;  
  i_front = 5;  i_right = 7; i_left = 6;
  min_valid_meter = 3;
  %meter_2_front_wall = 1.7;
  meter_2_front_wall = 2.0;
  meter_hallway = 4;

endif
state = 'straight'
%half_tunnel = 0.8 * meter_tunnel
half_tunnel = 0.85 * meter_tunnel
%half_tunnel = 0.9 * meter_tunnel
half_half_tunnel = 0.1 * meter_tunnel
rad_45 = (pi / 180.0) * 45.0; rad_90 = (pi / 180.0) * 90.0;
path_csv = sprintf('D:\\downloads\\Telegram Desktop\\%s.csv', name_csv)
mat_ms_dx_dy_f_l_r = csvread(path_csv);
mat_ms_dx_dy_f_l_r_trimmed = mat_ms_dx_dy_f_l_r(margin_from:end - margin_end, :);
i_pre = 1;  
x_pre = mat_ms_dx_dy_f_l_r_trimmed(i_pre, i_x_drone);
y_pre = mat_ms_dx_dy_f_l_r_trimmed(i_pre, i_y_drone);
idx_valid = [i_pre];
li_d_side = [];  li_d_front = [];
li_d_left = []; li_d_right = [];
li_direction = [0];
li_state = [state]
is_hallway_found_4_side = false;
is_hallway_found_4_front = false;
is_side_found = false;
dimple_direction = FRONT;
offset_yaw_rad = (pi / 180.0) * offset_yaw_deg;

for i_cur = 2 : length(mat_ms_dx_dy_f_l_r_trimmed) 
  x_cur = mat_ms_dx_dy_f_l_r_trimmed(i_cur, i_x_drone); 
  y_cur = mat_ms_dx_dy_f_l_r_trimmed(i_cur, i_y_drone);
  x_dif = x_cur - x_pre;  y_dif = y_cur - y_pre;
  d_meter = sqrt(x_dif * x_dif + y_dif * y_dif); 
  if d_meter > min_d_meter  
    %{
    if 800 < i_cur
      a = 0;
    endif
    %}
    hold on;  axis equal
    plot([x_pre; x_cur], [y_pre; y_cur], '.-r')    
    idx_valid = [idx_valid; i_cur];
    %if length(idx_valid) >= 2
    d_left = mat_ms_dx_dy_f_l_r_trimmed(i_cur, i_left); d_left = min([d_left 8])
    d_right = mat_ms_dx_dy_f_l_r_trimmed(i_cur, i_right); d_right = min([d_right 8])
    d_front = mat_ms_dx_dy_f_l_r_trimmed(i_cur, i_front); d_front = min([d_front 8])

    ang_rad = mat_ms_dx_dy_f_l_r_trimmed(i_cur, i_yaw) + offset_yaw_rad; 
    ang_rad_left = ang_rad + rad_45; 
    ang_rad_right = ang_rad - rad_45;

    x_front = x_cur + cos(ang_rad) .* d_front;
    y_front = y_cur + sin(ang_rad) .* d_front;
    x_left = x_cur + cos(ang_rad_left) .* d_left;
    y_left = y_cur + sin(ang_rad_left) .* d_left;
    x_right = x_cur + cos(ang_rad_right) .* d_right;
    y_right = y_cur + sin(ang_rad_right) .* d_right;
    
    plot([x_cur; x_left], [y_cur; y_left], '--k') 
    plot(x_left, y_left, 'xg')
    plot([x_cur; x_right], [y_cur; y_right], '--r')
    plot(x_right, y_right, 'om')

    plot([x_cur; x_front], [y_cur; y_front], '--c')
    plot(x_front, y_front, 'db')
  
    if strcmp('turning', state)
      is_turn_finished = false;
      li_d_front = push_queue(li_d_front, d_front, len_q_front_4_stop_turning); 
      if shall_left
        d_side = d_left
      else
        d_side = d_right
      endif
      li_d_side = push_queue(li_d_side, d_side, len_q_side_4_stop_turning);       
      if ~is_hallway_found_4_side
        is_hallway_found_4_side = check_hallway(is_hallway_found_4_side, li_d_side, len_q_side_4_stop_turning, meter_hallway);          
      endif
      if is_hallway_found_4_side
        if ~is_hallway_found_4_front          
          is_hallway_found_4_front = check_hallway(is_hallway_found_4_front, li_d_front, len_q_front_4_stop_turning, meter_hallway);          
        endif          
        if ~is_side_found          
          is_side_found = check_hallway(is_side_found, li_d_side, len_q_side_4_stop_turning, half_tunnel);                               
        endif
      endif
      is_turn_finished = is_hallway_found_4_side & is_hallway_found_4_front & is_side_found;
      if is_turn_finished
        state = 'straight'
        li_direction = [li_direction; 0];
        li_d_side = []; li_d_front = [];
        li_d_left = [d_left]; li_d_right = [d_right];
        dimple_direction = FRONT;
      else          
        if shall_left
          li_direction = [li_direction; -2];
        else   
          li_direction = [li_direction; 2];
        endif
      endif        
    elseif strcmp('straight', state)
      is_end_of_hallway = false;
      li_d_left = push_queue(li_d_left, d_left, len_q_side_4_start_turning);
      li_d_right = push_queue(li_d_right, d_right, len_q_side_4_start_turning);
      if length(li_d_left) >= len_q_side_4_start_turning
        dimple_direction_cur = check_dimple(li_d_left, li_d_right, meter_tunnel);
        if FRONT ~= dimple_direction_cur
          d_jump_l = calc_distance_jump(li_d_left);
          d_jump_r = calc_distance_jump(li_d_right);
          a = 0;
        endif        
        if FRONT == dimple_direction | FRONT ~= dimple_direction_cur
          dimple_direction = dimple_direction_cur;
        endif
      endif               
      is_something_blocking = d_front < meter_2_front_wall
      was_any_dimple = dimple_direction ~= FRONT
      is_end_of_hallway = is_something_blocking & was_any_dimple
      if is_end_of_hallway
      %if d_front < meter_2_front_wall
        state = 'turning'
        is_hallway_found_4_side = true;
        is_hallway_found_4_front = false;
        is_side_found = true;
%{  
        avg_left = mean(li_d_left); avg_right = mean(li_d_right);
        shall_left = avg_left > avg_right           
%}
        shall_left = LEFT == dimple_direction         
        if shall_left
          d_side = d_left
          li_direction = [li_direction; -2];
        else   
          d_side = d_right
          li_direction = [li_direction; 2];
        endif
        li_d_side = [d_side]
        li_d_front = [d_front]
      else
        if d_left > d_right + half_half_tunnel
          li_direction = [li_direction; -1];
        elseif d_right > d_left + half_half_tunnel
          li_direction = [li_direction; 1];
        else
          li_direction = [li_direction; 0];
        endif
      endif
    %endif  
    endif 
  
    if abs(li_direction(end)) <= 1
      plot(x_cur, y_cur, '*k')
    elseif li_direction(end) < 0
      plot(x_cur, y_cur, '*r')
    else
      plot(x_cur, y_cur, '*g')    
    endif    
    pause(0.01);
    i_pre = i_cur;  x_pre = x_cur;  y_pre = y_cur; li_state = [li_state; state];
  endif
endfor  
mat_ms_dx_dy_f_l_r_selected = mat_ms_dx_dy_f_l_r_trimmed(idx_valid, :);
%x_dif = mat_ms_dx_dy_f_l_r_selected(3:end, i_x_drone) - mat_ms_dx_dy_f_l_r_selected(1:end - 2, i_x_drone); 
%y_dif = -mat_ms_dx_dy_f_l_r_selected(3:end, i_y_drone) + mat_ms_dx_dy_f_l_r_selected(1:end - 2, i_y_drone);
x_drone = mat_ms_dx_dy_f_l_r_selected(:, i_x_drone);
y_drone = mat_ms_dx_dy_f_l_r_selected(:, i_y_drone);
d_front = mat_ms_dx_dy_f_l_r_selected(:, i_front);
d_left = mat_ms_dx_dy_f_l_r_selected(:, i_left);
d_right = mat_ms_dx_dy_f_l_r_selected(:, i_right);
  
ang_rad = mat_ms_dx_dy_f_l_r_selected(:, i_yaw) + offset_yaw_rad; 
%ang_rad = atan2(y_dif, x_dif); 
ang_deg = (180.0 / pi) * ang_rad; 
ang_rad_left = ang_rad + rad_45; 
ang_rad_right = ang_rad - rad_45;


x_front = x_drone + cos(ang_rad) .* d_front;
y_front = y_drone + sin(ang_rad) .* d_front;
x_left = x_drone + cos(ang_rad_left) .* d_left;
y_left = y_drone + sin(ang_rad_left) .* d_left;
x_right = x_drone + cos(ang_rad_right) .* d_right;
y_right = y_drone + sin(ang_rad_right) .* d_right;

for iP = 1 : length(x_drone)
  hold on;  axis equal
  if iP > 1
    plot(x_drone(iP - 1 : iP), y_drone(iP - 1 : iP), '.-r')
  end
%%{
  if d_left(iP) < min_valid_meter
    plot([x_drone(iP); x_left(iP)], [y_drone(iP); y_left(iP)], '--k') 
    plot(x_left(iP), y_left(iP), 'xg')
  end
  if d_right(iP) < min_valid_meter
    plot([x_drone(iP); x_right(iP)], [y_drone(iP); y_right(iP)], '--r')
    plot(x_right(iP), y_right(iP), 'om')
  end    
  %if d_front(iP) < min_valid_meter 
  plot([x_drone(iP); x_front(iP)], [y_drone(iP); y_front(iP)], '--c')
  plot(x_front(iP), y_front(iP), 'db')
  %end
  %t1 = li_state(iP, :);
  %if strcmp('straight', li_state(iP, :))
  if abs(li_direction(iP)) <= 1
    plot(x_drone(iP), y_drone(iP), '*k')
  elseif li_direction(iP) < 0
    plot(x_drone(iP), y_drone(iP), '*y')
  else
    plot(x_drone(iP), y_drone(iP), '*g')    
  endif    
%%}    
  %hold on;  plot(x_left, y_left, 'xy')
  %hold on;  plot(x_right, y_right, 'om')
  %pause(0.5);
  %pause(1.5);
  if iP < 12 | iP > 30
    pause(0.1);
  else
    pause(0.1);
  endif    
endfor
%%}





