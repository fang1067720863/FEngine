//#pragma once
//
//#include<string>
//#include<Asset.h>
//#include"CommonType.h"
//
//
//class FImageDescriptor
//{
//    enum ImageDimension
//    {
//        Image1D,
//        Image2D,
//        Image3D
//    };
//    ImageDimension m_dimension = ImageDimension::Image2D;
//
//    /// Size of the image in pixels.
//    struct Size {
//        uint32_t width;
//        uint32_t height;
//        uint32_t depth;
//    };
//    Size size;
//
//    uint16_t mipLevels = 1;
//    GraphicFormat pixelformat = GraphicFormat::Unknown;
//    uint32_t isCubemap = 0;
//};
//
//
//// creator创建要返回类型
//class FImageAsset :public FAssetData
//{
//public:
//    const FImageDescriptor& GetImageDescriptor() {}
//protected:
//    FImageDescriptor imageDescriptor;
//};
//
//
//class FStreamingImageAsset :public FAssetData
//{
//public:
//    virtual ~FStreamingImageAsset() { DeallocateData(); };
//    uint16_t GetMipLevelCount() const { return m_mipLevels; }
//
//protected:
//
//
//private:
//
//    void Init() {};
//    void SetReady() {};
//    void DeallocateData() { delete[] imageData; }
//
//    uint16_t m_mipLevels = 0;
//    uint16_t m_arraySize = 0;
//    unsigned char* imageData;
//    std::vector<uint16_t> subImageDataOffsets;
//
//
//    unsigned char* GetSubImageData(uint32_t subImageIndex, uint16_t& size) const
//    {
//        size = subImageDataOffsets[subImageIndex + 1] - subImageDataOffsets[subImageIndex];
//        return imageData + subImageDataOffsets[subImageIndex];
//    }
//};
//
//class FStreamingImageAssetCreator :public FAssetCreator<FStreamingImageAsset>
//{
//public:
//    FStreamingImageAssetCreator() = default;
//    void Begin(const AssetId& assetId, uint16_t mipLevels, uint16_t arraySize) {};
//    void AddSubImage(const void* data, size_t dataSize) {};
//    void EndMip() {};
//    bool End(FAsset<FStreamingImageAsset>& result) {}
//
//private:
//    bool IsBuildingMip() const {}
//    bool ValidateIsBuildingMip() {}
//
//    uint16_t m_mipLevelsPending = 0;
//    uint16_t m_mipLevelsCompleted = 0;
//    uint16_t m_arraySlicesCompleted = 0;
//    uint16_t m_subImageOffset = 0;
//};
//
//
//class FImage
//{
//
//};
//
//class FImageView
//{
//
//};