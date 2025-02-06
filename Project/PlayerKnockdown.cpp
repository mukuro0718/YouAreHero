#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "ReactionType.h"
#include "Character.h"
#include "Player.h"
#include "PlayerAction.h"
#include "VECTORtoUseful.h"
#include "PlayerKnockdown.h"
#include "EffectManager.h"
#include "EnemyManager.h"
#include "InputManager.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerKnockdown::PlayerKnockdown()
	: PlayerAction	()
	, MAX_SPEED		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["REACTION_SPEED"][static_cast<int>(Gori::PlayerReactionType::BLOW_BIG)])
	, DECEL			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["ACCEL"])
	, nowSpeed		(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->staminaRecoveryValue	= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"];
	this->maxStamina			= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->nextAnimation			= static_cast<int>(Player::AnimationType::DOWN_REACTION);
	this->playTime				= json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerKnockdown::~PlayerKnockdown()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerKnockdown::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerKnockdown::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerKnockdown::Update(Player& _player)
{
	/*�X�^�~�i�̉�*/
	_player.CalcStamina(this->staminaRecoveryValue, this->maxStamina);

	/*��]�̍X�V*/
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	VECTOR	nowRotation		= _player.GetRigidbody().GetRotation();
	VECTOR	nextRotation	= Gori::ORIGIN;
	VECTOR	enemyPosition	= enemy.GetRigidbody().GetPosition();
	VECTOR	positionToEnemy = VSub(_player.GetRigidbody().GetPosition(), enemyPosition);
			nextRotation.y	= static_cast<float>(atan2(static_cast<double>(positionToEnemy.x), static_cast<double>(positionToEnemy.z)));
			nowRotation		= Gori::LerpAngle(nowRotation, nextRotation, this->rotateLerpValue);
	_player.SetRotation(nowRotation, nextRotation);

	/*�ړ����x�̍X�V*/
	if (this->nowSpeed != 0)
	{
		this->nowSpeed += this->DECEL;
		if (this->nowSpeed > 0.0f)
		{
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::PLAYER_KNOCKDOWN);
			this->nowSpeed = 0.0f;
		}
		_player.SetSpeed(this->nowSpeed);
	}

	/*�ړ��x�N�g�����o��*/
	VECTOR nowVelocity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelocity, this->nowSpeed, false);
	_player.SetVelocity(newVelocity);

	/*�����̊J�n���Ɉ�x�����s������*/
	if (this->frameCount == 0)
	{
		//�G�t�F�N�g�̍Đ�
		auto& effect = Singleton<EffectManager>  ::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::BOSS_IMPACT);
		//�X�s�[�h�̐ݒ�
		const CharacterData& data = _player.GetCharacterData();
		this->nowSpeed = this->MAX_SPEED;
		//�q�b�g�X�g�b�v�̐ݒ�
		_player.SetHitStop(data.hitStopTime, data.hitStopType, data.hitStopDelay, data.slowFactor);
		//�q�b�g�t���O������
		_player.GetPlayerData().isHit = false;
		//���G�t���O�𗧂Ă�
		_player.GetPlayerData().isInvinvible = true;
	}



	/*�A�j���[�V�����̍Đ�*/
	bool isStopAnimation = false;
	if (this->frameCount != 0 && _player.GetIsChangeAnimation())
	{
		isStopAnimation = true;
	}
	if (!isStopAnimation)
	{
		_player.PlayAnimation(this->nextAnimation, this->playTime);
	}

	/*�A�j���[�V�����̍Đ����I�����Ă����瑁�����^�[��*/
	auto& input = Singleton<InputManager>  ::GetInstance();
	bool isChangeAnim = _player.GetIsChangeAnimation();
	if (isChangeAnim && (input.GetLStickState().XBuf != 0 || input.GetLStickState().YBuf != 0))
	{
		this->isEndAction = true;
	}

	/*�t���[���v��*/
	this->frameCount++;
}