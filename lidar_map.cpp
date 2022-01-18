#include <iostream>
//#include <queue>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "customqueue.h"

using namespace std;

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))  
#define round(x) ((x) >=0 ? (long)((x) + 0.5) :(long)((x) - 0.5))

//#define MIN_D_CENTI 				15
#define MIN_D_CENTI 				10

#define MIN_D_METER 				MIN_D_CENTI / 100.0

#define LEN_Q_SIDE_4_START_TURNING 		MIN(4, round(50.0 / MIN_D_CENTI))
//#define LEN_Q_4_STOP_TURNING 		round(35.0 / MIN_D_CENTI)
#define LEN_Q_SIDE_4_STOP_TURNING 		round(40.0 / MIN_D_CENTI)
#define LEN_Q_FRONT_4_STOP_TURNING 		LEN_Q_SIDE_4_STOP_TURNING

#define METER_HALLWAY				4
#define METER_TUNNEL 				3
#define HALF_TUNNEL 				METER_TUNNEL * 0.8
#define HALF_HALF_TUNNEL 			METER_TUNNEL * 0.1

//#define METER_2_FRONT_WALL 			1.7
//#define METER_2_FRONT_WALL 			1.8
#define METER_2_FRONT_WALL 			2.0

#define N_COLUMN					7

#define METER_4_DIMPLE_COUNT        METER_TUNNEL * 0.35
#define COUNT_THRES					10


enum state_drone
{
	STRAIGHT, 
	TURNING
};

enum directions
{
	TURN_LEFT = -2,
	MOVE_LEFT = -1,
	GO_STRAIGHT = 0,
	MOVE_RIGHT = 1,
	TURN_RIGHT = 2
};

enum dimple
{
	LEFT = -1,
	FRONT = 0,
	RIGHT = 1
};

enum columns
{
	COL_X = 1,
	COL_Y = 2,
	COL_FRONT = 4,
	//COL_RIGHT = 5,
	//COL_LEFT = 6
	COL_RIGHT = 6,
	COL_LEFT = 5
};

queue g_q_d_left(LEN_Q_SIDE_4_START_TURNING);
queue g_q_d_right(LEN_Q_SIDE_4_START_TURNING);
queue g_q_d_side(LEN_Q_SIDE_4_STOP_TURNING);
queue g_q_d_front(LEN_Q_FRONT_4_STOP_TURNING);
state_drone g_state;
dimple g_dimple_direction;

int g_cnt = 0, g_n_valid = 0, g_count_dimple_l = 0, g_count_dimple_r = 0;
float g_x_pre, g_y_pre;
bool g_shall_left = false, g_b_side_found = false, g_b_hallway_found_4_side = false, g_b_hallway_found_4_front = false;


bool check_hallway(queue& q_d, float meter_hallway)
{
	bool is_hallway_found = false;
	if(q_d.isFull())
	{
		float avg_d = q_d.average();
		is_hallway_found = avg_d > meter_hallway;	
	}
	return is_hallway_found;
}

float calc_distance_jump(queue& q_d)
{
	int idx_half = round(0.5 * q_d.size());
	float avg_front = q_d.average(0, idx_half - 1), avg_rear = q_d.average(idx_half, q_d.size() - 1);
	float d_jump = avg_rear - avg_front;
	return d_jump;	
}

float calc_distance_jump_lim(queue& q_d)
{
	//int idx_half = round(0.5 * q_d.size());
	float avg_front = q_d.average(0, 0), avg_rear = q_d.average(q_d.size() - 1, q_d.size() - 1);
	float d_jump = avg_rear - avg_front;
	return d_jump;	
}
/*
float calc_distance_jump_min_max(queue& q_d)
{
	//int idx_half = round(0.5 * q_d.size());
	float avg_front = q_d.average(0, 0), avg_rear = q_d.average(q_d.size() - 1, q_d.size() - 1);
	float d_jump = max_d - avg_rear - avg_front;
	return d_jump;	
}
*/


enum dimple check_dimple(queue& q_d_l, queue& q_d_r, float d_thres, bool shall_debug = false)
{
	dimple direc;

	float d_jump_l = calc_distance_jump(q_d_l), d_jump_r = calc_distance_jump(q_d_r);
	/*
	if(shall_debug) 
	{
		cout << "d_jump_l : " << d_jump_l << endl;	cout << "d_jump_r : " << d_jump_r << endl;	exit(0);
	}	
	*/
	if(d_jump_l > d_thres)
	{
		direc = LEFT;
	}
	else if(d_jump_r > d_thres)
	{
		direc = RIGHT;	
	}
	else
	{
		direc = FRONT;
	}
	return direc;
}



