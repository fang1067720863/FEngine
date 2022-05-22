#pragma once

#include"FDx11.h"
#include"Texture.h"


class FDx11Texture: Texture
{
    
    /// constructor 
    FDx11Texture(const std::string& name,
        const std::string& group, bool isManual,
        ID3D11Device* device)
    {
        switch (GetTextureType())
        {
        case TextureType::TEXTURE_1D:
        {
            this->_create1DTex();
            break; // For Feature levels that do not support 1D textures, revert to creating a 2D texture.
        }
        case TextureType::TEXTURE_2D:
        case TextureType::TEXTURE_CUBE:
        case TextureType::TEXTURE_2D_ARRAY:
            this->_create2DTex();
            break;
        case TextureType::TEXTURE_3D:
            this->_create3DTex();
            break;
        default:
            return;
            /* this->freeInternalResources();
             OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "Unknown texture type", "D3D11Texture::createInternalResources");*/
        }
    }
    /// destructor
    ~FDx11Texture(){}


    ID3D11Resource* GetTextureResource() { assert(mpTex); return mpTex.Get(); }
    ID3D11ShaderResourceView* GetSrvView() { assert(mpShaderResourceView); return mpShaderResourceView.Get(); }
    D3D11_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc() const { return mSRVDesc; }

    ID3D11Texture1D* GetTex1D() { return mp1DTex.Get(); };
    ID3D11Texture2D* GetTex2D() { return mp2DTex.Get(); };
    ID3D11Texture3D* GetTex3D() { return mp3DTex.Get(); };

    bool HasAutoMipMapGenerationEnabled() const { return mAutoMipMapGeneration; }

protected:
    /*TextureUsage _getTextureUsage() { return static_cast<TextureUsage>(mUsage); }*/

protected:
    template<typename fromtype, typename totype>

    void _create1DResourceView(){}
    void _create1DTex() {}
    void _create2DResourceView(){
        HRESULT hr;
        D3D11_TEXTURE2D_DESC desc;
        mp2DTex->GetDesc(&desc);
        mNumMipmaps = desc.MipLevels - 1;

        ZeroMemory(&mSRVDesc, sizeof(mSRVDesc));
        mSRVDesc.Format = desc.Format == DXGI_FORMAT_R32_TYPELESS ? DXGI_FORMAT_R32_FLOAT : desc.Format;

        switch (GetTextureType())
        {
        case TEXTURE_1D:
            mSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
            mSRVDesc.TextureCube.MipLevels = desc.MipLevels;
            mSRVDesc.TextureCube.MostDetailedMip = 0;
            break;

        case TEXTURE_2D:
            if (mFSAAType.Count > 1)
            {
                mSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
                mSRVDesc.Texture2DMSArray.FirstArraySlice = 0;
                mSRVDesc.Texture2DMSArray.ArraySize = desc.ArraySize;
            }
            else
            {
                mSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
                mSRVDesc.Texture2DArray.MostDetailedMip = 0;
                mSRVDesc.Texture2DArray.MipLevels = desc.MipLevels;
                mSRVDesc.Texture2DArray.FirstArraySlice = 0;
                mSRVDesc.Texture2DArray.ArraySize = desc.ArraySize;
            }
            break;

        case TEXTURE_CUBE:
        case TEXTURE_3D:  // For Feature levels that do not support 1D textures, revert to creating a 2D texture.
            if (mFSAAType.Count > 1)
            {
                mSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
            }
            else
            {
                mSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                mSRVDesc.Texture2D.MostDetailedMip = 0;
                mSRVDesc.Texture2D.MipLevels = desc.MipLevels;
            }
            break;
        }
        device->CreateShaderResourceView(mp2DTex.Get(), &mSRVDesc,
            mpShaderResourceView.ReleaseAndGetAddressOf());
    }
    void _create2DTex()
    {
        assert(mWidth > 0 || mHeight > 0);

        // calc mips
        UINT numMips = (mNumRequestedMipmaps == 8 || (1U << mNumRequestedMipmaps) > std::max(mWidth, mHeight)) ? 0 : mNumRequestedMipmaps + 1;

        D3D11_TEXTURE2D_DESC desc;
        desc.Width = static_cast<UINT>(mWidth);
        desc.Height = static_cast<UINT>(mHeight);
        desc.MipLevels = numMips;
        desc.ArraySize = mDepth == 0 ? 1 : mDepth;
        desc.Format = mD3DFormat;
        desc.SampleDesc = mFSAAType;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        if (this->GetTextureType() == Texture::TextureType::TEXTURE_CUBE)
        {
            desc.ArraySize = 6;
        }
        desc.MipLevels = 1;
    }
    void _create3DTex() {}
    void _create3DResourceView() {}





    //void createInternalResourcesImpl(void);
    //void freeInternalResourcesImpl(void);
    //void _setSrcAttributes(unsigned long width, unsigned long height, unsigned long depth, PixelFormat format);
    //void _setFinalAttributes(unsigned long width, unsigned long height, unsigned long depth, PixelFormat format, UINT miscflags);
    //void _createSurfaceList(void);
    //void notifyDeviceLost(D3D11Device* device);
    //void notifyDeviceRestored(D3D11Device* device);

    //void loadImpl();

protected:
    ID3D11Device* device;

    DXGI_FORMAT mD3DFormat;         
    DXGI_SAMPLE_DESC mFSAAType;     

    ComPtr<ID3D11Resource> mpTex;   
    ComPtr<ID3D11ShaderResourceView> mpShaderResourceView;
    ComPtr<ID3D11Texture1D> mp1DTex;
    ComPtr<ID3D11Texture2D> mp2DTex;
    ComPtr<ID3D11Texture3D> mp3DTex;

    D3D11_SHADER_RESOURCE_VIEW_DESC mSRVDesc;
    bool mAutoMipMapGeneration;
};