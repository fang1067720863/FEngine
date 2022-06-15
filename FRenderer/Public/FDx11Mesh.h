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

	FDx11Mesh(FGeometry* geom):FNode("") {
		Init(geom);
	}

	D3D_PRIMITIVE_TOPOLOGY GetTopology() { return mTopology; }
	VertexBufferObject* GetVBO() { return mVBO.get(); }
	IndexBufferObject* GetIBO() { return mIBO.get(); }
	bool UseIndex() { return mUseIndex; }

	void Init(FGeometry* geom)
	{
		// Init Dx11 InputLayout&Topology
		mInputLayout = new FDx11VertexInputLayout(geom->GetVertexElementType());
		// magic 
		mTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		// Init Dx11 VBO IBO
		mVBO = new VertexBufferObject(geom->GetCompiledVertexDataSize(), geom->GetCompiledVertexData());
		mIBO = new IndexBufferObject(geom->GetCompiledIndexDataSize(), geom->GetCompiledIndexData());
		
	}

	void Draw(ID3D11DeviceContext* mDeviceContext)
	{
		mDeviceContext->IASetInputLayout(mInputLayout->GetD3D11InputLayout());
		mDeviceContext->IASetPrimitiveTopology(mTopology);

		const VBufferDescriptor& vDesc = mVBO->descriptor;
		mDeviceContext->IASetVertexBuffers(vDesc.registerSlot, vDesc.count, mVBO->GetBufferViewAddress(), &vDesc.stride, &vDesc.offset);

		if (mUseIndex)
		{
			const IBufferDescriptor& iDesc = mIBO->descriptor;
			mDeviceContext->IASetIndexBuffer(mIBO->GetBufferView(), mIBO->GetElementFormat(), iDesc.offset);
			mDeviceContext->DrawIndexed(iDesc.count, iDesc.startIndexLocation, iDesc.baseVertexLocation);
		}
		else {
			mDeviceContext->Draw(vDesc.count, vDesc.startVertexLocation);
		}
	}
	

protected:
	ID3D11Device* device;
	Ptr<FDx11VertexInputLayout> mInputLayout = nullptr;
	D3D_PRIMITIVE_TOPOLOGY mTopology;
	bool mUseIndex = true;

	VBOPtr mVBO;
	IBOPtr mIBO;
	MaterialPtr mMaterial;
};

