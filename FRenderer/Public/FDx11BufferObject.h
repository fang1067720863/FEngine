#pragma once

#include<string>
#include"Buffer.h"
#include"FDx11.h"

//IASetVertexBuffers


class Dx11BufferObject:FReference
{
public:
	Dx11BufferObject()
	{

	}
	Dx11BufferObject(uint32_t byteSize, unsigned char* data)
	{
		InitDx11BufferObject(byteSize, data);
	}
	void InitDx11BufferObject(uint32_t byteSize, unsigned char* data)
	{
		mDesc.Usage = D3D11_USAGE_DYNAMIC;
		SetBufferBindFlag();
		mDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		mDesc.MiscFlags = 0;
		mDesc.ByteWidth = byteSize;
		mDesc.Usage = D3D11_USAGE_DEFAULT;
		mDesc.CPUAccessFlags = 0;
		device->CreateBuffer(&mDesc, nullptr, mDx11Buffer.GetAddressOf());
		mInit = true;
	}
	virtual void SetBufferBindFlag() = 0;
	DXGI_FORMAT  GetElementFormat() { return mFormat; }
	void SetElementFormat(DXGI_FORMAT format) { mFormat = format; }

	ID3D11Buffer* GetBufferView() { return mDx11Buffer.Get(); }
	ID3D11Buffer** GetBufferViewAddress() { return mDx11Buffer.GetAddressOf(); }
	void SetBufferData(unsigned char* data, uint32_t byteSize) { mDataByteSize = byteSize;  mData = data; mDirty = true; }

protected:
	DXGI_FORMAT mFormat = DXGI_FORMAT_R32_UINT;
	D3D11_BUFFER_DESC mDesc;
	ComPtr <ID3D11Buffer> mDx11Buffer;
	unsigned char* mData;
	uint32_t mDataByteSize;
	
	bool mDirty = false;
	bool mInit = false;

	ID3D11Device* device;

};

class VertexBufferObject: public Dx11BufferObject
{
public:
	VertexBufferObject(uint32_t byteSize, unsigned char* data):Dx11BufferObject(byteSize, data)
	{
		
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
	IndexBufferObject(uint32_t byteSize, unsigned char* data) :Dx11BufferObject(byteSize, data)
	{

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
	void SetBufferBindFlag() override
	{
		mDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	}
};