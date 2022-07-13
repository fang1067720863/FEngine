#pragma once


#include"FDx11BufferObject.h"
#include"FDx11InpuyLayout.h"

#include"Material.h"
#include"VecArray.h"
#include"Ptr.h"
#include"Geometry.h"
#include"Node.h"


class FDx11Mesh:public FNode
{
public:
	friend class FMeshBuilder;
	typedef std::shared_ptr<Material> MaterialPtr;
	typedef Ptr<VertexBufferObject> VBOPtr;
	typedef Ptr<IndexBufferObject> IBOPtr;
	

	FDx11Mesh(FGeometry* geom, const FDx11Device& _device):FNode(""),device(_device) {
		Init(geom);
	}

	D3D_PRIMITIVE_TOPOLOGY GetTopology() { return mTopology; }
	VertexBufferObject* GetVBO() { return mVBO.get(); }
	IndexBufferObject* GetIBO() { return mIBO.get(); }
	bool UseIndex() { return mUseIndex; }

	void Init(FGeometry* geom)
	{
		// Init Dx11 InputLayout&Topology
		//mInputLayout = new FDx11VertexInputLayout(geom->GetVertexElementType(),device);
		// magic 
		mTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		// Init Dx11 VBO IBO
		mVBO = new VertexBufferObject(geom->GetCompiledVertexDataSize(), geom->GetCompiledVertexData(), device, geom->GetVertexStride(),geom->GetVertexELementNum());
		mIBO = new IndexBufferObject(geom->GetCompiledIndexDataSize(), geom->GetCompiledIndexData(), device, geom->GetIndexELementNum());
		
	}

	void Draw()
	{
		//device.GetDeviceContext()->IASetInputLayout(mInputLayout->GetD3D11InputLayout());
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
	//Ptr<FDx11VertexInputLayout> mInputLayout = nullptr;
	D3D_PRIMITIVE_TOPOLOGY mTopology;
	bool mUseIndex = true;

	VBOPtr mVBO;
	IBOPtr mIBO;
	MaterialPtr mMaterial;
};

