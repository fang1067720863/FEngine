#pragma once

#include<string>
#include"Buffer.h"
#include"FDx11.h"
#include<D3DX11tex.h>
#include"UtfConverter.h"



class Dx11BufferObject:public FReference
{
public:
	
	Dx11BufferObject(uint32_t byteSize, unsigned int bindFlag, const FDx11Device& _device) :device(_device), mBindFlag(bindFlag)
	{
		InitDx11BufferObject(byteSize);
	}
	Dx11BufferObject(uint32_t byteSize,void* initData, unsigned int bindFlag, const FDx11Device& _device) :device(_device), mBindFlag(bindFlag)
	{
		InitDx11BufferObject(byteSize, initData);
	}
	virtual ~Dx11BufferObject()
	{
		// todo
	}
	void InitDx11BufferObject(uint32_t byteSize)
	{
		mDesc.Usage = D3D11_USAGE_DYNAMIC;
		mDesc.BindFlags = mBindFlag;
		mDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		mDesc.MiscFlags = 0;
		mDesc.ByteWidth = byteSize;
		mDesc.Usage = D3D11_USAGE_DEFAULT;
		mDesc.CPUAccessFlags = 0;
		HR(device.GetDevice()->CreateBuffer(&mDesc, nullptr, mDx11Buffer.GetAddressOf()));
		mInit = true;
	}
	void InitDx11BufferObject(uint32_t byteSize, void* data)
	{
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = data;
		mDesc.Usage = D3D11_USAGE_DYNAMIC;
		mDesc.BindFlags = mBindFlag;
		mDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		mDesc.MiscFlags = 0;
		mDesc.ByteWidth = byteSize;
		mDesc.Usage = D3D11_USAGE_DEFAULT;
		mDesc.CPUAccessFlags = 0;
		HR(device.GetDevice()->CreateBuffer(&mDesc, &initData, mDx11Buffer.GetAddressOf()));
		mInit = true;
	}
	DXGI_FORMAT  GetElementFormat() { return mFormat; }
	void SetElementFormat(DXGI_FORMAT format) { mFormat = format; }

	ID3D11Buffer* GetBufferView() { return mDx11Buffer.Get(); }
	ID3D11Buffer** GetBufferViewAddress() { return mDx11Buffer.GetAddressOf(); }
	void SetBufferData(unsigned char* data, uint32_t byteSize) { mDataByteSize = byteSize;  mData = data; mDirty = true; }