enum directions compute_direction(float x_drone, float y_drone, float d_front, float d_left, float d_right)
{
	directions direc = GO_STRAIGHT; 
	//cout << "g_cnt : " << g_cnt << endl;
	if(0 == g_cnt++)
	{
		g_x_pre = x_drone;	g_y_pre = y_drone;
		cout << "direc : " << direc << endl;	//exit(0);
		return direc; 
	}
	float x_cur = x_drone, y_cur = y_drone;
	float x_dif = x_cur - g_x_pre, y_dif = y_cur - g_y_pre;
	float d_meter = sqrt(x_dif * x_dif + y_dif * y_dif); 
	if(d_meter > MIN_D_METER)
	{
/*	
		cout << "g_x_pre : " << g_x_pre << ", g_y_pre : " << g_y_pre << endl;
		cout << "x_cur : " << x_cur << ", y_cur : " << y_cur << endl;
		cout << "d_meter : " << d_meter << endl;	exit(0);
*/
		//if(++g_n_valid >= 2)
		if(++g_n_valid >= 0)
		{
			//cout << "g_n_valid : " << g_n_valid << endl;	exit(0);
			d_left = MIN(d_left, 8);	d_right = MIN(d_right, 8);	d_front = MIN(d_front, 8);
			//cout << "g_state : " << g_state << endl;	exit(0);
			if(TURNING == g_state)
			{
  				g_q_d_front.push(d_front);
				float d_side = g_shall_left ? d_left : d_right;
				//if(g_q_d_lr.size() >= LEN_Q_4_STOP_TURNING) g_q_d_lr.pop();
				g_q_d_side.push(d_side);
				if(!g_b_hallway_found_4_side)
				{
					g_b_hallway_found_4_side = check_hallway(g_q_d_side, METER_HALLWAY); 	
				}
				if(g_b_hallway_found_4_side)
				{
					if(!g_b_hallway_found_4_front)
					{
						g_b_hallway_found_4_front = check_hallway(g_q_d_front, METER_HALLWAY);
					}
					if(!g_b_side_found) 
					{
						g_b_side_found = check_hallway(g_q_d_side, HALF_TUNNEL);
					}
				}
				
				bool is_turn_finished = g_b_hallway_found_4_side && g_b_hallway_found_4_front && g_b_side_found;
				if(is_turn_finished)
				{
					g_state = STRAIGHT;
  					g_q_d_side.emptyAll();
					cout << "g_q_d_left.printAll() b4 : " << endl;		g_q_d_left.printAll();
					cout << "g_q_d_right.printAll() b4 : " << endl;	g_q_d_right.printAll();

  					g_q_d_left.emptyAll();	g_q_d_left.push(d_left);
  					g_q_d_right.emptyAll();	g_q_d_right.push(d_right);
					cout << "g_q_d_left.printAll() after : " << endl;		g_q_d_left.printAll();
					cout << "g_q_d_right.printAll() after : " << endl;	g_q_d_right.printAll();	exit(0);
					direc = GO_STRAIGHT;
					g_dimple_direction = FRONT;
					g_count_dimple_l = 0;	g_count_dimple_r = 0;
				}	
				else
				{
					direc = g_shall_left ? TURN_LEFT : TURN_RIGHT;
				}
			}
			else
			{
				//cout << "d_left : " << d_left << ", d_right : " << d_right << endl;	exit(0);
				g_q_d_left.push(d_left);	g_q_d_right.push(d_right);
				//cout << "LEN_Q_SIDE_4_START_TURNING  : " << LEN_Q_SIDE_4_START_TURNING << endl;	exit(0);
				if(g_q_d_left.isFull())
				{
					/*
					if(9 == g_n_valid) 
					{
						g_q_d_left.printAll();	g_q_d_right.printAll();	exit(0);
					}	
					*/
					dimple dim_direc = check_dimple(g_q_d_left, g_q_d_right, METER_TUNNEL);
					if(FRONT == g_dimple_direction || FRONT != dim_direc)
					{
						g_dimple_direction = dim_direc;
					}
					if(calc_distance_jump_lim(g_q_d_left) > METER_4_DIMPLE_COUNT) g_count_dimple_l++;
					if(calc_distance_jump_lim(g_q_d_right) > METER_4_DIMPLE_COUNT) g_count_dimple_r++;
				}
				int over_count_l = g_count_dimple_l - COUNT_THRES, over_count_r = g_count_dimple_r - COUNT_THRES; 
				bool any_side_dominate = over_count_l * over_count_r < 0;
				bool is_something_blocking = METER_2_FRONT_WALL > d_front;
				bool was_any_dimple = g_dimple_direction != FRONT || any_side_dominate;
				bool is_end_of_hallway = is_something_blocking && was_any_dimple;
				if(is_end_of_hallway)
				{
					//cout << "d_front : " << d_front << endl;	exit(0);
					g_state = TURNING;
					g_b_hallway_found_4_side = true;
					g_b_side_found = true;
					g_b_hallway_found_4_front = false;
                    /*
					float avg_left = g_q_d_left.average();
					//cout << "avg_left : " << avg_left << endl;	exit(0);
					float avg_right = g_q_d_right.average();
					//cout << "avg_left : " << avg_left << endl;	//exit(0);
					//cout << "avg_right : " << avg_right << endl;	exit(0);
					g_shall_left = avg_left > avg_right;
					*/
					g_shall_left = FRONT == g_dimple_direction ? over_count_l > 0 : LEFT == g_dimple_direction; 
					float d_side;
					if(g_shall_left)
					{
						direc = TURN_LEFT;	d_side = d_left;
					}
					else
					{
						direc = TURN_RIGHT;	d_side = d_right;
					}
					//cout << "g_q_d_lr.size() b4 : " << g_q_d_lr.size() << endl;
  					//while (!g_q_d_lr.empty()) g_q_d_lr.pop();					g_q_d_lr.push(d_lr);
  					g_q_d_side.emptyAll();	g_q_d_side.push(d_side);
					g_q_d_front.emptyAll();	g_q_d_front.push(d_front);	
					//cout << "g_q_d_lr.size() after : " << g_q_d_lr.size() << endl;	exit(0);
				}
				else
				{
					direc = d_left > d_right + HALF_HALF_TUNNEL ? MOVE_LEFT : (d_right > d_left + HALF_HALF_TUNNEL ? MOVE_RIGHT : GO_STRAIGHT);
					//cout << "direc : " << direc << endl;  exit(0);
				}
			}
		}
		g_x_pre = x_cur; g_y_pre = y_cur;
		cout << "direc : " << direc << endl;
	}
	
