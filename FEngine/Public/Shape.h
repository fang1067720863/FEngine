#pragma once

#include"VecArray.h"

class FShape :public FReference
{

};
class FBox :public FShape
{
public:
	FBox(const Vec3f& halfVec) : mHalfLength(halfVec[0] / 2)
		, mHalfWidth(halfVec[1] / 2), mHalfHeight(halfVec[2] / 2) {}
	FBox(const float& length) : mHalfLength(length / 2)
		, mHalfWidth(length / 2), mHalfHeight(length / 2) {}
	float mHalfWidth;
	float mHalfLength;
	float mHalfHeight;
};


class FGeometry;
class ShapeGeometryBuilder
{
public:
	static ShapeGeometryBuilder& instance()
	{
		static ShapeGeometryBuilder instance;
		return instance;
	}
	FGeometry* Build(const FShape& shape);

	void build(const FBox& box);
	void buildTriangle();
	void buildQuad();
	

	void End();

	Ptr<FGeometry> BuildBox(const FBox& box);
	Ptr<FGeometry> BuildRenderQuad();
	FGeometry* BuildTriangle();
	
private:
	ShapeGeometryBuilder() {
		_vertices = new Vec3fArray();
		mNormalArray = new Vec3fArray();
		mTexcoordArray = new Vec2fArray();
		_colors = new Vec4fArray();
		mIndexArray = new IndexArray();
	}

	void Clear()
	{
		_vertices->clear();
		mNormalArray->clear();
		mTexcoordArray->clear();
		mIndexArray->clear();
	}
protected:
	inline void Vertex3f(const Vec3f& v) { _vertices->push_back(v); }
	inline void Normal3f(const Vec3f& v) { mNormalArray->push_back(v); }
	inline void TexCoord2f(const Vec2f& v) { mTexcoordArray->push_back(v); }
	inline void Vertex3f(float x, float y, float z) { Vertex3f(Vec3f(x, y, z)); }
	inline void Normal3f(float x, float y, float z) { Normal3f(Vec3f(x, y, z)); }
	inline void TexCoord2f(float u, float v) { TexCoord2f(Vec2f(u, v)); }


	Vec3fArrayPtr _vertices;
	Vec3fArrayPtr mNormalArray;
	Ptr<Vec2fArray> mTexcoordArray;
	Ptr<Vec4fArray> _colors;
	Ptr<IndexArray> mIndexArray;



};
