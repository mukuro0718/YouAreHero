#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerRunOutOfStamina.h"
#include "UseJson.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerRunOutOfStamina::PlayerRunOutOfStamina()
	: PlayerAction()
{
	auto& json	 = Singleton<JsonManager>::GetInstance();
	this->maxSpeed			 = json.GetJson(JsonManager::FileType::PLAYER)["NONE_STAMINA_RUN_SPEED"];
	this->staminaConsumption = json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"];
	this->maxStamina		 = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->nextAnimation		 = static_cast<int>(Player::AnimationType::RUN_FRONT);
	this->playTime			 = json.GetJson(JsonManager::FileType::PLAYER)["RUN_OUT_OF_STAMINA_PLAY_TIME"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerRunOutOfStamina::~PlayerRunOutOfStamina()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerRunOutOfStamina::Initialize()
{

}

/// <summary>
/// �㏈��
/// </summary>
void PlayerRunOutOfStamina::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerRunOutOfStamina::Update(Player& _player)
{
	/*�ړ������i�ړ������Ȃ��ꍇ�ł��A���x�̌���������̂ŏ������s���j*/
	MoveData data;
	data.Set(Gori::ORIGIN, this->maxSpeed, false, true);
	Move(_player, data);

	/*�X�^�~�i�̌v�Z*/
	_player.CalcStamina(this->staminaConsumption, this->maxStamina);
	
	/*�A�j���[�V�����̍Đ�*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);

	this->isEndAction = true;
}