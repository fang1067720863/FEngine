#pragma once

#include<map>
#include<string>
#include"FDx11BufferObject.h"
#include"UtfConverter.h"
#include"Factory.h"




typedef int32_t TextureSlot;



class ConstantBufferPool : public ResourceFactory<Ptr<ConstantBufferObject>, std::string, ConstantBufferPool>
{

public:
	Resource CreateDeviceResource(const SLOT& slot, std::int32_t size, const FDx11Device& _device)
	{
		if (resourceMap.find(slot)!= resourceMap.end())
		{
			return resourceMap[slot];
		}
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
		textureGuid++;
		HR(D3DX11CreateShaderResourceViewFromFile(_device.GetDevice(), ConvertUtf(name).c_str(), NULL, NULL, textureView.GetAddressOf(), NULL));
		resourceMap.insert(std::pair <SLOT, Resource>(textureGuid, textureView));
		return textureGuid;
	}
	bool CreateResouce(const SLOT& slot, Resource resource) override { return false; }
	SLOT CreateShaderResouce(Resource resource)  {
		textureGuid++;
		resourceMap.insert(std::pair <SLOT, Resource>(textureGuid, resource));
		return textureGuid; 
	}
	/*bool CreateShaderResouceView(const SLOT& slot, const FDx11Device& _device)  {
		_device.device->CreateShaderResourceView()
		return false; }*/


private:
	TextureSlot textureGuid{ 0 };
};