	return direc;
}


const char* getfield(char* line, int num)
{
    const char* tok;
	    //for (tok = strtok(line, ";");
	    for (tok = strtok(line, ",");
		            tok && *tok;
					            tok = strtok(NULL, ";\n"))
								    {
									        if (!--num)
											            return tok;
														    }
															    return NULL;
																}

int main(int argc, char* argv[])
{
	// create a queue of capacity 5
	queue q(7);

	for(int ii = 0; ii < 20; ii++)
	{
		q.push(ii);
		q.printAll();
		if(q.isFull())
		{
			float t1 = calc_distance_jump(q);
			cout << "ii : " << ii << ", t1 : " << t1 << endl;
		}
	}		
	//exit(0);
	q.push(1);
	q.push(1);
	q.push(3);
	q.push(3);
	q.push(3);
	q.push(3);
	q.printAll();
	cout << "calc_distance_jump(q) : " << calc_distance_jump(q) << endl;	//exit(0);
	cout << "calc_distance_jump(q) : " << calc_distance_jump(q) << endl;

	
	cout << "Front element is: " << q.front() << endl;
	q.pop();
	
	q.push(4);

	cout << "Queue size is " << q.size() << endl;

	q.pop();
	q.pop();
	q.pop();
	
	if (q.empty())
		cout << "Queue Is Empty\n";
	else
		cout << "Queue Is Not Empty\n";




    //FILE* stream = fopen("input", "r");
	cout << "argv[1] : " << argv[1] << endl;
    FILE* stream = fopen(argv[1], "r");
	cout << "stream : " << stream << endl;
	char line[1024];
	int cnt = 0;
	g_state = STRAIGHT;
	g_dimple_direction = FRONT;
	while (fgets(line, 1024, stream))
	{
		if(0 == cnt++) continue;
		char* tmp = strdup(line);
		float val[N_COLUMN];

    	char *pt = strtok (tmp, ",");
		val[0] = atof(pt);
		//cout << "val[0] : " << val[0] << endl;
		for(int idx = 1; idx < N_COLUMN; idx++)
		{
			pt = strtok (NULL, ",");
			val[idx] = atof(pt);
			//cout << "idx : " << idx << ", val[idx] : " << val[idx] << endl;
		}		
		free(tmp);
		directions direc = compute_direction(val[COL_X], val[COL_Y], val[COL_FRONT], val[COL_LEFT], val[COL_RIGHT]);
		//cout << "direc : " << direc << endl;
	}
	cout << "end of days" << endl;
	return 1;
	
}
