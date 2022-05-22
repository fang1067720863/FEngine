#pragma once

#include"Mesh.h"
#include"Renderer.h"

class FModel
{
	using MeshPtr = FPtr<FMesh>;
	std::vector<MeshPtr> m_meshes;


	void draw(FRenderer* renderer){
		for(const auto& mesh : m_meshes)
		{
			renderer->DrawMeshObject(mesh.get());
		}
};