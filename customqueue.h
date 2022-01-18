#ifndef CUSTOM_QUEUE
#define CUSTOM_QUEUE



#include <iostream>
#include <cstdlib>
using namespace std;

// define default capacity of the queue
//#define SIZE 10

// Class for queue
class queue
{
	float *arr;		// array to store queue elements
	int capacity;	// maximum capacity of the queue
	int phront;		// front points to front element in the queue (if any)
	int rear;		// rear points to last element in the queue
	int count;		// current size of the queue

public:
	queue(int size);		// constructor
	~queue();		// constructor
	void pop();
	void push(float x);
	float front();
	int size();
	bool empty();
	void emptyAll();
	bool isFull();
	float seom();
	float seom(int i_from, int i_to);
	float average();
	float average(int i_from, int i_to);
	void printAll();
};


#endif //	CUSTOM_QUEUE
