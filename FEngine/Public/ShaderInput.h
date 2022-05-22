#pragma once

#include"CommonType.h"

using IndexType = uint32_t;
// 所有的资源通过RegisterSlot 注册到ShaderResouceGroup (SRG当中)
static const uint32_t UndefinedRegisterSlot = static_cast<uint32_t>(-1);

using ShaderInputConstantIndex = Handle< IndexType, ShaderInputConstantDescriptor>;
using ShaderInputSamplerIndex = Handle< IndexType, ShaderInputSamplerDescriptor>;
using ShaderInputImageIndex = Handle< IndexType, ShaderInputImageDescriptor>;
using ShaderInputBufferIndex = Handle< IndexType, ShaderInputBufferDescriptor>;


enum class ShaderInputType
{
	Buffer = 0,
	Image = 1,
	Sampler = 2,
	Constant = 3,
	Static = 4
};

class ShaderInputBufferDescriptor
{

};
class ShaderInputConstantDescriptor
{
	ShaderInputConstantDescriptor() = default;
	ShaderInputConstantDescriptor(
		const std::string& name,
		uint32_t constantByteOffset,
		uint32_t constantByteCount,
		uint32_t registerId);

	std::string m_name;
	uint32_t constantByteOffset = 0;
	uint32_t constantByteCount = 0;
	uint32_t registerId = 0;
};
class ShaderInputSamplerDescriptor
{
	ShaderInputSamplerDescriptor() = default;
	ShaderInputSamplerDescriptor(const std::string& name, uint32_t samplerCount, uint32_t registerId);

	std::string m_name;
	uint32_t m_count = 0;
	uint32_t m_registerId = 0;
};
class ShaderInputImageDescriptor
{
	enum class ShaderInputImageType : uint32_t
	{
		Unknown = 0,
		Image1D,
		Image1DArray,
		Image2D,
		Image2DArray,
		Image2DMultisample,
		Image2DMultisampleArray,
		Image3D,
		ImageCube,
		ImageCubeArray,
		SubpassInput
	};
	ShaderInputImageDescriptor() = default;
	ShaderInputImageDescriptor(
		const std::string& name,
		ShaderInputImageType type,
		uint32_t imageCount,
		uint32_t registerId);

	std::string m_name;
	uint32_t m_count = 0;    // image数量
	uint32_t m_registerId = UndefinedRegisterSlot;
	ShaderInputImageType m_type = ShaderInputImageType::Unknown;
	// ShaderInputImageAccess m_access = ShaderInputImageAccess::Read;
};


