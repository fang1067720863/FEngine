#pragma once

#include"VecArray.h"
#include"Node.h"
#include"CommonType.h"
#include"Material.h"
#include"Reference.h"
#include"UtfConverter.h"




class FGeometry: public FNode
{
	PROPERTY_REF_PTR(Vec3fArray, PositionArray)
	PROPERTY_REF_PTR(Vec3fArray, NormalArray)
	PROPERTY_REF_PTR(Vec2fArray, TexcoordArray)
	PROPERTY_REF_PTR(IndexArray, IndexArray)
	PROPERTY_DEFAULT(MaterialType, MaterialType, MaterialType::None)
	PROPERTY(File, ModelFile)
	
public:
	FGeometry() = default;
	FGeometry(const std::string& name) :FNode(name){}
	~FGeometry()
	{
		if (mCompiledVertices)
		{
			delete[] mCompiledVertices;
		}
	}
 	
	uint32_t GetIndexELementNum()
	{
		return mIndexNum;
	}
	uint32_t GetVertexELementNum(){return mVertexNum;}
	
	//hack
	bool Compile();
	bool CompilePos();
	
	
	VertexElementType GetVertexElementType() const{ return mType; }
	unsigned int GetVertexStride() const {return mVertexStride;}
	void * GetCompiledVertexData() { return mCompiledVertices; }
	unsigned int GetCompiledVertexDataSize() { return mVertexStride * mVertexNum; }

	void* GetCompiledIndexData() { return static_cast<void*>(mIndexArray->data()); }
	unsigned int GetCompiledIndexDataSize() { return mIndexNum * mIndexStride; }

	int32_t materialSlot = -1;
	int32_t materialMapSlot = -1;
	

	
	PrimitiveType mIndicesType;
	VertexElementType mType = VertexElementType::POS_NOR_TEXO;
	bool mCompiled = false;
	unsigned int mVertexStride = 0;
	unsigned int mVertexNum = 0;
	unsigned char* mCompiledVertices = nullptr;


	const unsigned int mIndexStride = 4;
	unsigned int mIndexNum = 0;

	
};
