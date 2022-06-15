#pragma once

#include<string>
#include<vector>
#include<memory>
#include"Vec.h"
#include"Matrix.h"
#include"VecArray.h"
#include"Reference.h"


class UniformBase: public FReference
{

public:
    UniformBase(std::string sematic, ElementType type):mOffset(0), mByteSize(0),mSematic(sematic), mType(type) {}
    //virtual void AddSubUniform(Ptr<UniformBase> uniform) = 0;
public:
    // meta
    std::string mSematic;
    ElementType mType;

    uint32_t mOffset;
    bool mCompiled = false;
    uint32_t mByteSize;

    virtual bool Compile(uint32_t offset = 0) = 0;
    virtual ~UniformBase() {}
    virtual void Print(int incident = 0) = 0;
    virtual unsigned char* GetDataPtr() { return nullptr; }


};


template<class T, ElementType type=ElementType::CUSTOM>
class Uniform :public UniformBase
{
public:
    // Basic type
    Uniform(std::string sematic) :UniformBase(sematic, type) {}  // default value
    Uniform(std::string sematic, const T& data) :UniformBase(sematic, type) , mData(data){

        mByteSize = sizeof(T);
        mOffset = 0;
        mCompiled = true;
       // mData = data;
    }
    ~Uniform() {}
    T mData;

    void AddSubUniform(Ptr<UniformBase> uniform) {
        mSubUniformList.push_back(uniform);
    }


    template<class U>
    void ModifySubUniform(std::string Sematic, uint32_t index, U data) {}
    uint32_t GetByteSize() { return mByteSize; }
   
    bool Compile(uint32_t offset = 0) override {
        mOffset = offset;
        uint32_t range = 0;
        for (auto iter = mSubUniformList.begin(); iter != mSubUniformList.end(); iter++)
        {
            if (!(*iter)->mCompiled)
            {
                (*iter)->Compile(offset);
            }
            else {
                (*iter)->mOffset = offset;
            }
            range += (*iter)->mByteSize;
            offset += (*iter)->mByteSize;

        }
        mByteSize = range;
        mCompiled = true;
        return true;
    }

    void Print(int incident = 0) override
    {
       // printf("%*s", incident * 4, " ");
        //std::cout << " mSematic " << mSematic << " mOffset " << mOffset << " mByteSize " << mByteSize << std::endl;
        for (auto iter = mSubUniformList.begin(); iter != mSubUniformList.end(); iter++)
        {
            if ((*iter)->mCompiled)
            {
                (*iter)->Print(incident + 1);
            }
        }
    }
protected:

    //instance data
    // 这里好像只能用继承  不能用模板
       // 这里不希望结构体存储具体的数据T 而只存储地址T* 因为在constantBuffer中所有的数据合并成一个长Buffer
    std::vector<Ptr<UniformBase>> mSubUniformList;
    
};


//typedef Uniform<int, ElementType::ET_INT> UniformInt;
//typedef Uniform<float, ElementType::ET_INT> UniformFloat;
//typedef Uniform<double, ElementType::ET_INT> UniformDouble;
//typedef Uniform<Mat4, ElementType::ET_INT> UniformMatrix4f;
//typedef Uniform<Vec4f, ElementType::ET_INT> UniformVec4f;
//typedef Uniform<Vec3f, ElementType::ET_INT> UniformVec3f;
using UniformInt = Uniform<int, ElementType::ET_INT>;
using UniformFloat = Uniform<float, ElementType::ET_FLOAT>;
using UniformDouble = Uniform<double, ElementType::ET_DOUBLE>;
using UniformMatrix4f = Uniform<Mat4, ElementType::MATRIX4F>;
using UniformVec4f = Uniform<Vec4f, ElementType::VEC4F>;
using UniformVec3f = Uniform<Vec3f, ElementType::VEC3F>;


//template<class T>
//class UniformArray :public UniformBase
//{
//public:
//    UniformArray(std::string sematic, std::string type) :UniformBase(sematic, type) {}  // default value
//    UniformArray(std::string sematic, std::string type, size_t maxNum) :UniformBase(sematic, type), mElementMaxNum(maxNum) {
//
//        mElementByteSize = sizeof(T);
//        mOffset = 0;
//        mByteSize = mElementByteSize * maxNum;
//        mCompiled = true;
//        mArray = new TemplateArray<T, ElementType::CUSTOM, mElementByteSize>();
//    }
//    ~UniformArray() {}
//    uint32_t mElementByteSize;
//    size_t mElementMaxNum;
//
//    uint32_t AddElement(const T& data)
//    {
//
//    }
//    ArrayPtr mArray;
//
//    template<class T>
//    void ModifyElement(uint32_t index, const T& data) {
//
//    }
//
//    bool Compile(uint32_t offset = 0) override
//    {
//        if (!mCompiled)
//        {
//            mArray->mElementNum
//        }
//    }
//    virtual ~UniformArray() {}
//    virtual void Print(int incident = 0) = 0;
//};
class Data {

};
using UniformDefault = Uniform<Data,ElementType::STRUCT>;
using UniformBasePtr = Ptr<UniformBase>;

