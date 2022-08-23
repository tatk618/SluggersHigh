#include "../../States/list/Statelist.h"
#include "../../player/Player.h"

playerBlowState::playerBlowState(const Math::Vector3& _targetVec)
{
	target_vec = _targetVec;

	//y•ûŒü‚ğ‚Â‚Ô‚·
	target_vec.y = 0;

	//‚Á”ò‚Ô•ûŒü‚ğ³‹K‰»
	target_vec.Normalize();

	//‚Á”ò‚Ô‘¬“x
	target_vec *= 1.0f;

	//‚Á”ò‚ñ‚Å‚©‚çÁ–Å‚·‚éŠÔ
	death_time = 1 * 60;
}

Stateclass* playerBlowState::Action(Math::Matrix& _mat, C_Player* _player, Math::Matrix& _player_mat, float _playerR, int _playerhp)
{
	Math::Matrix tmpMat;
	tmpMat = DirectX::XMMatrixTranslation(target_vec.x, target_vec.y, target_vec.z);

	_mat *= tmpMat;

	death_time--;

	//c‚èŠÔ‚ª‚O‚É‚È‚Á‚½‚ç
	if (death_time <= 0)
	{
		return nullptr;
	}

}
