#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerAvoid.h"
#include "UseJson.h"
#include "CharacterData.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerAvoid::PlayerAvoid()
	: PlayerAction	()
{
	auto& json		= Singleton<JsonManager>::GetInstance();
	this->maxStamina			= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->staminaRecoveryValue	= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"];
	this->firstSpeed			= json.GetJson(JsonManager::FileType::PLAYER)["ROLLING_SPEED"];
	this->staminaConsumpion		= json.GetJson(JsonManager::FileType::PLAYER)["AVOID_STAMINA_CONSUMPTION"];
	this->rotatableFrame		= json.GetJson(JsonManager::FileType::PLAYER)["AVOID_CAN_ROTATE_FRAME"];
	this->justAvoidFrame		= json.GetJson(JsonManager::FileType::PLAYER)["JUST_AVOID_MAX_FRAME"];
	this->nextAnimation			= static_cast<int>(Player::AnimationType::AVOID);
	this->playTime				= json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerAvoid::~PlayerAvoid()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerAvoid::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
	this->isRotate = false;

}

/// <summary>
/// �㏈��
/// </summary>
void PlayerAvoid::Finalize()
{
}

/// <summary>
/// �X�V
/// </summary>
void PlayerAvoid::Update(Player& _player)
{
	/*�J�n����̏���*/
	if (this->frameCount == 0)
	{
		_player.GetPlayerData().isInvinvible = true;
		_player.CalcStamina(this->staminaConsumpion, this->maxStamina);
		_player.SetSpeed(this->firstSpeed);
	}
	else
	{
		//�A�j���[�V�������I�����Ă��邩
		if (_player.GetIsChangeAnimation())
		{
			this->isEndAction = true;
			this->isRotate = false;
		}
	}

	/*�t���[���̌v��*/
	if (!this->isRotate)
	{
		//�t���[���J�E���g�̑���
		this->frameCount++;
		//�A�N�V�����L�����Z�����\��������
		if (this->frameCount >= this->rotatableFrame)
		{
			this->isChangeAction = true;
			this->isRotate = true;
		}
		//���G����
		if (this->frameCount >= this->justAvoidFrame && _player.GetPlayerData().isInvinvible)
		{
			_player.GetPlayerData().isInvinvible = false;
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::PLAYER_ROLL);
		}
	}

	/*�ړ�����*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, this->isRotate, false);
	Move(_player, data);

	/*�A�j���[�V�����̍Đ�*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);
}