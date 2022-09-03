

#include"GltfReader.h"
#include"Material.h"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE

#include<tiny_gltf.h>
GltfReader::GltfReader()
{
}
bool GltfReader::loadAssets(const std::string& path, GLTFModel& model)
{
	loadglTFFile(path, model);
	return true;
}
bool GltfReader::loadglTFFile(const std::string& filename, GLTFModel& model)
{
	tinygltf::Model glTFInput;
	tinygltf::TinyGLTF gltfContext;
	std::string error, warning;

	bool fileLoaded = gltfContext.LoadASCIIFromFile(&glTFInput, &error, &warning, filename);
	if (fileLoaded) {
		loadImages(glTFInput, model);
		loadMaterials(glTFInput, model);
		loadTextures(glTFInput, model);

		const tinygltf::Scene& scene = glTFInput.scenes[0];
		for (size_t i = 0; i < scene.nodes.size(); i++) {
			const tinygltf::Node node = glTFInput.nodes[scene.nodes[i]];
			loadNode(node, glTFInput, model);
		}
	}
	else {
		//std::count<<("Could not open the glTF file.\n\nThe file is part of the additional asset pack.\n\nRun \"download_assets.py\" in the repository root to download the latest version.", -1);
		
	}
	return true;
}

bool GltfReader::loadImages(const tinygltf::Model& glTFInput, GLTFModel& model)
{
	model.images.reserve(glTFInput.images.size());
	for (size_t i = 0; i < glTFInput.images.size(); i++) {
		model.images.push_back(
			GLTFModel::Image{ glTFInput.images[i].uri,
			glTFInput.images[i].width, glTFInput.images[i].height,false });	    
	}
	return false;
}

