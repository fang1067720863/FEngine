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
	ET_FLOAT,
	ET_DOUBLE,
	ET_INT,
	ET_SHORT,
	MATRIX4F,
	STRUCT,
	ARRAY,
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
public:
	TemplateArray(int ElementNum=0):Array(type, ElementNum, ElementSize), std::vector<T>(ElementNum){}
	virtual unsigned int    getNumElements() const { 
		return static_cast<unsigned int>(this->size()); }
	virtual void reserveArray(unsigned int num) { this->reserve(num); }

};
// ͨ��ģ��Vec2, Vec3�����Ǽ̳���ʵ�ֶ�̬ �� ĳ����Ҫ�������ֲ�ȷ������ ģ��̳���һ����ģ�����Array
typedef TemplateArray<Vec3f, ElementType::VEC3F, 12> Vec3fArray;
typedef TemplateArray<Vec2f, ElementType::VEC2F, 8> Vec2fArray;
typedef TemplateArray<uint32_t, ElementType::ET_INT, 4> IndexArray;
typedef TemplateArray<uint16_t, ElementType::ET_SHORT, 2> ShortIndexArray;
typedef Ptr<Vec3fArray>  Vec3fArrayPtr;
typedef Ptr<Array>  ArrayPtr;

