#pragma once
#include<string>
struct  TextureProfile
{

    unsigned int    target;
    unsigned int     numMipmapLevels;
    //GLenum    internalFormat,
    unsigned int   width;
    unsigned int   height;
    unsigned int   depth;
    unsigned int     border;
    unsigned int     minFilter;
    unsigned int     magFilter;
    unsigned int     wrapS;
    unsigned int     wrapT;
    unsigned int     wrapR;
    float   maxAnisotropy;
};

enum class TextureType : uint8_t
{
    TEXTURE_1D,
    TEXTURE_2D,
    TEXTURE_3D,
    TEXTURE_CUBE,
    TEXTURE_2D_ARRAY = 5,
};
class Texture
{
public:


    uint32_t mHeight;
    uint32_t mWidth;
    uint32_t mDepth;
    uint32_t mNumRequestedMipmaps;
    uint32_t mNumMipmaps;
    TextureType mType;
    TextureType GetTextureType() const { return mType; }
};