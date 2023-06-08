/*
 Name:		BasicExample.ino
 Created:	6/6/2023 10:34:16 PM
 Author:	jiaji
*/

#include "TaskQueue.h"
#include <iostream>

TaskQueue<> MyTasks;

// the setup function runs once when you press reset or power the board
void setup()
{
	Serial.begin(115200);

	std::cout << "Setup() running on core " << xPortGetCoreID() << '\n';
	std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < 5; i++)
	{
		std::cout << "Adding task " << i << '\n';
		MyTasks.push([=]
			{
				std::cout << "Task" << i << " starting on core " << xPortGetCoreID() << '\n';
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				std::cout << "Task " << i << " done" << '\n';
			});
		std::cout << "Added task " << i << '\n';
	}

	std::cout << "Setup END, time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << '\n';
}

// the loop function runs over and over again until power down or reset
void loop() {

}
