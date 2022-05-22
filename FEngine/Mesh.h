#pragma once

#include"Material.h"
#include"Buffer.h"
class AAbb {

};

class FMesh
{
public:
	FMesh() = default;

	FBufferAssetView* indicesAssetView;
	FBufferAssetView* verticesAssetView;

	AAbb m_aabb;

};

