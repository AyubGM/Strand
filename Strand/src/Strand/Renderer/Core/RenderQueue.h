#pragma once
#include "RenderObject.h"
#include <vector>
namespace Strand {

	class RenderQueue
	{
	public:
		RenderQueue();
		RenderQueue(uint32_t capacity);
		~RenderQueue();

		void Clear();
		void Emplace(const RenderObject& object);
		void Reserve(uint32_t capacity);

		const std::vector<RenderObject>& GetData() const { return m_Objects; }
		std::vector<RenderObject>& GetData() { return m_Objects; }

		uint32_t GetCount() const { return static_cast<uint32_t>(m_Objects.size()); }
		bool Empty() const { return m_Objects.empty(); }

	private:
		std::vector<RenderObject> m_Objects;

	};

	
}