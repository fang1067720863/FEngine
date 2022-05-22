#pragma once
#include"ShaderInput.h"
class ShaderCollection;



class FMaterial
{
	// texture shader params
	template<typename Type>
	void SetShaderConstant(){}

private:

	ShaderCollection m_shaderCollection;
	ShaderResourceGroup m_shaderResourceGroup;
};

class PbrMaterial :public Material
{

};