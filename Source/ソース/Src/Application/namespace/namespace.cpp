#include "namespace.h"

float function::TurnToAng(Math::Matrix& _mat, const float _targetAng, const float _plusAng)
{
	//このキャラクターの向きを調べる
	Math::Vector3 frontVec;
	frontVec = _mat.Backward();	//右手座標での後方（左手座標の前方にあたる）
	//frontVec = DirectX::XMVector3TransformNormal(Math::Vector3(0,0,1),_mat);

	float nowAng;
	nowAng = GetVecAngY(frontVec);

	//あと何度回転すべきか
	float lastAng;
	lastAng = _targetAng - nowAng;
	//例:目標が+40度、今の向きが-20度なら、残りは60度となる

	//角度が+180～-180の範囲に収まるようにする
	while (lastAng > 180)
	{
		lastAng -= 360;
	}
	while (lastAng < -180)
	{
		lastAng += 360;
	}

	//残りの角度が_plusAng以上に残っているなら、
	//_plusAngだけ回る回転行列を作る

	Math::Matrix tmpMat;

	if (lastAng > _plusAng)
	{
		//右回転する場合
		tmpMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(_plusAng));
		lastAng -= _plusAng;

	}
	else if (lastAng < -_plusAng)
	{
		//左回転する場合
		tmpMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(-_plusAng));
		lastAng += _plusAng;
	}
	else {
		tmpMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(lastAng));
		lastAng = 0;
	}

	//キャラクターを回転させる
	_mat = tmpMat * _mat;

	//残っている角度を返す
	return lastAng;

}

float function::GetVecAngY(Math::Vector3 _vec)
{
	//ベクトルの上下の向きをつぶす
	_vec.y = 0;

	_vec.Normalize();

	Math::Vector3 dot;
	dot = DirectX::XMVector3Dot(Math::Vector3(0, 0, 1), _vec);

	float ang;
	ang = DirectX::XMConvertToDegrees(acos(dot.x));
	if (_vec.x < 0)
	{
		ang *= -1;
	}

	return ang;
}

bool function::ModelIntersects(const KdModel& _model, const Math::Matrix& _mat, const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis,Math::Vector3& _wallnormal)
{
	Math::Vector3 hitVPos[3];	//レイが当たったポリゴンの頂点座標
	Math::Matrix hitMat;	//レイが当たったポリゴンの行列

	float nearestDist = FLT_MAX;	//FLT_MAX==float型の最大値
	for (auto&& node : _model.GetAllNodes())
	{
		//メッシュが含まれていないノードなら飛ばす
		if (node.Mesh == nullptr)continue;

		//そのノードのワールド行列を取得
		Math::Matrix worldMat = node.LocalTransform * _mat;

		//逆行列化
		Math::Matrix inv = worldMat.Invert();

		//逆行列を使ってローカル座標に変換
		Math::Vector3 localPos = DirectX::XMVector3Transform(_rayPos, inv);
		Math::Vector3 localDir = DirectX::XMVector3TransformNormal(_rayVec, inv);

		//拡大行列を使っているメッシュ用にレイの長さを計っておく
		float rayLen = localDir.Length();

		//方向は必ず長さ１にする
		localDir.Normalize();

		//メッシュ内の全ポリゴンを回す
		for (auto&& face : node.Mesh->GetFaces())
		{
			float dist;

			Math::Vector3 vPos[3];
			vPos[0] = node.Mesh->GetVertexPositions()[face.Idx[0]];
			vPos[1] = node.Mesh->GetVertexPositions()[face.Idx[1]];
			vPos[2] = node.Mesh->GetVertexPositions()[face.Idx[2]];

			bool hit = DirectX::TriangleTests::Intersects(
				localPos,
				localDir,
				node.Mesh->GetVertexPositions()[face.Idx[0]],//１頂点目の座標
				node.Mesh->GetVertexPositions()[face.Idx[1]],//２頂点目の座標
				node.Mesh->GetVertexPositions()[face.Idx[2]],//３頂点目の座標
				dist	//レイが当たったところまでの距離(参照型)
			);

			if (hit == false)continue;

			//拡大分を反映
			dist /= rayLen;

			//もっとも近い距離を残す
			if (dist < nearestDist)
			{
				nearestDist = dist;

				hitVPos[0] = vPos[0];
				hitVPos[1] = vPos[1];
				hitVPos[2] = vPos[2];

				//レイが当たったモデルの行列を保存
				hitMat = worldMat;
			}
		}
	}
	if (nearestDist == FLT_MAX)
	{
		return false;
	}
	else {
		_dis = nearestDist;

		//ポリゴンの３つの辺（フチ）のうちの２つを表すベクトルを求める
		Math::Vector3 vec1, vec2;
		vec1 = hitVPos[0] - hitVPos[2];
		vec2 = hitVPos[1] - hitVPos[2];

		//ポリゴンの２辺ともに垂直なベクトルを外積で求める
		//２辺ともに垂直==ポリゴン面に垂直なので法線となる
		_wallnormal = DirectX::XMVector3Cross(vec1, vec2);

		//ローカル座標上での法線を実際の3D空間の向きに変換する
		_wallnormal = DirectX::XMVector3TransformNormal(_wallnormal, hitMat);

		//法線は長さ1にする必要がある
		_wallnormal.Normalize();

		return true;
	}
}
