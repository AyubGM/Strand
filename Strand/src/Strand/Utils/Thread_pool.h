#pragma once

#include "Strand/Utils/Blocking_queue.h"

#include <thread>
#include <functional>
#include <mutex>
#include <chrono>

namespace Strand {
	class Thread_pool
	{
	private:
		struct func_t {
			std::function<void()> func;
			bool poison = false;
		};

		using my_time = uint64_t;

		my_time get_time() {
			using namespace std::chrono;
			return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		}

		std::vector<std::thread> m_Pool;
		Blocking_queue<func_t> m_Queue;

		std::mutex m_MutexCoroutine;
		std::vector<                           std::function<bool()>>  m_Coroutines; // main thread tasks
		std::vector<std::tuple<my_time, float, std::function<void()>>> m_Defered;    // ^


	};

}