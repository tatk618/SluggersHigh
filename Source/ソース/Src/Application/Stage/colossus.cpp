#include "../Stage/colossus.h"
#include "../define.h"
#include "../Manager/Model/ModelManager.h"
#include "../namespace/namespace.h"

C_Colossus::C_Colossus()
{
	colossus_mod = MODEL.GetModel("Data/Model/stage/city2.gltf");

	DWORD polygonnum = 0;
	DWORD vertexnum = 0;

	for (auto&& node : colossus_mod->GetAllNodes())
	{
		if (node.Mesh == nullptr)continue;

		polygonnum += node.Mesh->GetFaces().size();

		vertexnum += node.Mesh->GetVertexPositions().size();
	}
}

void C_Colossus::Draw()
{
	if (colossus_mod != nullptr)
	{
		SHADER.m_standardShader.SetWorldMatrix(colossus_mat);
		SHADER.m_standardShader.DrawModel(colossus_mod);
	}
}

bool C_Colossus::Checkhit(const Math::Vector3& _raypos, const Math::Vector3& _rayvec, float& _dis, Math::Vector3& _returnvec) const
{
	Math::Vector3 wallnormal;

	if (function::ModelIntersects(*colossus_mod, colossus_mat, _raypos, _rayvec, _dis,wallnormal))
	{
		Math::Vector3 dot;
		dot = DirectX::XMVector3Dot(-_rayvec, wallnormal);

		float limit;
		limit = 3 / dot.x;

		if (_dis <= limit)
		{
			_returnvec = wallnormal * ((limit - _dis * dot.x));

			return true;
		}

		return false;
	}

	return false;
}
