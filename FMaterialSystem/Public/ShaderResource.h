#pragma once
#pragma once
#include"Common.h"
#include"ShaderInput.h"
#include<any>


class SamplerState;
class ImageView;

class ShaderResourceGroup
{
public:
    // 这里需要知道T的size 
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
    ShaderResourceGroup() = default;

    //static ShaderResourceGroup* CreateInternal(ShaderAsset& shaderAsset) {}

    bool m_isInitialized = false;

    /// Pool for allocating RHI::ShaderResourceGroup objects
    Data::Instance<ShaderResourceGroupPool> m_pool;

    /// The shader resource group data that is manipulated by this class
    RHI::ShaderResourceGroupData m_data;

    /// The shader resource group that can be submitted to the renderer
    RHI::Ptr<RHI::ShaderResourceGroup> m_shaderResourceGroup;

    /// A reference to the SRG asset used to initialize and manipulate this group.
    Asset<ShaderAsset> m_asset;

    /// A pointer to the layout inside of m_srgAsset
    const RHI::ShaderResourceGroupLayout* m_layout = nullptr;

    /**
     * The set of images currently bound. The shader resource group maintains these references to
     * keep the hardware resource in memory, manage streaming operations, and support reload operations.
     * However, entries remain null when RHI image views are bound.
     */
    AZStd::vector<Data::Instance<Image>> m_imageGroup;

    /**
     * The set of buffers currently bound. The shader resource group maintains these references to
     * keep the hardware resource in memory, manage streaming operations, and support reload operations.
     * However, entries remain null when RHI buffer views are bound.
     */
    AZStd::vector<Data::Instance<Buffer>> m_bufferGroup;
};

class ShaderCollection
{

};