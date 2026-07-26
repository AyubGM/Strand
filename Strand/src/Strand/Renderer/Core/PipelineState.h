#pragma once
#include <cstdint>
#include <glm/glm.hpp>

namespace Strand {

	enum class CullMode : uint8_t
	{
		None = 0,
		Front,
		Back,
	};

	enum class DepthFunc : uint8_t
	{
		Never = 0,
		Less,
		Equal,
		LessEqual,
		Greater,
		NotEqual,
		GreaterEqual,
		Always,
	};

	enum class BlendMode : uint8_t
	{
		None = 0,
		Alpha,
		Additive,
		Multiply
	};

	enum class PrimitiveTopology : uint8_t {
		Triangles = 0,
		Lines,
		LineStrip,
		Points
	};

	struct PipelineState
	{
		uint32_t ShaderID = 0;
		CullMode Cull = CullMode::Back;
		DepthFunc Depth = DepthFunc::LessEqual;
		bool DepthWrite = true;
		bool DepthTest = true;
		BlendMode Blend = BlendMode::None;
		PrimitiveTopology Topology = PrimitiveTopology::Triangles;

		// [21 unused bits] [1 bit:Test] [1 bit:Write] [2 bits:Topology] [2 bits:Blend] [3 bits:Depth] [2 bits:Cull]
		[[nodiscard]] uint64_t GetPackedKey() const
		{
			// the state of the pipeline is packed into a 32-bit integer, with the shader ID occupying the upper 32 bits
			uint32_t stateKey = 0;
			stateKey |= static_cast<uint32_t>(Cull);
			stateKey |= static_cast<uint32_t>(Depth) << 2;       // Cull uses 2 bits
			stateKey |= static_cast<uint32_t>(Blend) << 5;	   // Depth uses 3 bits
			stateKey |= static_cast<uint32_t>(Topology) << 7;   // Blend uses 2 bits
			stateKey |= (DepthWrite ? 1u : 0u) << 9;              // Topology uses 2 bits
			stateKey |= (DepthTest ? 1u : 0u) << 10;              // Write uses 1 bit

			//TODO ADD stencil state, alpha to coverage, etc. to the key

			return  (static_cast<uint64_t>(ShaderID << 32) | stateKey);
		}

		bool operator==(const PipelineState& other) const {
			return GetPackedKey() == other.GetPackedKey();
		}
	};

}
template<>
struct std::hash<Strand::PipelineState> {
	size_t operator()(const Strand::PipelineState& state) const noexcept {
		return std::hash<uint64_t>{}(state.GetPackedKey());
	}
};
