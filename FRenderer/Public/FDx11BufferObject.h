#pragma once

#include<string>
#include"Buffer.h"
#include"FDx11.h"

//IASetVertexBuffers



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

	VertexBufferObject(uint32_t byteSize, void* data, const FDx11Device& _device,  uint32_t stride, uint32_t vertexNum, uint32_t componentNum = 1) :
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
public:
	ConstantBufferObject(uint32_t byteSize, const FDx11Device& _device):Dx11BufferObject(byteSize, D3D11_BIND_CONSTANT_BUFFER, _device)
	{

	}
};

class ConstantBufferPool 
{
public :
	Ptr<ConstantBufferObject> CreateConstantBuffer(const std::string name, std::int32_t size, const FDx11Device& _device)
	{
		Ptr<ConstantBufferObject> cbo = new ConstantBufferObject(size, _device);
		mCBOMap.insert(std::pair < const std::string, Ptr<ConstantBufferObject>>(name, cbo));
		return cbo;
	}
	Ptr<ConstantBufferObject> GetConstantBuffer(const std::string& name, std::int32_t size)
	{
		return mCBOMap.at(name);
	}
	static ConstantBufferPool& GetInstance()
	{
		static ConstantBufferPool instance;
		return instance;
	}
private:
	typedef std::unordered_map<std::string, Ptr<ConstantBufferObject>> CBOMap;
	CBOMap mCBOMap;
	ConstantBufferPool() = default;
	~ConstantBufferPool() = default;

};