	template <typename DataType>
	void Upload(std::vector<DataType> const& data, size_t stride = 0) const
	{
		size_t elementSize = stride ? stride : sizeof(DataType);
		assert(sizeof(DataType) <= elementSize);

		assert(mDx11Buffer);

		if (mGPUWritable)
		{
			device.GetDeviceContext()->UpdateSubresource(mDx11Buffer.Get(), 0, nullptr, data.data(), 0, 0);
		}
		else
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HR(device.GetDeviceContext()->Map(mDx11Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
			CopyMemory(mappedResource.pData, data.data(), elementSize);
			device.GetDeviceContext()->Unmap(mDx11Buffer.Get(), 0);
		}
	}
	template <typename DataType>
	void Upload(DataType const& data, size_t stride = 0) const
	{
		size_t elementSize = stride ? stride : sizeof(DataType);
		assert(sizeof(DataType) <= elementSize);

		assert(mDx11Buffer);

		if (mGPUWritable)
		{
			device.GetDeviceContext()->UpdateSubresource(mDx11Buffer.Get(), 0, nullptr, &data, 0, 0);
		}
		else
		{
			//todo
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HR(device.GetDeviceContext()->Map(mDx11Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
			CopyMemory(mappedResource.pData, &data, elementSize);
			device.GetDeviceContext()->Unmap(mDx11Buffer.Get(), 0);
		}
	}

protected:
	DXGI_FORMAT mFormat =	DXGI_FORMAT_R32_UINT;
	D3D11_BUFFER_DESC		mDesc;
	ComPtr <ID3D11Buffer>	mDx11Buffer;
	unsigned char*			mData;
	uint32_t				mDataByteSize;
	
	bool					mDirty = false;
	bool					mInit = false;
	bool                    mGPUWritable = true;

	const FDx11Device&      device;
	unsigned int            mBindFlag;
	

	

};

class VertexBufferObject: public Dx11BufferObject
{
public:
	VertexBufferObject(uint32_t byteSize, void* data, const FDx11Device& _device): Dx11BufferObject(byteSize,data, D3D11_BIND_VERTEX_BUFFER, _device)
	{
	   
	}

	VertexBufferObject( uint32_t byteSize, void* data, const FDx11Device& _device,  uint32_t stride, uint32_t vertexNum, uint32_t componentNum = 1) :
		Dx11BufferObject(byteSize, data, D3D11_BIND_VERTEX_BUFFER, _device)
	{
		descriptor.numBuffers = componentNum;
		descriptor.registerSlot = 0;  
		descriptor.offset = 0;
		descriptor.startVertexLocation = 0;
		descriptor.stride = stride;
		descriptor.count = vertexNum;
	}

	VBufferDescriptor descriptor;

};

struct VertexAttribute
{
	uint32_t byteSize{ 0 };
	uint32_t stride{ 0 };
	void* data{ nullptr };
};

struct VextexData
{
	std::vector<VertexAttribute> _attributes;
	uint32_t componentNum;
	uint32_t vertexNum; 
};
class DisJointVertexBufferObject :public FReference
{
public:
	DisJointVertexBufferObject(const VextexData& vData, const FDx11Device& _device):device(_device)
	{
		size_t num = vData.componentNum;
		mVbos.resize(num);
		descriptor.strides.resize(num);
		descriptor.offsets.resize(num);

		uint32_t offset = 0;
		for (uint32_t i = 0; i < num; i++)
		{
			const VertexAttribute& attr = vData._attributes[i];
			Ptr<VertexBufferObject> vbo=new VertexBufferObject(attr.byteSize, attr.data, _device, attr.stride,vData.vertexNum);
			mVbos.push_back(vbo);   // ???
			descriptor.strides.push_back(attr.stride);
			descriptor.offsets.push_back(offset);
			offset += attr.stride;      
		}

	}

	std::vector<Ptr<VertexBufferObject>> mVbos;
	const FDx11Device& device;
	VBufferDescriptor descriptor;

	ID3D11Buffer** GetBufferViewAddress() { 
		std::vector<ID3D11Buffer*> buffers(mVbos.size());
		for (int i = 0; i < mVbos.size(); i++)
		{
			buffers.push_back(mVbos[i]->GetBufferView());
		}
		return buffers.data();
		
	}
	ID3D11Buffer* GetBufferView(uint32_t i) { return mVbos[i]->GetBufferView(); }
	/*std::vector<uint32_t> strides;
	std::vector<uint32_t> offsets;*/
};

class IndexBufferObject : public Dx11BufferObject
{
public:
	IndexBufferObject(uint32_t byteSize,void* data, const FDx11Device& _device) 
		:Dx11BufferObject(byteSize,data, D3D11_BIND_INDEX_BUFFER, _device)
	{
	
	}
	IndexBufferObject(uint32_t byteSize, void* data, const FDx11Device& _device, uint32_t indexCount)
		:Dx11BufferObject(byteSize, data, D3D11_BIND_INDEX_BUFFER, _device)
	{
		descriptor.count = indexCount;
		descriptor.offset = 0;
		descriptor.startIndexLocation = 0;
	}

	IBufferDescriptor descriptor;
	
	//uint32_t mBindFlag;
};

class ConstantBufferObject : public Dx11BufferObject
{
	friend class ConstantBufferPool;
	enum CB_SHADER_STAGE : uint16_t
	{
		VS_STAGE = 1 << 0,
		PS_STAGE = 1 << 1,
		VS_PS_STAGE = VS_STAGE + PS_STAGE
	};
public:
	ConstantBufferObject(uint32_t byteSize,  const FDx11Device& _device, const std::string& slot):Dx11BufferObject(byteSize, D3D11_BIND_CONSTANT_BUFFER, _device), bufferSlot(slot)
	{

	}
	//CB_SHADER_STAGE mShaderStage;
	std::string bufferSlot;
	const std::string& GetBufferSlot() const { return bufferSlot; }
};


//class SamplerResoucePool
//{
//
//public:
//	enum class SamplerType: uint16_t
//	{
//		SSLinearWrap,			            // 线性过滤
//	    SSAnistropicWrap		                // 各项异性过滤
//	};
//	typedef SamplerType SamplerSlot;
//	typedef ComPtr <ID3D11SamplerState> ResouceViewPtr;
//	typedef std::unordered_map<SamplerSlot, ResouceViewPtr> ResouceMap;
//
//	ResouceViewPtr FindorCreateTextureView(SamplerSlot slot, const FDx11Device& _device)
//	{
//		if (resourceMap.find(slot) != resourceMap.end())
//		{
//			return resourceMap.at(slot);
//		}
//	
//		if (slot == SamplerSlot::SSLinearWrap)
//		{
//			ResouceViewPtr samplerLinearWrap;
//
//			//ComPtr<ID3D11SamplerState> SSLinearWrap = nullptr;
//			D3D11_SAMPLER_DESC sampDesc;
//			ZeroMemory(&sampDesc, sizeof(sampDesc));
//
//			// 线性过滤模式
//			sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//			sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//			sampDesc.MinLOD = 0;
//			sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
//			HR(_device.GetDevice()->CreateSamplerState(&sampDesc, samplerLinearWrap.GetAddressOf()));
//			resourceMap.insert(std::pair <SamplerSlot, ResouceViewPtr>(SamplerType::SSLinearWrap, samplerLinearWrap));
//		}
//		else
//		{
//			ResouceViewPtr samplerAnistropicWrap;
//			D3D11_SAMPLER_DESC sampDesc;
//			// 各向异性过滤模式
//			sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
//			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//			sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//			sampDesc.MaxAnisotropy = 4;
//			sampDesc.MinLOD = 0;
//			sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
//			HR(_device.GetDevice()->CreateSamplerState(&sampDesc, samplerAnistropicWrap.GetAddressOf()));
//			resourceMap.insert(std::pair <SamplerSlot, ResouceViewPtr>(SamplerType::SSAnistropicWrap, samplerAnistropicWrap));
//		}
//		return GetSamplerState(slot);
//	
//	}
//	static SamplerResoucePool& Instance()
//	{
//		static SamplerResoucePool instance;
//		return instance;
//	}
//	ResouceViewPtr GetSamplerState(SamplerSlot slot)
//	{
//		return resourceMap.at(slot);
//	}
//public:
//
//	SamplerResoucePool()
//	{
//	
//	}
//
//	ResouceMap resourceMap;
//};