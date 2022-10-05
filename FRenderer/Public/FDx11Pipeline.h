#pragma once

#include"FDx11Pass.h"

class FGroup;
class FDx11Pipeline: public FReference
{
public:
	FDx11Pipeline(const FDx11Device& _device):device(_device)
	{

	}
	enum class PipelineConfig : uint8_t
	{
		Forward,
		Deferred,
		Ulnit
	};
	using CBOPtr = Ptr<ConstantBufferObject>;
public:
	const FDx11Device& device;
	void Execute(FGroup* );

	void Init();
	void _InitAllConstantBuffer();

	void _InitForwardPassShaderInput();
	void _InitSkyPassShaderInput();
	void _InitDeferredPassShaderInput();
	void _InitGBufferPassShaderInput();

	Ptr<FDx11Pass> forwardPass;
	Ptr<FDx11Pass> skyPass;
	Ptr<FDx11Pass> gBufferPass;
	Ptr<FDx11Pass> deferredPass;
};