#pragma once

#include<string>
#include"Buffer.h"
#include"FDx11.h"

//IASetVertexBuffers



class Dx11BufferObject:public FReference
{
public:
	Dx11BufferObject(uint32_t byteSize, unsigned char* data, const FDx11Device& _device) :device(_device)
	{
		InitDx11BufferObject(byteSize, data);
	}
	void InitDx11BufferObject(uint32_t byteSize, unsigned char* data)
	{
		mDesc.Usage = D3D11_USAGE_DYNAMIC;
		mDesc.BindFlags = mBindFlag;
		mDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		mDesc.MiscFlags = 0;
		mDesc.ByteWidth = byteSize;
		mDesc.Usage = D3D11_USAGE_DEFAULT;
		mDesc.CPUAccessFlags = 0;
		device.GetDevice()->CreateBuffer(&mDesc, nullptr, mDx11Buffer.GetAddressOf());
		mInit = true;
	}
	virtual void SetBufferBindFlag() = 0;
	DXGI_FORMAT  GetElementFormat() { return mFormat; }
	void SetElementFormat(DXGI_FORMAT format) { mFormat = format; }

	ID3D11Buffer* GetBufferView() { return mDx11Buffer.Get(); }
	ID3D11Buffer** GetBufferViewAddress() { return mDx11Buffer.GetAddressOf(); }
	void SetBufferData(unsigned char* data, uint32_t byteSize) { mDataByteSize = byteSize;  mData = data; mDirty = true; }

protected:
	DXGI_FORMAT mFormat =	DXGI_FORMAT_R32_UINT;
	D3D11_BUFFER_DESC		mDesc;
	ComPtr <ID3D11Buffer>	mDx11Buffer;
	unsigned char*			mData;
	uint32_t				mDataByteSize;
	
	bool					mDirty = false;
	bool					mInit = false;
	bool                    mGPUWritable = false;

	const FDx11Device&      device;
	unsigned int            mBindFlag;

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
			device.GetDeviceContext()->Unmap(mDx11Buffer.Get().Get(), 0);
		}
	}

};

class VertexBufferObject: public Dx11BufferObject
{
public:
	VertexBufferObject(uint32_t byteSize, unsigned char* data, const FDx11Device& _device):Dx11BufferObject(byteSize, data, _device)
	{
		mBindFlag = D3D11_BIND_VERTEX_BUFFER;
	}

	VBufferDescriptor descriptor;
	void SetBufferBindFlag() override
	{
		mDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	}

};

class IndexBufferObject : public Dx11BufferObject
{
public:
	IndexBufferObject(uint32_t byteSize, unsigned char* data, const FDx11Device& _device) :Dx11BufferObject(byteSize, data, _device)
	{
		mBindFlag = D3D11_BIND_INDEX_BUFFER;
	}

	IBufferDescriptor descriptor;
	void SetBufferBindFlag() override
	{
		mDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	}
};

class ConstantBufferObject : public Dx11BufferObject
{
public:
	ConstantBufferObject(uint32_t byteSize, unsigned char* data, const FDx11Device& _device) :Dx11BufferObject(byteSize, data, _device)
	{
		mBindFlag = D3D11_BIND_CONSTANT_BUFFER;
	}

	void SetBufferBindFlag() override
	{
		mDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	}
};