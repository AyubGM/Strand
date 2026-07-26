#include "sdpch.h"
#include "RenderQueue.h"

namespace Strand
{
	RenderQueue::RenderQueue()
	{
	}

	RenderQueue::RenderQueue(uint32_t capacity)
	{
		m_Objects.reserve(capacity);
	}

	RenderQueue::~RenderQueue()
	{
	}

	void RenderQueue::Clear()
	{
		m_Objects.clear();
	}

	void RenderQueue::Emplace(const RenderObject& object)
	{
		m_Objects.emplace_back(object);
	}

	void RenderQueue::Reserve(uint32_t capacity)
	{
		m_Objects.reserve(capacity);
	}
}