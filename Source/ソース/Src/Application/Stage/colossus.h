#pragma once

class C_Colossus
{
public:
	C_Colossus();

	void Draw();
	bool Checkhit(const Math::Vector3& _raypos, const Math::Vector3& _rayvec, float& _dis, Math::Vector3& _returnvec)const;

	const KdModel& GetMapModel() const
	{
		return *colossus_mod;
	}

	const Math::Matrix& GetMapMat() const
	{
		return colossus_mat;
	}

private:
	KdModel* colossus_mod = nullptr;
	Math::Matrix colossus_mat;
};