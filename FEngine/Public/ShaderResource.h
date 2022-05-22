#pragma once
#pragma once
#pragma once

#include"ShaderInput.h"
#include<any>
#include"Image.h"
#include"Buffer.h"
#include"Shader.h"
#include"Ptr.h"


class SamplerState;
class ImageView;

class ShaderResourceGroupLayout;
class ShaderResourceGroup
{
public:
    template <typename T>
    bool SetConstantArray(ShaderInputConstantIndex& inputIndex, const T values) {}
    // bool float vec2 vec3 vec4 mat4
    template <typename T>
    bool SetConstant(ShaderInputConstantIndex& inputIndex, const T& value, uint32_t arrayIndex) {}

    bool SetImageView(ShaderInputImageIndex inputIndex, const ImageView* imageView, uint32_t arrayIndex = 0) {}
    bool SetImageViewArray(ShaderInputImageIndex& inputIndex, const ImageView* imageViews, uint32_t arrayIndex = 0) {}

    bool SetSampler(ShaderInputSamplerIndex inputIndex, const SamplerState& sampler, uint32_t arrayIndex = 0) {}
    bool SetSamplerArray(ShaderInputSamplerIndex inputIndex, const SamplerState samplers, uint32_t arrayIndex = 0) {}

    ShaderInputBufferIndex    FindShaderInputBufferIndex(const std::string& name) const {}
    ShaderInputImageIndex     FindShaderInputImageIndex(const std::string& name) const {}
    ShaderInputSamplerIndex   FindShaderInputSamplerIndex(const std::string& name) const {}
    ShaderInputConstantIndex  FindShaderInputConstantIndex(const std::string& name) const {}

private:
private:
    template<class T>
    using Ptr = FPtr<T>;

    ShaderResourceGroup() = default;

    static ShaderResourceGroup* CreateInternal(FShaderAsset& shaderAsset) {}

    bool m_isInitialized = false;
   // ShaderResourceGroupData m_data;
    Ptr<ShaderResourceGroup> m_shaderResourceGroup;
    //FAsset<FShaderAsset> m_asset;
    const ShaderResourceGroupLayout* m_layout = nullptr;
    std::vector<Ptr<FImage>> m_imageGroup;
};

class ShaderCollection
{

};