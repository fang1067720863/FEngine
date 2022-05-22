#pragma once

#include"CommonType.h"
#include"Reference.h"
#include"BufferDescription.h"
#include<functional>


class BufferObject
{
public:
	
	const BufferDescription GetBufferDescription()const { return bufferDescription; }
protected:
	BufferDescription bufferDescription;
    BufferType bufferType;
};




class BufferData
{
   using ModifiedCallback =
        const std::function<void(const std::string&, const std::vector<uint32_t>&)>;
protected:
    bool dirty;
    ModifiedCallback modifyCallback;
    
};

struct LightPassConstant
{
    //GRiDirectionalLight dirLight[MAX_DIRECTIONAL_LIGHT_NUM];
    //GRiPointLight pointLight[MAX_POINT_LIGHT_NUM];
    //DirectX::XMFLOAT3 cameraPosition;
    int pointLightCount;
    int dirLightCount;
};

template <typename T>
class BufferTemplate : public BufferData
{
public:
    BufferTemplate() :
        BufferData(),
        _data(T())
    {}

protected:
    virtual ~BufferTemplate() {};

private:
    T _data;
};
