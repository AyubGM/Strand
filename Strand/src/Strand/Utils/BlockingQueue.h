#pragma once

#include <mutex>
#include <queue>

namespace Strand {

	template<typename T>
	class BlockingQueue
	{
	public:
		using iterator = typename std::deque<T>::iterator;
		using const_iterator = typename std::deque<T>::const_iterator;

	public:
		/**
		* Pushes an item to the back of the queue.
		*
		* @param item The item to push.
		*/
		void push(const T& item)
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Queue.push_back(item);

			m_Condition.notify_one();
		}

		/**
		* Pops an item from the front of the queue.
		*/
		T pop() 
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Condition.wait(lock, [=] { return !m_Queue.empty(); });

			T item = m_Queue.front(); m_Queue.pop_front();
			return item;
		}

		void clear()
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Queue.clear();

			//m_Condition.notify_all()
			m_Condition.notify_one();
		}

		bool empty() 
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			return m_Queue.empty(); // not sure if this is thread safe?
		}

		sizeT size() 
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			return m_Queue.size();
		}

		iterator begin() 
		{
			return m_Queue.begin();
		}

		iterator end() 
		{
			return m_Queue.end();
		}

		const_iterator begin() const 
		{
			return m_Queue.begin();
		}

		const_iterator end() const 
		{
			return m_Queue.end();
		}



	private:
		std::mutex              m_Mutex;
		std::condition_variable m_Condition;
		std::deque<T>          m_Queue;
	};

}