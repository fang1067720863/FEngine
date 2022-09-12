//#pragma once
#ifndef OSGEARTH_GLTF_READER_H
#define OSGEARTH_GLTF_READER_H
#include <Windows.h>
#include"Group.h"
#include"Geometry.h"


struct GLTFModel
{

	struct Material
	{
		MaterialMapSlot mapSlot;
		MaterialSlot materialSlot;
	};

	struct Texture
	{
		uint32_t imageIndex;
	};

	struct Image
	{
		std::string uri;
		int32_t w, h;
		bool mipMaps;
	};

	vector<Material> materials;
	vector<Texture> textures;
	vector<Image> images;
	Ptr<FNode> node{};
};

namespace tinygltf {
	class Model;
	class Node;
}
class GltfReader {
public:
	GltfReader();

	bool loadAssets(const std::string& path, GLTFModel& model);
	
    
	// scene -> nodes (matrix) -> meshes (buffers) -> primitives
	bool loadglTFFile(const std::string& filename, GLTFModel& model);
	
protected:
	bool loadImages(const tinygltf::Model& glTFInput, GLTFModel& model);
	bool loadMaterials(const tinygltf::Model& glTFInput, GLTFModel& model);
	bool loadTextures(const tinygltf::Model& glTFInput, GLTFModel& model);
	bool loadNode(const tinygltf::Node& inputNode, const tinygltf::Model& input, GLTFModel& model);

	File modelFile;
};
#endif

