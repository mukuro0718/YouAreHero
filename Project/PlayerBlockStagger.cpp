#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "Character.h"
#include "Player.h"
#include "PlayerAction.h"
#include "PlayerBlockStagger.h"
#include "EffectManager.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerBlockStagger::PlayerBlockStagger()
	: PlayerAction()
	, MAX_SPEED(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAGGER_SPEED"])
	, DECEL(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["ACCEL"])
{
	auto& json = Singleton<JsonManager>  ::GetInstance();
	this->staminaConsumption = json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"];
	this->maxStamina		 = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->nextAnimation		 = static_cast<int>(Player::AnimationType::BLOCK_REACTION);
	this->playTime			 = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerBlockStagger::~PlayerBlockStagger()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerBlockStagger::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
	this->nowSpeed = 0;
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerBlockStagger::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerBlockStagger::Update(Player& _player)
{
	/*�ړ����x�̍X�V*/
	if (this->nowSpeed != 0)
	{
		this->nowSpeed += this->DECEL;
		if (this->nowSpeed > 0.0f)
		{
			this->nowSpeed = 0.0f;
		}
		_player.SetSpeed(this->nowSpeed);
	}

	if (_player.GetSpeed() != 0)
	{
		MoveData data;
		data.Set(_player.GetNextRotation(), this->nowSpeed, true, false);
		Move(_player, data);
	}

	/*�����̊J�n���Ɉ�x�����s������*/
	if (this->frameCount == 0)
	{
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::PLAYER_BLOCK);
		//�G�t�F�N�g�̍Đ�
		auto& effect = Singleton<EffectManager>::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::PLAYER_GUARD_HIT);
		//�q�b�g�t���O������
		_player.GetPlayerData().isHit = false;
		//_player.GetPlayerData().isInvinvible = true;
		this->nowSpeed = this->MAX_SPEED;
		this->frameCount++;
	}

	/*�A�j���[�V�����̍Đ�*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);

	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
		//_player.GetPlayerData().isInvinvible = false;
	}
}