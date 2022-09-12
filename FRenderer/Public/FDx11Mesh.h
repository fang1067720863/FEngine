#pragma once


#include"FDx11BufferObject.h"
#include"FDx11InpuyLayout.h"
#include"FDx11RenderState.h".h"
#include"FDx11ResourceFactory.h"

//#include"Material.h"
#include"VecArray.h"
#include"Ptr.h"
#include"Geometry.h"
#include"Node.h"
#include"UtfConverter.h"
#include"Constant.h"
struct Primitive
{
	uint32_t firstIndex = 0;
	uint32_t indexCount = 0;
	uint32_t materialSlot = -1;
};
struct Material
{
	Vec4f Ambient;
	Vec4f Diffuse;
	Vec4f Specular;
	Vec4f Reflect;
};
class FMesh :public FReference
{
	enum class Topology : uint16_t
	{

	};
};
class FDx11Mesh:public FNode
{
public:
	friend class FMeshBuilder;
	//typedef std::shared_ptr<Material> MaterialPtr;
	typedef Ptr<VertexBufferObject> VBOPtr;
	typedef Ptr<IndexBufferObject> IBOPtr;
	

	FDx11Mesh(FGeometry* geom, const FDx11Device& _device, bool useIndex = true):FNode(""),device(_device),mUseIndex(useIndex) {
		Init(geom);
	}

	D3D_PRIMITIVE_TOPOLOGY GetTopology() { return mTopology; }
	VertexBufferObject* GetVBO() { return mVBO.get(); }
	IndexBufferObject* GetIBO() { return mIBO.get(); }
	std::vector<Primitive> mPrimitives;
	bool UseIndex() { return mUseIndex; }
	bool mUseIndex;
	void Init(FGeometry* geom)
	{
		mGeomtry = geom;
		// Init Dx11 InputLayout&Topology
		//mInputLayout = new FDx11VertexInputLayout(geom->GetVertexElementType(),device);
		// magic 
		mTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		// Init Dx11 VBO IBO
		mVBO = new VertexBufferObject(geom->GetCompiledVertexDataSize(), geom->GetCompiledVertexData(), device, geom->GetVertexStride(),geom->GetVertexELementNum());
		if (mUseIndex)
		{
			mIBO = new IndexBufferObject(geom->GetCompiledIndexDataSize(), geom->GetCompiledIndexData(), device, geom->GetIndexELementNum());
		}
		switch (geom->GetMaterialType())
		{
		case MaterialType::None:
			break;
		case MaterialType::Pbr:
			InitPbrMaterial();
			break;
		case MaterialType::Phong:
			InitNormalMaterial();
			break;
		case MaterialType::SkyBox:
			InitSkyBoxMaterial();
			break;
		default:
			break;
		}
		
		
		
	}
	struct MaterialSlot
	{
		struct UniformSlot
		{
			uint32_t slotId;
			bool vsStage{ true };
			bool psStage{ true };
			uint32_t num{ 1 };
			std::string bufferSlot;
		};
		struct TextureSlot
		{
			int32_t slotId{ -1 };
			uint32_t num{ 1 };
			int32_t resourceSlot;
		};
		struct SamplerSlot
		{
			int32_t slotId{ -1 };
			uint32_t num{ 1 };
			int32_t samplerSlot;
		};
		vector<UniformSlot> uniformSlots;
		vector<TextureSlot> textureSlots;
		vector<SamplerSlot> samplerSlots;

	};
	

	void PreDraw()
	{
		// set model matrix
		
		Ptr<ConstantBufferObject> buffer = ConstantBufferPool::Instance().GetResource("world");
		buffer->Upload<CBEveryObject>(CBEveryObject{ this->GetLocalTransform() });
		
		
		// set Material Params
		for (auto uniformSlot : mMaterialSlot.uniformSlots)
		{
			auto buffer = ConstantBufferPool::Instance().GetResource(uniformSlot.bufferSlot);
			ID3D11Buffer* bufferArray[1];
			bufferArray[0] = buffer->GetBufferView();
			if (uniformSlot.vsStage)
			{
				device.GetDeviceContext()->VSSetConstantBuffers(uniformSlot.slotId, uniformSlot.num, bufferArray);
			}
			if (uniformSlot.psStage)
			{
				device.GetDeviceContext()->PSSetConstantBuffers(uniformSlot.slotId, uniformSlot.num, bufferArray);
			}
		}
		for (auto textureSlot : mMaterialSlot.textureSlots)
		{
			auto texture = ShaderResoucePool::Instance().GetResource(textureSlot.resourceSlot);
			ID3D11ShaderResourceView* srArray[1];
			srArray[0] = texture.Get();
			device.GetDeviceContext()->PSSetShaderResources(textureSlot.slotId, textureSlot.num, srArray);
		}
		for (auto samplerSlot : mMaterialSlot.samplerSlots)
		{
			auto sampler = SamplerResoucePool::Instance().GetResource(SamplerStateType::LINIEAR_WRAP);
			ID3D11SamplerState* samplerArray[1];
			samplerArray[0] = sampler.Get();
			device.GetDeviceContext()->PSSetSamplers(0, samplerSlot.num, samplerArray);
		}
	}

