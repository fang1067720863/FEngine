#pragma once

#include<map>
#include<string>
#include"FDx11ConstantBuffer.h"
#include"UtfConverter.h"
#include"Factory.h"




typedef int32_t TextureSlot;



class ConstantBufferPool : public ResourceFactory<Ptr<ConstantBufferObject>, std::string, ConstantBufferPool>
{

public:
	Resource CreateDeviceResource(const SLOT& slot, std::int32_t size, const FDx11Device& _device)
	{
		Ptr<ConstantBufferObject> cbo = new ConstantBufferObject(size, _device, slot);
		resourceMap.insert(std::pair < const std::string, Ptr<ConstantBufferObject>>(slot, cbo));
		return cbo;
	}
	bool CreateResouce(const SLOT& slot, Resource resource) override { return false; }
};

class ShaderResoucePool : public ResourceFactory<ComPtr<ID3D11ShaderResourceView>, TextureSlot, ShaderResoucePool>
{
public:
	SLOT CreateDeviceResource(const std::string& name, const FDx11Device& _device)
	{
		Resource textureView;
		
		HR(D3DX11CreateShaderResourceViewFromFile(_device.GetDevice(), ConvertUtf(name).c_str(), NULL, NULL, textureView.GetAddressOf(), NULL));
		textureGuid++;
		resourceMap.insert(std::pair <SLOT, Resource>(textureGuid, textureView));
		return textureGuid;
	}
	bool CreateResouce(const SLOT& slot, Resource resource) override { return false; }

private:
	TextureSlot textureGuid{ 0 };
};

