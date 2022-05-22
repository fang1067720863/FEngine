#pragma once
#include<string>

typedef uint32_t AssetId;

class FAssetData
{

};

class FImageAsset:public FAssetData
{

};

class FMaterialAsset :public FAssetData
{

};
class FModelAsset :public FAssetData
{

};



template<typename AssetDataT>
class FAsset
{

};

template<typename AssetDataT>
class FAssetCreator
{
public:
protected:
	FAssetCreator() {};
	virtual ~FAssetCreator() = default;
	FAsset<AssetDataT> m_asset;
};