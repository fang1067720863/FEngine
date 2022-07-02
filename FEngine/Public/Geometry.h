#pragma once

#include"VecArray.h"
#include"Ptr.h"
#include"CommonType.h"

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
	Vec3fArray* GetVertexPositionArray()
	{
		return _positions.get();
	}
	Vec3fArray* GetVertexNormalArray()
	{
		return _normals.get();
	}
	Vec2fArray* GetVertexTexcoordArray()
	{
		return _texcoords.get();
	}
	IndexArray* GetIndexArray(IndexArray* indices)
	{
		return _indices.get();
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
	VertexElementType mType = VertexElementType::POS_NOR_TEXO;
};

class ShapeGeometryBuilder
{
public:
	static ShapeGeometryBuilder& instance()
	{
		static ShapeGeometryBuilder instance;
		return instance;
	}
     FGeometry* Build(const FShape& shape) {
		/*build(shape);
		FGeometry* geom = new FGeometry();
		geom->SetVertexPositionArray(_vertices.get());
		geom->SetVertexNormalArray(_normals.get());
		geom->SetVertexTexcoordArray(_texcoords.get());
		geom->SetIndexArray(_indices.get());
		return geom;*/
	 }

	 void build(const FBox& box)
	 {
		 float dx = box.mHalfLength;
		 float dy = box.mHalfWidth;
		 float dz = box.mHalfHeight;

		 // 24 points in box
		 // create body
		 Normal3f(0.0f, -1.0f, 0.0f);
		 TexCoord2f(0.0f, 1.0f);
		 Vertex3f(-dx, -dy, dz);

		 Normal3f(0.0f, -1.0f, 0.0f);
		 TexCoord2f(0.0f, 0.0f);
		 Vertex3f(-dx, -dy, -dz);

		 Normal3f(0.0f, -1.0f, 0.0f);
		 TexCoord2f(1.0f, 0.0f);
		 Vertex3f(dx, -dy, -dz);

		 Normal3f(0.0f, -1.0f, 0.0f);
		 TexCoord2f(1.0f, 1.0f);
		 Vertex3f(dx, -dy, dz);

		 // +ve y plane
		 Normal3f(0.0f, 1.0f, 0.0f);
		 TexCoord2f(0.0f, 1.0f);
		 Vertex3f(dx, dy, dz);

		 Normal3f(0.0f, 1.0f, 0.0f);
		 TexCoord2f(0.0f, 0.0f);
		 Vertex3f(dx, dy, -dz);

		 Normal3f(0.0f, 1.0f, 0.0f);
		 TexCoord2f(1.0f, 0.0f);
		 Vertex3f(-dx, dy, -dz);

		 Normal3f(0.0f, 1.0f, 0.0f);
		 TexCoord2f(1.0f, 1.0f);
		 Vertex3f(-dx, dy, dz);

		 // +ve x plane
		 Normal3f(1.0f, 0.0f, 0.0f);
		 TexCoord2f(0.0f, 1.0f);
		 Vertex3f(dx, -dy, dz);

		 Normal3f(1.0f, 0.0f, 0.0f);
		 TexCoord2f(0.0f, 0.0f);
		 Vertex3f(dx, -dy, -dz);

		 Normal3f(1.0f, 0.0f, 0.0f);
		 TexCoord2f(1.0f, 0.0f);
		 Vertex3f(dx, dy, -dz);

		 Normal3f(1.0f, 0.0f, 0.0f);
		 TexCoord2f(1.0f, 1.0f);
		 Vertex3f(dx, dy, dz);

		 // -ve x plane
		 Normal3f(-1.0f, 0.0f, 0.0f);
		 TexCoord2f(0.0f, 1.0f);
		 Vertex3f(-dx, dy, dz);

		 Normal3f(-1.0f, 0.0f, 0.0f);
		 TexCoord2f(0.0f, 0.0f);
		 Vertex3f(-dx, dy, -dz);

		 Normal3f(-1.0f, 0.0f, 0.0f);
		 TexCoord2f(1.0f, 0.0f);
		 Vertex3f(-dx, -dy, -dz);

		 Normal3f(-1.0f, 0.0f, 0.0f);
		 TexCoord2f(1.0f, 1.0f);
		 Vertex3f(-dx, -dy, dz);

		 //createTop
		 // +ve z plane
		 Normal3f(0.0f, 0.0f, 1.0f);
		 TexCoord2f(0.0f, 1.0f);
		 Vertex3f(-dx, dy, dz);

		 Normal3f(0.0f, 0.0f, 1.0f);
		 TexCoord2f(0.0f, 0.0f);
		 Vertex3f(-dx, -dy, dz);

		 Normal3f(0.0f, 0.0f, 1.0f);
		 TexCoord2f(1.0f, 0.0f);
		 Vertex3f(dx, -dy, dz);

		 Normal3f(0.0f, 0.0f, 1.0f);
		 TexCoord2f(1.0f, 1.0f);
		 Vertex3f(dx, dy, dz);


		 //createBottom
		 Normal3f(0.0f, 0.0f, -1.0f);
		 TexCoord2f(0.0f, 1.0f);
		 Vertex3f(dx, dy, -dz);

		 Normal3f(0.0f, 0.0f, -1.0f);
		 TexCoord2f(0.0f, 0.0f);
		 Vertex3f(dx, -dy, -dz);

		 Normal3f(0.0f, 0.0f, -1.0f);
		 TexCoord2f(1.0f, 0.0f);
		 Vertex3f(-dx, -dy, -dz);

		 Normal3f(0.0f, 0.0f, -1.0f);
		 TexCoord2f(1.0f, 1.0f);
		 Vertex3f(-dx, dy, -dz);

	 }
	 FGeometry* BuildBox(const FBox& box)
	 {
		 build(box);
		 FGeometry* geom = new FGeometry();
		 geom->SetVertexPositionArray(_vertices.get());
		 geom->SetVertexNormalArray(_normals.get());
		 geom->SetVertexTexcoordArray(_texcoords.get());
		 geom->SetIndexArray(_indices.get());
		 return geom;
	 }
	 void Clear()
	 {
		 
	 }
private:
	ShapeGeometryBuilder() {
		_vertices = new Vec3fArray();
		_normals= new Vec3fArray();
		_texcoords = new Vec2fArray();
		_indices = new IndexArray();
	}
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