	void InitNormalMaterial()
	{
		Ptr<ConstantBufferObject> materialCBO = ConstantBufferPool::Instance().CreateDeviceResource("material", sizeof(Material), device);

		materialCBO->Upload<Material>(Material{ Vec4f(0.1,0.1,0.1,1.0),Vec4f(0.2,0.2,0.2,1.0),Vec4f(0.7,0.7,0.7,1.0),Vec4f(0.5,0.5,0.5,1.0) });

		const uint32_t MaterialParamSlot = 3; // fixed in default shader
		mMaterialSlot.uniformSlots.push_back(MaterialSlot::UniformSlot{ MaterialParamSlot, true, true, 1, materialCBO->GetBufferSlot() });
		
		auto path = GLOBAL_PATH + "Model//TexturesCom_Brick_BlocksBare_1K_albedo.tif";
		
		int32_t resourceSlot = ShaderResoucePool::Instance().CreateDeviceResource(path, device);
		int32_t mapSlot = 0;  // fixed in default ps shader
		mMaterialSlot.textureSlots.push_back(MaterialSlot::TextureSlot{ mapSlot, 1, resourceSlot });

		mMaterialSlot.samplerSlots.push_back(MaterialSlot::SamplerSlot{ 0, 1, 1 });
	}
	
	void InitPbrMaterial()
	{
		
		Ptr<ConstantBufferObject> materialCBO = ConstantBufferPool::Instance().CreateDeviceResource("pbrMaterial", sizeof(PbrMaterialMetalRoughness), device);
		
		int32_t materialSlot = mGeomtry->materialSlot;
		int32_t materialMapSlot = mGeomtry->materialMapSlot;
		string path = mGeomtry->GetModelFile().GetFilePath();

		MaterialMapPtr mapPtr = MaterialMapBuilder::Instance().GetResource(materialMapSlot);
		
		materialCBO->Upload<PbrMaterialMetalRoughness>(*(MaterialBuilder::Instance().GetResource(materialSlot).get()));

		const uint32_t PbrMaterialParamSlot = 3; // fixed in pbr shader
		mMaterialSlot.uniformSlots.push_back(MaterialSlot::UniformSlot{ PbrMaterialParamSlot, false, true, 1, materialCBO->GetBufferSlot() });

		int32_t resourceSlot =ShaderResoucePool::Instance().CreateDeviceResource(path + mapPtr->baseColorMap.uri, device);
		int32_t mapSlot = 0;  // fixed in pbr shader
		mMaterialSlot.textureSlots.push_back(MaterialSlot::TextureSlot{ mapSlot, 1, resourceSlot });

		resourceSlot = ShaderResoucePool::Instance().CreateDeviceResource(path + mapPtr->metalRoughnessMap.uri, device);
		mapSlot = 1;  // fixed in pbr shader
		mMaterialSlot.textureSlots.push_back(MaterialSlot::TextureSlot{ mapSlot, 1, resourceSlot });

		resourceSlot = ShaderResoucePool::Instance().CreateDeviceResource(path + mapPtr->normalMap.uri, device);
		mapSlot = 2;  // fixed in pbr shader
		mMaterialSlot.textureSlots.push_back(MaterialSlot::TextureSlot{ mapSlot, 1, resourceSlot });

		resourceSlot = ShaderResoucePool::Instance().CreateDeviceResource(path + mapPtr->aoMap.uri, device);
		mapSlot = 3;  // fixed in pbr shader
		mMaterialSlot.textureSlots.push_back(MaterialSlot::TextureSlot{ mapSlot, 1, resourceSlot });

		resourceSlot = ShaderResoucePool::Instance().CreateDeviceResource(path + mapPtr->emissiveMap.uri, device);
		mapSlot = 4;  // fixed in pbr shader
		mMaterialSlot.textureSlots.push_back(MaterialSlot::TextureSlot{ mapSlot, 1, resourceSlot });

		mMaterialSlot.samplerSlots.push_back(MaterialSlot::SamplerSlot{ 0, 1, 1 });
	}

	void InitSkyBoxMaterial()
	{
		string path = mGeomtry->GetModelFile().GetFilePath();
		int32_t resourceSlot = ShaderResoucePool::Instance().CreateDeviceResource(path + "\\skybox.dds", device);
		int32_t mapSlot = 0;  // fixed in default sky shader
		mMaterialSlot.textureSlots.push_back(MaterialSlot::TextureSlot{ mapSlot, 1, resourceSlot });
		mMaterialSlot.samplerSlots.push_back(MaterialSlot::SamplerSlot{ 0, 1, 1 });
	}

	void Draw() override
	{
		PreDraw();
		device.GetDeviceContext()->IASetPrimitiveTopology(mTopology);

		const VBufferDescriptor& vDesc = mVBO->descriptor;
		device.GetDeviceContext()->IASetVertexBuffers(vDesc.registerSlot, vDesc.numBuffers, mVBO->GetBufferViewAddress(), &vDesc.stride, &vDesc.offset);

		if (mUseIndex)
		{
			const IBufferDescriptor& iDesc = mIBO->descriptor;
			device.GetDeviceContext()->IASetIndexBuffer(mIBO->GetBufferView(), mIBO->GetElementFormat(), iDesc.offset);
			device.GetDeviceContext()->DrawIndexed(iDesc.count, iDesc.startIndexLocation, iDesc.baseVertexLocation);
		}
		else {
			device.GetDeviceContext()->Draw(vDesc.count, vDesc.startVertexLocation);
		}
	
	}

protected:
	const FDx11Device& device;
	D3D_PRIMITIVE_TOPOLOGY mTopology;
	

	VBOPtr mVBO;
	Ptr<DisJointVertexBufferObject> mDisjointVBO;
	IBOPtr mIBO;
	MaterialSlot mMaterialSlot;
	Ptr<FGeometry> mGeomtry;   // weak ptr
};

