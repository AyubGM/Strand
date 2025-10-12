#pragma once

#include <mutex>
#include <queue>

namespace Strand {

	template<typename T>
	class Blocking_queue
	{

	public:
		/**
		* Pushes an item to the back of the queue.
		*
		* @param item The item to push.
		*/
		void push(const T& item)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_queue.push_back(item);

			//lock.unlock();
			m_condition.notify_one();
		}

		/**
		* Pops an item from the front of the queue.
		*/
		T pop() 
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_condition.wait(lock, [=] { return !m_queue.empty(); });

			T item = m_queue.front(); m_queue.pop_front();
			return item;
		}

		void clear()
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_queue.clear();

			//lock.unlock();
			m_condition.notify_one();
		}

		bool empty() 
		{
			return m_queue.empty(); // not sure if this is thread safe?
		}

		sizeT size() 
		{
			return m_queue.size();
		}

		iterator begin() 
		{
			return m_queue.begin();
		}

		iterator end() 
		{
			return m_queue.end();
		}

		const_iterator begin() const 
		{
			return m_queue.begin();
		}

		const_iterator end() const 
		{
			return m_queue.end();
		}

	public:
		using iterator = typename std::deque<T>::iterator;
		using const_iterator = typename std::deque<T>::const_iterator;

	private:
		std::mutex              m_Mutex;
		std::condition_variable m_Condition;
		std::deque<T>          m_Queue;
	};

}