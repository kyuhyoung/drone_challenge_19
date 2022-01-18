#include "customqueue.h"

// Constructor to initialize queue
queue::queue(int size)
{
	arr = new float[size];
	capacity = size;
	phront = 0;
	rear = -1;
	count = 0;
}
queue::~queue()
{
	delete arr;
}

// Utility function to remove front element from the queue

// Utility function to remove front element from the queue
void queue::pop()
{
	// check for queue underflow
	if (empty())
	{
		cout << "UnderFlow\nProgram Terminated\n";
		exit(EXIT_FAILURE);
	}

	//cout << "Removing " << arr[phront] << '\n';

	phront = (phront + 1) % capacity;
	count--;
}

void queue::emptyAll()
{
	while(!empty())
	{
		pop();
	}
	return;
}

// Utility function to add an item to the queue
void queue::push(float item)
{
	/*
	// check for queue overflow
	if (isFull())
	{
		cout << "OverFlow\nProgram Terminated\n";
		exit(EXIT_FAILURE);
	}
	*/
	while(isFull())
	{
		pop();
	}
	//cout << "Inserting " << item << '\n';
	rear = (rear + 1) % capacity;
	arr[rear] = item;
	count++;
}

// Utility function to return front element in the queue
float queue::front()
{
	if (empty())
	{
		cout << "UnderFlow\nProgram Terminated\n";
		exit(EXIT_FAILURE);
	}
	return arr[phront];
}

// Utility function to return the size of the queue
int queue::size()
{
	return count;
}

// Utility function to check if the queue is empty or not
bool queue::empty()
{
	return (size() == 0);
}

// Utility function to check if the queue is full or not
bool queue::isFull()
{
	return (size() == capacity);
}

void queue::printAll()
{
	//cout << "count : " << count << endl;	exit(0);
	for(int ii = 0; ii < count; ii++)
	{
		int idx = (phront + ii) % capacity;
		cout << arr[idx] << " ";
	}
	cout << endl;
}

float queue::seom(int i_from, int i_to)
{
	float ssum = 0;
	for(int ii = i_from; ii <= i_to; ii++)
	{
		int idx = (phront + ii) % capacity;
		ssum += arr[idx];
	}
	return ssum;
}


float queue::seom()
{
	return seom(0, count - 1); 
}


float queue::average(int i_from, int i_to)
{
	return seom(i_from, i_to) / (float)(i_to - i_from + 1);	
}

float queue::average()
{
	return seom() / float(size());
}

#if 0
// main function
int main()
{
	// create a queue of capacity 5
	queue q(5);

	q.enqueue(1);
	q.enqueue(2);
	q.enqueue(3);
	
	cout << "Front element is: " << q.peek() << endl;
	q.dequeue();
	
	q.enqueue(4);

	cout << "Queue size is " << q.size() << endl;

	q.dequeue();
	q.dequeue();
	q.dequeue();
	
	if (q.empty())
		cout << "Queue Is Empty\n";
	else
		cout << "Queue Is Not Empty\n";

	return 0;
}
#endif
