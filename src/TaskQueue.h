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

template<typename TaskType = std::function<void()>>
class TaskQueue
{
public:
	TaskQueue();
	~TaskQueue();

	void push(TaskType task);

private:
	std::queue<TaskType> m_tasks;
	std::condition_variable cv;
	std::mutex mutex;
	std::thread worker;
	bool done = false;
};

#include <TaskQueueImpl.hpp>

#endif