bool CompileMaterial(GLTFModel& model)
{
	//FGeometry* geom = dynamic_cast<FGeometry*>(model.node.get());

	//PbrMaterialMetalRoughness meterial;
	//tinygltf::Material glTFMaterial = glTFInput.materials[i];
	return false;


}
bool GltfReader::loadMaterials(const tinygltf::Model& glTFInput, GLTFModel& model)
{

	model.materials.reserve(glTFInput.materials.size());

	for (size_t i = 0; i < glTFInput.materials.size(); i++) {
		// We only read the most basic properties required for our sample
		tinygltf::Material glTFMaterial = glTFInput.materials[i];
		//GLTFModel::Material material;

		MaterialPtr material = make_shared<PbrMaterialMetalRoughness>();
		MaterialMapPtr materialMap = make_shared<MaterialMap>();

		int32_t slot = MaterialBuilder::Instance().CreateResouce(material);
		int32_t mapSlot = MaterialMapBuilder::Instance().CreateResouce(materialMap);

		int32_t index = 0;

		// pbr metalRoughness
		material->baseColorFactor = glTFMaterial.pbrMetallicRoughness.baseColorFactor;
		materialMap->baseColorMap.uri = model.images[glTFMaterial.pbrMetallicRoughness.baseColorTexture.index].uri;
		
		material->metallicFactor = static_cast<float>(glTFMaterial.pbrMetallicRoughness.metallicFactor);
		material->roughnessFactor = static_cast<float>(glTFMaterial.pbrMetallicRoughness.roughnessFactor);
		materialMap->metalRoughnessMap.uri = model.images[glTFMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index].uri;
		// normal
		materialMap->normalMap.uri = model.images[glTFMaterial.normalTexture.index].uri;

		// alphaMask
		material->alphaMaskCutoff = static_cast<float>(glTFMaterial.alphaCutoff);

		// emissive
		material->emissiveFactor = Vec3f(static_cast<float>(glTFMaterial.emissiveFactor[0]), static_cast<float>(glTFMaterial.emissiveFactor[1]),
			static_cast<float>(glTFMaterial.emissiveFactor[2]));
		materialMap->emissiveMap.uri = model.images[glTFMaterial.emissiveTexture.index].uri;
		
		// ao
		material->aoStrength = static_cast<float>(glTFMaterial.occlusionTexture.strength);
		materialMap->aoMap.uri = model.images[glTFMaterial.occlusionTexture.index].uri;

		model.materials.push_back({ slot,mapSlot });
	
	}
	return true;
}
bool GltfReader::loadTextures(const tinygltf::Model& glTFInput, GLTFModel& model)
{
	
	model.textures.resize(glTFInput.textures.size());
	for (size_t i = 0; i < glTFInput.textures.size(); i++) {
		model.textures[i].imageIndex = glTFInput.textures[i].source;
	}
	return false;
}
bool GltfReader::loadNode(const tinygltf::Node& inputNode, const tinygltf::Model& input, GLTFModel& model)
{

	Ptr<FNode> node = new FGeometry(inputNode.name);


	if (inputNode.translation.size() == 3) {
		node->SetPosition(Vec3f(inputNode.translation[0], inputNode.translation[1], inputNode.translation[2]));
	}
	if (inputNode.rotation.size() == 4) {
		node->SetRotation(Quatf(inputNode.rotation[0], inputNode.rotation[1], inputNode.rotation[2], inputNode.rotation[3]));
	}
	if (inputNode.scale.size() == 3) {
		node->SetScale(Vec3f(inputNode.scale[0], inputNode.scale[1], inputNode.scale[2]));
	}
	if (inputNode.matrix.size() == 16) {
		Mat4 mat(inputNode.matrix);
		node->SetLocalTransform(mat);
	};


	// Load node's children
	if (inputNode.children.size() > 0) {
		for (size_t i = 0; i < inputNode.children.size(); i++) {
			//loadNode(input.nodes[inputNode.children[i]], input, &node, indexBuffer, vertexBuffer);
		}
	}
	FGeometry* geom = dynamic_cast<FGeometry*>(node.get());
	// If the node contains mesh data, we load vertices and indices from the buffers
	// In glTF this is done via accessors and buffer views
	if (inputNode.mesh > -1) {
		const tinygltf::Mesh mesh = input.meshes[inputNode.mesh];
		// Iterate through all primitives of this node's mesh
		/*for (size_t i = 0; i < mesh.primitives.size(); i++) {*/
		const tinygltf::Primitive& glTFPrimitive = mesh.primitives[0];
		// current support one mesh one primitive !!!
		uint32_t firstIndex = static_cast<uint32_t>(0);
		uint32_t vertexStart = static_cast<uint32_t>(0);
		uint32_t indexCount = 0;

		// Vertices
		{
			const float* positionBuffer = nullptr;
			const float* normalsBuffer = nullptr;
			const float* texCoordsBuffer = nullptr;
			size_t vertexCount = 0;

			// Get buffer data for vertex normals
			if (glTFPrimitive.attributes.find("POSITION") != glTFPrimitive.attributes.end()) {
				const tinygltf::Accessor& accessor = input.accessors[glTFPrimitive.attributes.find("POSITION")->second];
				const tinygltf::BufferView& view = input.bufferViews[accessor.bufferView];
				positionBuffer = reinterpret_cast<const float*>(&(input.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
				vertexCount = accessor.count;
			}
			// Get buffer data for vertex normals
			if (glTFPrimitive.attributes.find("NORMAL") != glTFPrimitive.attributes.end()) {
				const tinygltf::Accessor& accessor = input.accessors[glTFPrimitive.attributes.find("NORMAL")->second];
				const tinygltf::BufferView& view = input.bufferViews[accessor.bufferView];
				normalsBuffer = reinterpret_cast<const float*>(&(input.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
			}
			// Get buffer data for vertex texture coordinates
			// glTF supports multiple sets, we only load the first one
			if (glTFPrimitive.attributes.find("TEXCOORD_0") != glTFPrimitive.attributes.end()) {
				
				const tinygltf::Accessor& accessor = input.accessors[glTFPrimitive.attributes.find("TEXCOORD_0")->second];
				const tinygltf::BufferView& view = input.bufferViews[accessor.bufferView];
				texCoordsBuffer = reinterpret_cast<const float*>(&(input.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
			}

			Vec3fArrayPtr arrayPos, arrayNor;

			arrayPos = new Vec3fArray();
			arrayNor = new Vec3fArray();
			Ptr<Vec2fArray> arrayTex0 = new Vec2fArray();

			for (size_t v = 0; v < vertexCount; v++)
			{
				arrayPos->push_back(Vec3f(positionBuffer[v * 3], positionBuffer[v * 3 + 1], positionBuffer[v * 3 + 2]));
				arrayNor->push_back(Vec3f(normalsBuffer[v * 3], normalsBuffer[v * 3 + 1], normalsBuffer[v * 3 + 2]));
				arrayTex0->push_back(Vec2f(texCoordsBuffer[v * 2], texCoordsBuffer[v * 2 + 1]));
			}
			geom->SetVertexPositionArray(arrayPos.get());
			geom->SetVertexNormalArray(arrayNor.get());
			geom->SetVertexTexcoordArray(arrayTex0.get());
		}
		// Indices
		{
			const tinygltf::Accessor& accessor = input.accessors[glTFPrimitive.indices];
			const tinygltf::BufferView& bufferView = input.bufferViews[accessor.bufferView];
			const tinygltf::Buffer& buffer = input.buffers[bufferView.buffer];

			indexCount += static_cast<uint32_t>(accessor.count);
			Ptr<IndexArray> indexBuffer = new IndexArray();
			// glTF supports different component types of indices
			switch (accessor.componentType) {
			case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT: {
				const uint32_t* buf = reinterpret_cast<const uint32_t*>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
				for (size_t index = 0; index < accessor.count; index++) {
					indexBuffer->push_back(buf[index] + vertexStart);
				}
				break;
			}
			case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {
				const uint16_t* buf = reinterpret_cast<const uint16_t*>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
				for (size_t index = 0; index < accessor.count; index++) {
					indexBuffer->push_back(buf[index] + vertexStart);
				}
				break;
			}
			case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE: {
				const uint8_t* buf = reinterpret_cast<const uint8_t*>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
				for (size_t index = 0; index < accessor.count; index++) {
					indexBuffer->push_back(buf[index] + vertexStart);
				}
				break;
			}
			default:
				std::cerr << "Index component type " << accessor.componentType << " not supported!" << std::endl;
				return false;
			}
			geom->SetIndexArray(indexBuffer.get());
		}
		geom->materialSlot =model.materials[glTFPrimitive.material].materialSlot;
		geom->materialMapSlot = model.materials[glTFPrimitive.material].mapSlot;
	}
	
	geom->Compile();

	model.node = node;

	return true;
}
