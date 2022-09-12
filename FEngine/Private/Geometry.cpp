#include"Geometry.h"

bool FGeometry::Compile()
{
	if (!mPositionArray || !mNormalArray || !mTexcoordArray)
	{
		return false;
	}
	const uint32_t vertexNum = mPositionArray->getNumElements();
	const uint32_t stride = 4 * 3 + 4 * 3 + 4 * 2;
	mVertexStride = stride;
	unsigned char* posT = (unsigned char*)(mPositionArray->data()->data());
	unsigned char* norT = (unsigned char*)(mNormalArray->data()->data());
	unsigned char* tex0T = (unsigned char*)(mTexcoordArray->data()->data());

	mCompiledVertices = new unsigned char[vertexNum * stride];
	if (!mCompiledVertices)
	{
		int a = 1;
	}
	for (auto i = 0; i < vertexNum; i++) {
		memcpy(mCompiledVertices + i * stride, posT + 12 * i, 12);
		memcpy(mCompiledVertices + i * stride + 12, norT + 12 * i, 12);
		memcpy(mCompiledVertices + i * stride + 24, tex0T + 8 * i, 8);
	}
	return true;
}
bool FGeometry::CompilePos()
{
	if (!mPositionArray)
	{
		return false;
	}
	const uint32_t vertexNum = mPositionArray->getNumElements();
	const uint32_t stride = 4 * 3;
	mVertexStride = stride;
	mCompiledVertices = new unsigned char[vertexNum * stride];
	memcpy(mCompiledVertices, mPositionArray->data()->data(), vertexNum * stride);
	return true;
}