#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerRun.h"
#include "UseJson.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerRun::PlayerRun()
	: PlayerAction()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->maxSpeed				= json.GetJson(JsonManager::FileType::PLAYER)["RUN_SPEED"];
	this->staminaRecoveryValue	= json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"];
	this->maxStamina			= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->nextAnimation			= static_cast<int>(Player::AnimationType::RUN_FRONT);
	this->playTime				= json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerRun::~PlayerRun()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerRun::Initialize()
{

}

/// <summary>
/// �㏈��
/// </summary>
void PlayerRun::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerRun::Update(Player& _player)
{
	auto& sound = Singleton<SoundManager>::GetInstance();
	if (!sound.GetCheckEffectSoundState(SoundManager::EffectType::PLAYER_RUN))
	{
		sound.OnIsPlayEffect(SoundManager::EffectType::PLAYER_RUN);
	}

	this->isEndAction = true;

	/*�ړ������i�ړ������Ȃ��ꍇ�ł��A���x�̌���������̂ŏ������s���j*/
	MoveData data;
	data.Set(Gori::ORIGIN, this->maxSpeed, false, true);
	Move(_player, data);

	/*�X�^�~�i�̌v�Z*/
	_player.CalcStamina(this->staminaRecoveryValue, this->maxStamina);
	
	/*�A�j���[�V�����̍Đ�*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);
}