#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerWalk.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerWalk::PlayerWalk()
	: PlayerAction()
{
	auto& json					= Singleton<JsonManager>::GetInstance();
	this->maxSpeed				= json.GetJson(JsonManager::FileType::PLAYER)["WALK_SPEED"];
	this->staminaRecoveryValue	= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"];
	this->maxStamina			= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->nextAnimation			= static_cast<int>(Player::AnimationType::WALK_FRONT);
	this->playTime				= json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerWalk::~PlayerWalk()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerWalk::Initialize()
{

}

/// <summary>
/// �㏈��
/// </summary>
void PlayerWalk::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerWalk::Update(Player& _player)
{
	auto& sound = Singleton<SoundManager>::GetInstance();
	if (!sound.GetCheckEffectSoundState(SoundManager::EffectType::PLAYER_WALK))
	{
		sound.OnIsPlayEffect(SoundManager::EffectType::PLAYER_WALK);
	}

	this->isEndAction = true;

	/*�ړ������i�ړ������Ȃ��ꍇ�ł��A���x�̌���������̂ŏ������s���j*/
	MoveData data;
	data.Set(Gori::ORIGIN, this->maxSpeed, false, false);
	Move(_player, data);

	/*�X�^�~�i�̉�*/
	_player.CalcStamina(this->staminaRecoveryValue, this->maxStamina);

	/*�A�j���[�V�����̍Đ�*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);
}