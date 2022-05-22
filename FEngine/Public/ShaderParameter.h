#pragma once

#include<string>
#include<vector>
#include<memory>
#include"Vec.h"
#include"Matrix.h"
#include"VecArray.h"
#include"Reference.h"

enum Semantic
{
    POSITION,
    NORMAL,
    COLOR,
    TEXTURE_COORDINATES,

    BINORMAL,
    TANGENT,

    BLEND_WEIGHTS,
    BLEND_INDICES,
};
enum class GpuContantType : uint8_t
{
    GCT_FLOAT1,
    GCT_FLOAT2,
    GCT_FLOAT3,
    GCT_FLOAT4,
    GCT_SAMPLER1D,
    GCT_SAMPLER2D,
    GCT_SAMPLER3D,
    GCT_SAMPLERCUBE ,
    GCT_SAMPLER1DSHADOW ,
    GCT_SAMPLER2DSHADOW ,
    GCT_SAMPLER2DARRAY ,
    GCT_SAMPLER_EXTERNAL_OES,
    GCT_MATRIXF_4X4,
    GCT_COUNT
};


class UniformBase: public FReference
{

public:
    UniformBase(std::string sematic, std::string type) :mSematic(sematic), mType(type) {}
    virtual void AddSubUniform(std::shared_ptr<UniformBase> uniform) = 0;
public:
    // meta
    std::string mSematic;
    std::string mType;

    uint32_t mOffset;
    bool mCompiled = false;
    uint32_t mByteSize;

    virtual bool Compile(uint32_t offset = 0) = 0;
    virtual ~UniformBase() {}
    virtual void Print(int incident = 0) = 0;
    virtual unsigned char* GetDataPtr() { return nullptr; }


};
using UniformBasePtr = Ptr<UniformBase>;

template<class T>
class Uniform :public UniformBase
{
public:
    // Basic type
    Uniform(std::string sematic, std::string type) :UniformBase(sematic, type) {}  // default value
    Uniform(std::string sematic, std::string type, T data) :UniformBase(sematic, type) {

        mByteSize = sizeof(T);
        mOffset = 0;
        mCompiled = true;
    }
    ~Uniform() {}

    void AddSubUniform(UniformBasePtr uniform)override {
        mSubUniformList.push_back(uniform);
    }


    template<class U>
    void ModifySubUniform(std::string Sematic, uint32_t index, U data) {}
    uint32_t GetByteSize() { return mByteSize; }
   

protected:

    //instance data
    T data;     // 这里不希望结构体存储具体的数据T 而只存储地址T* 因为在constantBuffer中所有的数据合并成一个长Buffer
    std::vector<UniformBasePtr> mSubUniformList;
    // 这里好像只能用继承  不能用模板
    uint32_t mIndex;

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
        printf("%*s", incident * 4, " ");
        std::cout << " mSematic " << mSematic << " mOffset " << mOffset << " mByteSize " << mByteSize << std::endl;
        for (auto iter = mSubUniformList.begin(); iter != mSubUniformList.end(); iter++)
        {
            if ((*iter)->mCompiled)
            {
                (*iter)->Print(incident + 1);
            }
        }
    }
};

using UniformInt = Uniform<int>;
using UniformFloat = Uniform<float>;
using UniformDouble = Uniform<double>;
using UniformMatrix4f = Uniform<Mat4>;
using UniformVec4f = Uniform<Vec4f>;
using UniformVec3f = Uniform<Vec3f>;


template<class T>
class UniformArray :public UniformBase
{
public:
    UniformArray(std::string sematic, std::string type) :UniformBase(sematic, type) {}  // default value
    UniformArray(std::string sematic, std::string type, size_t maxNum) :UniformBase(sematic, type) : mElementMaxNum(elementNum) {

        mElementByteSize = sizeof(T);
        mOffset = 0;
        mByteSize = mElementByteSize * maxNum；
        mCompiled = true;
        mArray = new TemplateArray<T, ElementType::CUSTOM, mElementByteSize>();
    }
    ~UniformArray() {}
    uint32_t mElementByteSize;
    size_t mElementMaxNum;

    uint32_t AddElement(const T& data)
    {

    }
    ArrayPtr mArray;

    template<class T>
    void ModifyElement(uint32_t index, const T& data) {

    }
};
class Data {

};
using UniformDefault = Uniform<Data>;



//template<class T>
//class UniformArray :class Uniform
//{
//    using ArrayType = std::vector<T> ;
//public:
//    UniformArray() :mType("array")
//    {
//        
//
//    }
//
//    ArrayType arrayData;
//    
//};