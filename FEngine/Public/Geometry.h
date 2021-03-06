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
	FGeometry() = default;
	~FGeometry()
	{
		if (mCompiledVertices)
		{
			delete[] mCompiledVertices;
		}
	}
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
	uint32_t GetIndexELementNum()
	{
		if (!_indices) return 0;
		return _indices->getNumElements();
	}
	uint32_t GetVertexELementNum()
	{
		return _positions->getNumElements();
	}

	//hack
	bool Compile()
	{
		if (!_positions || !_normals || !_texcoords)
		{
			return false;
		}
		float gg = 1.0f;
		unsigned char* test2 = (unsigned char*)(&gg);
		const uint32_t vertexNum = _positions->getNumElements();
		const uint32_t stride = 4 * 3 + 4 * 3 + 4 * 2;
		mVertexStride = stride;
		unsigned char* posT = (unsigned char*)(_positions->data()->data());
		unsigned char* norT = (unsigned char*)(_normals->data()->data());
		unsigned char* tex0T = (unsigned char*)(_texcoords->data()->data());
		//unsigned int tt = 1.0;
		//unsigned char* test = (unsigned char*)(&tt);
		
		
		mCompiledVertices = new unsigned char[vertexNum * stride];
		
		for (auto i = 0; i < vertexNum; i++) {
			memcpy(mCompiledVertices + i * stride, posT + 12 * i, 12);
			memcpy(mCompiledVertices + i * stride +12, norT + 12 * i, 12);
			memcpy(mCompiledVertices + i * stride+24, tex0T + 8 * i, 8);
		}
		return true;
	}
	bool CompilePos()
	{
		if (!_positions)
		{
			return false;
		}
		const uint32_t vertexNum = _positions->getNumElements();
		const uint32_t stride = 4 * 3;
		mVertexStride = stride;
		/*unsigned char* posT = (unsigned char*)(_positions->data()->data());
		unsigned char* norT = (unsigned char*)(_normals->data()->data());
		unsigned char* tex0T = (unsigned char*)(_texcoords->data()->data());*/

		mCompiledVertices = new unsigned char[vertexNum * stride];
		memcpy(mCompiledVertices, _positions->data()->data(), vertexNum * stride);
	/*	for (auto i = 0; i < vertexNum; i++) {
			memcpy(mCompiledVertices + i * stride, posT + 12 * i, 12);
			memcpy(mCompiledVertices + i * stride + 12, norT + 12 * i, 12);
			memcpy(mCompiledVertices + i * stride + 20, tex0T + 8 * i, 8);
		}*/
		return true;
	}

	VertexElementType GetVertexElementType() const{ return mType; }
	unsigned int GetVertexStride() const
	{
		return mVertexStride;
	}
	void * GetCompiledVertexData() { return mCompiledVertices; }
	unsigned int GetCompiledVertexDataSize() { return mVertexStride * (_positions->getNumElements()); }

	void* GetCompiledIndexData() { return static_cast<void*>(_indices->data()); }
	unsigned int GetCompiledIndexDataSize() { return (_indices->getNumElements())*4; }
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
	bool mCompiled = false;
	unsigned int mVertexStride = 0;
	unsigned char* mCompiledVertices = nullptr;
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
		 Vertex3f(-1.0f, -1.0f, -1.0f);
		 Normal3f(0.0f, -1.0f, 0.0f);
		 TexCoord2f(0.0f, 0.0f);

		 Vertex3f(-1.0f, 1.0f, -1.0f);
		 Normal3f(0.0f, 1.0f, 0.0f);
         TexCoord2f(1.0f, 0.0f);


		 Vertex3f(1.0f, 1.0f, -1.0f);
		 Normal3f(1.0f, 0.0f, 0.0f);
		 TexCoord2f(1.0f, 0.0f);

		 Vertex3f(1.0f, -1.0f, -1.0f);
		 Normal3f(1.0f, 0.0f, 0.0f);
		 TexCoord2f(0.0f, 0.0f);

		 Vertex3f(-1.0f, -1.0f, 1.0f);
		 Normal3f(0.0f, -1.0f, 0.0f);
         TexCoord2f(0.0f, 1.0f);

		 Vertex3f(-1.0f, 1.0f, 1.0f);
		 Normal3f(-1.0f, 0.0f, 0.0f);
         TexCoord2f(0.0f, 1.0f);

		 Vertex3f(1.0f, 1.0f, 1.0f);
		 Normal3f(0.0f, 1.0f, 0.0f);
         TexCoord2f(0.0f, 1.0f);

		 Vertex3f(1.0f, -1.0f, 1.0f);
		 Normal3f(1.0f, 0.0f, 0.0f);
		 TexCoord2f(0.0f, 1.0f);

		 unsigned int indices[] = {
			 // ????
			 0, 1, 2,
			 2, 3, 0,
			 // ????
			 4, 5, 1,
			 1, 0, 4,
			 // ????
			 1, 5, 6,
			 6, 2, 1,
			 // ????
			 7, 6, 5,
			 5, 4, 7,
			 // ????
			 3, 2, 6,
			 6, 7, 3,
			 // ????
			 4, 0, 3,
			 3, 7, 4
		 };
		 for (int i = 0; i < 36; i++)
		 {
			 _indices->emplace_back(indices[i]);
		 }
		 


		 // 24 points in box
		 // create body
		 //Normal3f(0.0f, -1.0f, 0.0f);
		 //TexCoord2f(0.0f, 1.0f);
		 //Vertex3f(-dx, -dy, dz);

		 //Normal3f(0.0f, -1.0f, 0.0f);
		 //TexCoord2f(0.0f, 0.0f);
		 //Vertex3f(-dx, -dy, -dz);

		 //Normal3f(0.0f, -1.0f, 0.0f);
		 //TexCoord2f(1.0f, 0.0f);
		 //Vertex3f(dx, -dy, -dz);

		 //Normal3f(0.0f, -1.0f, 0.0f);
		 //TexCoord2f(1.0f, 1.0f);
		 //Vertex3f(dx, -dy, dz);

		 //// +ve y plane
		 //Normal3f(0.0f, 1.0f, 0.0f);
		 //TexCoord2f(0.0f, 1.0f);
		 //Vertex3f(dx, dy, dz);

		 //Normal3f(0.0f, 1.0f, 0.0f);
		 //TexCoord2f(0.0f, 0.0f);
		 //Vertex3f(dx, dy, -dz);

		 //Normal3f(0.0f, 1.0f, 0.0f);
		 //TexCoord2f(1.0f, 0.0f);
		 //Vertex3f(-dx, dy, -dz);

		 //Normal3f(0.0f, 1.0f, 0.0f);
		 //TexCoord2f(1.0f, 1.0f);
		 //Vertex3f(-dx, dy, dz);

		 //// +ve x plane
		 //Normal3f(1.0f, 0.0f, 0.0f);
		 //TexCoord2f(0.0f, 1.0f);
		 //Vertex3f(dx, -dy, dz);

		 //Normal3f(1.0f, 0.0f, 0.0f);
		 //TexCoord2f(0.0f, 0.0f);
		 //Vertex3f(dx, -dy, -dz);

		 //Normal3f(1.0f, 0.0f, 0.0f);
		 //TexCoord2f(1.0f, 0.0f);
		 //Vertex3f(dx, dy, -dz);

		 //Normal3f(1.0f, 0.0f, 0.0f);
		 //TexCoord2f(1.0f, 1.0f);
		 //Vertex3f(dx, dy, dz);

		 //// -ve x plane
		 //Normal3f(-1.0f, 0.0f, 0.0f);
		 //TexCoord2f(0.0f, 1.0f);
		 //Vertex3f(-dx, dy, dz);

		 //Normal3f(-1.0f, 0.0f, 0.0f);
		 //TexCoord2f(0.0f, 0.0f);
		 //Vertex3f(-dx, dy, -dz);

		 //Normal3f(-1.0f, 0.0f, 0.0f);
		 //TexCoord2f(1.0f, 0.0f);
		 //Vertex3f(-dx, -dy, -dz);

		 //Normal3f(-1.0f, 0.0f, 0.0f);
		 //TexCoord2f(1.0f, 1.0f);
		 //Vertex3f(-dx, -dy, dz);

		 ////createTop
		 //// +ve z plane
		 //Normal3f(0.0f, 0.0f, 1.0f);
		 //TexCoord2f(0.0f, 1.0f);
		 //Vertex3f(-dx, dy, dz);

		 //Normal3f(0.0f, 0.0f, 1.0f);
		 //TexCoord2f(0.0f, 0.0f);
		 //Vertex3f(-dx, -dy, dz);

		 //Normal3f(0.0f, 0.0f, 1.0f);
		 //TexCoord2f(1.0f, 0.0f);
		 //Vertex3f(dx, -dy, dz);

		 //Normal3f(0.0f, 0.0f, 1.0f);
		 //TexCoord2f(1.0f, 1.0f);
		 //Vertex3f(dx, dy, dz);


		 ////createBottom
		 //Normal3f(0.0f, 0.0f, -1.0f);
		 //TexCoord2f(0.0f, 1.0f);
		 //Vertex3f(dx, dy, -dz);

		 //Normal3f(0.0f, 0.0f, -1.0f);
		 //TexCoord2f(0.0f, 0.0f);
		 //Vertex3f(dx, -dy, -dz);

		 //Normal3f(0.0f, 0.0f, -1.0f);
		 //TexCoord2f(1.0f, 0.0f);
		 //Vertex3f(-dx, -dy, -dz);

		 //Normal3f(0.0f, 0.0f, -1.0f);
		 //TexCoord2f(1.0f, 1.0f);
		 //Vertex3f(-dx, dy, -dz);

		// End();
	 }
	 void buildTriangle()
	 {
		 Vertex3f(0.5, 0.5, 0.5);
		 Vertex3f(0.8, 0.5, 0.5);
		 Vertex3f(0.5, -0.5, 0.5);
	 }

	 void End()
	 {
		 // Quad Mode
		 for (unsigned int i = 0; i < _vertices->size(); i += 4)
		 {
			 unsigned int p0 = i;
			 unsigned int p1 = i + 1;
			 unsigned int p2 = i + 2;
			 unsigned int p3 = i + 3;

			 _indices->emplace_back(p0);
			 _indices->emplace_back(p1);
			 _indices->emplace_back(p2);

			 _indices->emplace_back(p2);
			 _indices->emplace_back(p3);
			 _indices->emplace_back(p0);
		 }
	 }
	 FGeometry* BuildBox(const FBox& box)
	 {
		 build(box);
		 FGeometry* geom = new FGeometry();
		 geom->SetVertexPositionArray(_vertices.get());
		 geom->SetVertexNormalArray(_normals.get());
		 geom->SetVertexTexcoordArray(_texcoords.get());
		 geom->SetIndexArray(_indices.get());
		 //geom->CompilePos();
		 geom->Compile();
		 return geom;
	 }
	 FGeometry* BuildTriangle()
	 {
		 buildTriangle();
		 FGeometry* geom = new FGeometry();
		 geom->SetVertexPositionArray(_vertices.get());
		 geom->CompilePos();
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
		_colors = new Vec4fArray();
		_indices = new IndexArray();
	}
protected:
	// ????????????????????
	// inline void Vertex(Vec3f&& v) { _vertices->push_back(std::move(v)); }
	// ???????????? ????????
	inline void Vertex3f(const Vec3f& v) { _vertices->push_back(v); }
	inline void Normal3f(const Vec3f& v) { _normals->push_back(v); }
	inline void TexCoord2f(const Vec2f& v) { _texcoords->push_back(v); }
	//inline void Color(const Vec4f& v) { _colors->push_back(v); }
	inline void Vertex3f(float x, float y, float z) { Vertex3f(Vec3f(x, y, z)); }
	inline void Normal3f(float x, float y, float z) { Normal3f(Vec3f(x, y, z)); }
	inline void TexCoord2f(float u, float v) { TexCoord2f(Vec2f(u, v)); }
	//inline void Color(const Vec4f& v) { Color(Vec4f(x, y, z, w)); }
	
	Vec3fArrayPtr _vertices;
	Vec3fArrayPtr _normals;
	Ptr<Vec2fArray> _texcoords;
	Ptr<Vec4fArray> _colors;
    Ptr<IndexArray> _indices;
	
	

};
