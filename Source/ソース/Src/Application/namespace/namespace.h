#pragma once
namespace function
{
	float TurnToAng(Math::Matrix& _mat, const float _targetAng, const float _plusAng);
	float GetVecAngY(Math::Vector3 _vec);
	//ModelIntersects(モデル、モデルの行列、レイの発射位置、レイの発射方向、レイが命中した位置までの距離)
	bool ModelIntersects(const KdModel& _model, const Math::Matrix& _mat, const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _wallnormal);
}