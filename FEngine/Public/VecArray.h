#pragma once

#include<Vec.h>
#include<vector>
#include"Reference.h"
#include"Ptr.h"


enum ElementType {
	UNKNOWN,
	VEC2F,
	VEC3F,
	VEC4F,
	FLOAT,
	DOUBLE,
	INT,
	SHORT,
	MATRIX4F,
	CUSTOM
};
class Array :public FObject{
public:
	
	// default
	Array(ElementType type = ElementType::UNKNOWN, size_t size = 0, uint32_t byteSize = 0) :mElementType(type),
		mElementNum(size), mElementByteSize(byteSize) {

	}
	// copy
	Array(const Array& array) {
		//todo
	}
	// move
	Array(Array&& array)
	{
		//todo
	}
	
protected:
	// profile
	ElementType mElementType;
	size_t mElementNum;  
	uint32_t mElementByteSize;

};

template<typename T, ElementType type,  int ElementSize>
class TemplateArray : public Array, public std::vector<T>
{
	TemplateArray(int ElementNum=0):Array(type, ElementNum, ElementSize), std::vector<T>(ElementNum){}
};

typedef TemplateArray<Vec3f, ElementType::VEC3F, 12> Vec3fArray;
typedef TemplateArray<Vec2f, ElementType::VEC2F, 8> Vec2fArray;
typedef TemplateArray<uint32_t, ElementType::INT, 4> IndexArray;
typedef TemplateArray<uint16_t, ElementType::SHORT, 2> ShortIndexArray;
typedef Ptr<Vec3fArray>  Vec3fArrayPtr;
typedef Ptr<Array>  ArrayPtr;

