#include "stdafx.h"
#include "ConcurrentTaskQueue.h"
#include <iostream>
#include <assert.h>
using namespace realology;

ConcurrentTaskQueue::ConcurrentTaskQueue()
{
}

ConcurrentTaskQueue::ConcurrentTaskQueue(size_t threadNum,
	const std::string& name)
	: queueCount_(threadNum), queueName_(name), stop_(false)
{
	assert(threadNum > 0);
	for (unsigned int i = 0; i < queueCount_; ++i)
	{
		threads_.emplace_back(
			std::thread(std::bind(&ConcurrentTaskQueue::queueFunc, this, i)));
	}
}
void ConcurrentTaskQueue::runTaskInQueue(const std::function<void()>& task)
{
	std::cout << "copy task into queue" << std::endl;
	std::lock_guard<std::mutex> lock(taskMutex_);
	taskQueue_.push(task);
	taskCond_.notify_one();
}
void ConcurrentTaskQueue::runTaskInQueue(std::function<void()>&& task)
{
	std::cout << "move task into queue" << std::endl;
	std::lock_guard<std::mutex> lock(taskMutex_);
	taskQueue_.push(std::move(task));
	taskCond_.notify_one();
}
void ConcurrentTaskQueue::queueFunc(int queueNum)
{
	char tmpName[32];
	snprintf(tmpName, sizeof(tmpName), "%s%d", queueName_.c_str(), queueNum);

	while (!stop_)
	{
		std::function<void()> r;
		{
			std::unique_lock<std::mutex> lock(taskMutex_);
			while (!stop_ && taskQueue_.size() == 0)
			{
				taskCond_.wait(lock);
			}
			if (taskQueue_.size() > 0)
			{
				std::cout << "got a new task!" << std::endl;
				r = std::move(taskQueue_.front());
				taskQueue_.pop();
			}
			else
				continue;
		}
		r();
	}
}

size_t ConcurrentTaskQueue::getTaskCount()
{
	std::lock_guard<std::mutex> guard(taskMutex_);
	return taskQueue_.size();
}

void ConcurrentTaskQueue::stop()
{
	if (!stop_)
	{
		stop_ = true;
		taskCond_.notify_all();
		for (auto& t : threads_)
			t.join();
	}
}
ConcurrentTaskQueue::~ConcurrentTaskQueue()
{
	stop();
}
