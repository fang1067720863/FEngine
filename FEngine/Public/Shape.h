#pragma once

#include"VecArray.h"



class FBox;
class FQuad;
class FTriangle;
class FSphere;
class IShape;
class IDrawer : public FReference
{
public:
	IDrawer() = default;
	virtual bool Draw(const IShape& event) { return false; }
	virtual bool Draw(const FBox& event)  { return false; }
	virtual bool Draw(const FQuad& event) { return false; }
	virtual bool Draw(const FTriangle& event)  { return false; }
	virtual bool Draw(const FSphere& event) { return false; }
};

class IShape :public FReference
{
public:
	virtual void Drawn(IDrawer& drawer) const = 0;
};

class FBox :public IShape
{
public:
	FBox(const Vec3f& halfVec) : mHalfLength(halfVec[0] / 2)
		, mHalfWidth(halfVec[1] / 2), mHalfHeight(halfVec[2] / 2) {}
	FBox(const float& length) : mHalfLength(length / 2)
		, mHalfWidth(length / 2), mHalfHeight(length / 2) {}
	float mHalfWidth;
	float mHalfLength;
	float mHalfHeight;

	virtual void Drawn(IDrawer& drawer) const
	{
		drawer.Draw(*this);
	}
};
class FTriangle : public IShape
{
public:
	virtual void Drawn(IDrawer& drawer) const
	{
		drawer.Draw(*this);
	}
};
class FSphere : public IShape
{
public:
	virtual void Drawn(IDrawer& drawer) const
	{
		drawer.Draw(*this);
	}
};
class FQuad : public IShape
{
public:
	virtual void Drawn(IDrawer& drawer) const
	{
		drawer.Draw(*this);
	}
};



class FGeometry;
class ShapeGeometryBuilder:protected IDrawer
{
public:
	static ShapeGeometryBuilder& instance()
	{
		static ShapeGeometryBuilder instance;
		return instance;
	}

	Ptr<FGeometry> BuildGeomtry(const IShape& shape);


protected:

	void End();
	virtual bool Draw(const FBox& box);
	virtual bool Draw(const FTriangle& triangle);
	virtual bool Draw(const FSphere& sphere);
	virtual bool Draw(const FQuad& quad);
	

	
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
