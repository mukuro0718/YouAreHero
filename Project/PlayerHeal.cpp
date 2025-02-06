#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerHeal.h"
#include "UseJson.h"
#include "Debug.h"
#include "EffectManager.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerHeal::PlayerHeal()
	: PlayerAction		()
	, HEAL_VALUE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["HEAL_VALUE"])
	, MAX_HP			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["HP"])
	, DO_HEAL_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["DO_HEAL_PLAY_TIME"])
	, MAX_HEAL_VALUE	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["MAX_HEAL_VALUE"])
	, nowTotalHealValue	(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->staminaRecoveryValue	= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"];
	this->maxStamina			= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->nextAnimation			= static_cast<int>(Player::AnimationType::HEAL);
	this->playTime				= json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerHeal::~PlayerHeal()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerHeal::Initialize()
{
	this->isChangeAction	= false;
	this->isEndAction		= false;
	this->frameCount		= 0;
	this->nowTotalHealValue = 0;
	this->nowTotalPlayTime  = 0;
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerHeal::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerHeal::Update(Player& _player)
{
	/*�X�^�~�i�̉�*/
	_player.CalcStamina(this->staminaRecoveryValue, this->maxStamina);

	/*�J�n���ɍs������*/
	if (this->frameCount == 0)
	{
		//�T�E���h�G�t�F�N�g�̍Đ�
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::PLAYER_HEAL);
		//�񕜉񐔂̌���
		int healCount = _player.GetHealCount();
		healCount--;
		_player.SetHealCount(healCount);
		//�񕜃G�t�F�N�g�̍Đ�
		auto& effect = Singleton<EffectManager> ::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::PLAYER_HEAL);
		this->frameCount++;
	}

	/*HP����*/
	if (this->nowTotalPlayTime > this->DO_HEAL_PLAY_TIME)
	{
		if (this->nowTotalHealValue < this->MAX_HEAL_VALUE)
		{
			//HP���񕜁i�ő�𒴂��Ȃ��悤�ɂ���j
			int hp = _player.GetPlayerData().hp;
			hp += this->HEAL_VALUE;
			this->nowTotalHealValue += this->HEAL_VALUE;
			if (hp >= this->MAX_HP)
			{
				hp = this->MAX_HP;
			}
			_player.GetPlayerData().hp = hp;
		}
	}

	/*��]�̍X�V*/
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	VECTOR nextRotation = _player.GetNextRotation();
	UpdateRotation(true, nextRotation, nowRotation);
	_player.SetRotation(nowRotation, nextRotation);

	/*�ړ����x�̍X�V*/
	_player.SetSpeed(0.0f);

	/*�ړ��x�N�g�����o��*/
	VECTOR nowVelocity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelocity, 0.0f, false);
	_player.SetVelocity(newVelocity);

	/*�A�j���[�V�����̍Đ�*/
	this->nowTotalPlayTime += this->playTime;
	_player.PlayAnimation(this->nextAnimation, this->playTime);

	/*�A�j���[�V�������I�����Ă�����*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}