#pragma once
#include<map>
#include<unordered_map>
#include"Factory.h"


struct PhoneMaterial
{

};
struct PbrMaterialMetalRoughness
{
	Vec4f baseColorFactor{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vec3f emissiveFactor{ 0.0f, 0.0f, 0.0f };
	float metallicFactor{ 1.0f };
	float roughnessFactor{ 1.0f };
	float alphaMask{ 1.0f };
	float alphaMaskCutoff{ 0.5f };
	float aoStrength{ 0.0f };
};

struct MaterialMap
{
	struct Map
	{
		std::string uri;

	};
	Map baseColorMap;
	Map metalRoughnessMap;
	Map aoMap;
	Map normalMap;
	Map emissiveMap;
};

typedef int32_t MaterialSlot;
typedef shared_ptr<PbrMaterialMetalRoughness> MaterialPtr;
typedef int32_t MaterialMapSlot;
typedef shared_ptr<MaterialMap> MaterialMapPtr;




class MaterialBuilder : public UintResourceFactory< MaterialPtr, MaterialBuilder>
{

};

class MaterialMapBuilder : public UintResourceFactory< MaterialMapPtr, MaterialMapBuilder>
{

};
