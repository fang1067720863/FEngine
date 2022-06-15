#pragma once

#include"VecArray.h"
#include"Ptr.h"

class FShape:public FReference
{

};
class FBox:public FShape
{
public:
	FBox(const Vec3f& halfVec): mHalfLength(halfVec[0]/2) 
		, mHalfWidth(halfVec[1] / 2), mHalfHeight(halfVec[2] / 2) {}
	FBox(const float& length) : mHalfLength(length / 2)
		, mHalfWidth(length / 2), mHalfHeight(length / 2) {}
	float mHalfWidth;
	float mHalfLength;
	float mHalfHeight;
};
class FGeometry: public FReference
{
public:
	void SetVertexPositionArray(Vec3fArray* poss)
	{
		_positions = poss;
	}
	void SetVertexNormalArray(Vec3fArray* normals)
	{
		_normals = normals;
	}
	void SetVertexTexcoordArray(Vec2fArray* texcoords)
	{
		_texcoords = texcoords;
	}
	void SetIndexArray(IndexArray* indices)
	{
		_indices = indices;
	}

	VertexElementType GetVertexElementType() const{ return mType; }
	unsigned char* GetCompiledVertexData() { return nullptr; }
	unsigned int GetCompiledVertexDataSize() { return 0; }

	unsigned char* GetCompiledIndexData() { return nullptr; }
	unsigned int GetCompiledIndexDataSize() { return 0; }
protected:
	Ptr<Vec3fArray> _positions;
	Ptr<Vec3fArray> _normals;
	Ptr<Vec2fArray> _texcoords;
	Ptr<IndexArray> _indices;

	enum IndexType {
		Triange,
		Quad
	};
	IndexType mIndicesType;
	VertexElementType mType;
};
class ShapeGeometryBuilder
{
public:
	static FGeometry* Build(FShape* shape) { 
		build(&shape);

		FGeometry* geom = new FGeometry();
		geom->SetVertexPositionArray(_vertices);
		return nullptr; }
	void build(const FBox& box);
protected:
	// 移动之后还是会析构吗
	// inline void Vertex(Vec3f&& v) { _vertices->push_back(std::move(v)); }
	// 一次拷贝构造 一次析构
	inline void Vertex3f(const Vec3f& v) { _vertices->push_back(v); }
	inline void Normal3f(const Vec3f& v) { _normals->push_back(v); }
	inline void TexCoord2f(const Vec2f& v) { _texcoords->push_back(v); }
	inline void Vertex3f(float x, float y, float z) { Vertex3f(Vec3f(x, y, z)); }
	inline void Normal3f(float x, float y, float z) { Normal3f(Vec3f(x, y, z)); }
	inline void TexCoord2f(float u, float v) { TexCoord2f(Vec2f(u, v)); }
	
	Ptr<Vec3fArray> _vertices;
	Ptr<Vec3fArray> _normals;
	Ptr<Vec2fArray> _texcoords;
    Ptr<IndexArray> _indices;
	
	

};
