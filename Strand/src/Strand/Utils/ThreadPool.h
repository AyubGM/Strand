#pragma once

#include "Strand/Utils/BlockingQueue.h"
#include "Strand/Core/Log.h"

#include <thread>
#include <functional>
#include <mutex>
#include <chrono>

namespace Strand {
	class ThreadPool
	{


	public:
		ThreadPool(uint32_t threads)
		{
			m_Pool.resize(threads);
		}

		~ThreadPool() {
			Shutdown();
		}

		void Init() 
		{
			for (std::thread& thread : m_Pool)
			{
				thread = std::thread([&]() {
					while (true)
					{
						func_t func = m_Queue.pop();

						if (func.poison)
						{
							break;
						}

						if (!func.func)
						{
							SD_CORE_ERROR("Tried to call queued function in thread pool but it was missing!");
						}

						func.func();
					}
				});
			}
		}

		void Shutdown()
		{
			// 1. Poison the queue (one pill for each thread)
			for (std::thread& thread : m_Pool)
			{
				func_t t{
					{},
					true
				};
				m_Queue.push(t);
			}
			
			// 2. Wait for all threads to finish their current job and exit
			for (std::thread& thread : m_Pool)
			{
				thread.join();
			}
		}

		void Queue(std::function<void()> func)
		{
			func_t t{
				func, false
			};

			m_Queue.push(t);
		}

		void StepCoroutines()
		{
			std::vector<                           std::function<bool()>>  copyCoroutines;
			std::vector<std::tuple<my_time, float, std::function<void()>>> copyDefered;

			{
				std::unique_lock lock(m_MutexCoroutine);
				copyCoroutines = m_Coroutines;
				copyDefered = m_Defered;
			}

			for (uint32_t i = 0; i < copyCoroutines.size(); i++)
			{
				if (copyCoroutines.at(i)())
				{
					copyCoroutines.erase(copyCoroutines.begin() + i); // sucks, lots of moves
				}
			}

			for (uint32_t i = 0; i < copyDefered.size(); i++)
			{
				auto& [begin, delay, func] = copyDefered.at(i);

				if (get_time() - begin > (my_time)(delay * 1000.f))
				{
					func();
					copyDefered.erase(copyDefered.begin() + i);
				}
			}

			// move back
			{
				std::unique_lock lock(m_MutexCoroutine);
				m_Coroutines = std::move(copyCoroutines);
				m_Defered = std::move(copyDefered);
			}
		}

		void Defer(std::function<void()> func)
		{
			Delay(-1.f, func);
		}

		void Delay(float howLong, std::function<void()> func)
		{
			std::unique_lock lock(m_MutexCoroutine);
			m_Defered.emplace_back(get_time(), howLong, func);
		}

		template<typename Container, typename Func>
		void foreach(Container& container, Func&& func)
		{
			std::mutex mutex;
			std::condition_variable cond;

			size_t remaining = container.size();

			for (auto index = 0; index < container.size(); index++)
			{
				Queue([&, func, index]() // need to copy func?
					{
						func(index);

						std::unique_lock lock(mutex);
						remaining -= 1;
						cond.notify_one();
					});
			}

			std::unique_lock lock(mutex);
			cond.wait(lock, [&]() { return remaining == 0; });
		}


		template<typename Container, typename PreThreadFunc, typename Func>
		void foreach(
			Container& container,
			PreThreadFunc&& preThreadFunc,
			Func&& func)
		{
			std::mutex mutex;
			std::condition_variable cond;

			size_t remaining = container.size();
			auto index = 0;

			for (auto& element : container)
			{
				auto preThread = preThreadFunc(element);

				Queue([&, func, index, preThread]() // need to copy func?
					{
						func(index, preThread);

						std::unique_lock lock(mutex);
						remaining -= 1;
						cond.notify_one();
					});

				index += 1;
			}

			std::unique_lock lock(mutex);
			cond.wait(lock, [&]() { return remaining == 0; });
		}

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
		BlockingQueue<func_t> m_Queue;

		std::mutex m_MutexCoroutine;
		std::vector<                           std::function<bool()>>  m_Coroutines; // main thread tasks
		std::vector<std::tuple<my_time, float, std::function<void()>>> m_Defered;    // ^
	};

}