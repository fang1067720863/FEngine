#pragma once

#include<vector>
typedef struct VBufferDescriptor
{
	uint32_t registerSlot = 0;
	uint32_t numBuffers = 1;
	uint32_t stride = 0;             //cellSize
	uint32_t offset = 0;
	std::vector<uint32_t> strides;
	std::vector<uint32_t> offsets;
	uint32_t count = 0;
	uint32_t startVertexLocation = 0;
}VBufferDescriptor;
typedef struct IBufferDescriptor
{
	uint32_t count = 0;
	uint32_t offset = 0;
	uint32_t startIndexLocation = 0;
	uint32_t baseVertexLocation = 0;   // incement
	bool used = false;
}IBufferDescriptor;




enum BufferType :uint8_t {
	Vertex,
	Index,
	BT_Uniform,
	ShaderStorage,
	Indirect
};
struct BufferDescription
{
	enum Usage
	{
		DEFAULT = 0,
		IMMUTABLE = 1,
		DYNAMIC = 2,
		STAGING = 3
	};
	enum BindFlag
	{
		VERTEX_BUFFER = 0x1L,
		INDEX_BUFFER = 0x2L,
		CONSTANT_BUFFER = 0x4L,
		SHADER_RESOURCE = 0x8L,
		STREAM_OUTPUT = 0x10L,
		RENDER_TARGET = 0x20L,
		DEPTH_STENCIL = 0x40L,
		UNORDERED_ACCESS = 0x80L,
		DECODER = 0x200L,
		VIDEO_ENCODER = 0x400L
	};
	Usage usage = Usage::DEFAULT;
	BindFlag bindFlag = BindFlag::VERTEX_BUFFER;
	unsigned int byteWidth = 0;
	unsigned int CPUAccessFlags = 0;
	bool GPUWritable = true;
};
