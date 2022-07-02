#pragma once
#include"Buffer.h"
#include"FDx11BufferObject.h"
# include"ShaderParameter.h"

//class FDx11ConstantBufferObject:public Dx11BufferObject
//{
//public:
//	//ComPtr<ID3D11Buffer>        m_buffer;
//
//	//ID3D11Buffer* GetBuffer() { return m_buffer.Get(); }
//
//	void ApplyChange(FDx11ConstantBufferObject* cbo, void* data)
//	{
//		// HR(Buffer != nullptr);
//
//		/*bool GPUWritable = cbo->GetBufferDescription().GPUWritable;
//
//
//		if (GPUWritable)
//		{
//			m_pd3dImmediateContext->UpdateSubresource(cbo->GetBuffer(), 0, nullptr, data, 0, 0);
//		}
//		else
//		{
//			D3D11_MAPPED_SUBRESOURCE mappedResource;
//			m_pd3dImmediateContext->Map(cbo->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//			CopyMemory(mappedResource.pData, data, cbo->GetBufferDescription().byteWidth);
//			m_pd3dImmediateContext->Unmap(cbo->GetBuffer(), 0);
//		}*/
//	}
//	//template<class T>
//	//void CompileBuffer(Uniform<T>* uniform)
//	//{
//	//	uint32_t range = uniform->GetByteSize();
//	//	uint32_t offset = 0;
//	//	InitDx11BufferObject(range, uniform->GetDataPtr());
//	//}
//};


