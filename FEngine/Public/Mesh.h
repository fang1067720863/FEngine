#pragma once

#include"Buffer.h"


class Mesh
{
public:
	const VBufferDescriptor GetVertexBufferDescriptor() const { return vBufferDescriptor; }
	const IBufferDescriptor GetIndexBufferDescriptor() const { return iBufferDescriptor; }
	BufferObject* GetVBufferObject() { return vBufferObject.get(); }
	BufferObject* GetIBufferObject() { return iBufferObject.get(); }
	inline bool UseIndices() { return true; }

protected:
	VBufferDescriptor vBufferDescriptor;
	IBufferDescriptor iBufferDescriptor;
	bool useIndex;
	FPtr<BufferObject> vBufferObject;
	FPtr<BufferObject> iBufferObject;
};
