/*
 Name:		TaskQueue.h
 Created:	6/6/2023 10:32:21 PM
 Author:	jiaji
 Editor:	http://www.visualmicro.com
*/

#ifndef _TaskQueue_h
#define _TaskQueue_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <functional>

class TaskQueue
{
public:
	using Task = std::function<void()>;

	TaskQueue();
	~TaskQueue();

	void push(Task task);

private:
	std::queue<Task> m_tasks;
	std::condition_variable cv;
	std::mutex mutex;
	std::thread worker;
	bool done = false;
};

#endif

