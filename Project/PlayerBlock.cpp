#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "CharacterData.h"
#include "PlayerBlock.h"
#include "EnemyManager.h"
#include "InputManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerBlock::PlayerBlock()
	: PlayerAction()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->staminaRecoveryValue	= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"];
	this->maxStamina			= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->maxSpeed				= json.GetJson(JsonManager::FileType::PLAYER)["NONE_STAMINA_RUN_SPEED"];
	this->enemyFirstDirection	= Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_DIRECTION"]);
	this->playerFirstDirection	= Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["FIRST_DIRECTION"]);
	this->toleranceDot			= json.GetJson(JsonManager::FileType::PLAYER)["TOLERANCE_DOT"];
	this->staminaConsumption	= json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"];
	this->nextAnimation			= static_cast<int>(Player::AnimationType::BLOCK);
	this->playTime				= json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerBlock::~PlayerBlock()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerBlock::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerBlock::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerBlock::Update(Player& _player)
{
	/*��]����*/
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	if (this->frameCount < 10)
	{
		if (!_player.GetIsLock())
		{
			enemy.SetNearestEnemyIndent(_player.GetRigidbody().GetPosition());
		}
		VECTOR  nextRotation	= Gori::ORIGIN;
		VECTOR  enemyPosition	= enemy.GetPositionForLockon();
		VECTOR  positionToEnemy = VSub(_player.GetRigidbody().GetPosition(), enemyPosition);
				nextRotation.y	= static_cast<float>(atan2(static_cast<double>(positionToEnemy.x), static_cast<double>(positionToEnemy.z)));
				nowRotation		= Gori::LerpAngle(nowRotation, nextRotation, this->rotateLerpValue);
		if (VSquareSize(VSub(enemy.GetPositionForLockon(), _player.GetRigidbody().GetPosition())) <= 400.0f)
		{
			_player.SetRotation(nowRotation, nextRotation);
		}
	}

	/*�ړ����x�̍X�V*/
	_player.SetSpeed(0.0f);

	/*�ړ��x�N�g�����o��*/
	VECTOR nowVelocity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelocity, 0.0f, false);
	_player.SetVelocity(newVelocity);

	/*�K�[�h���������Ă��邩�̔��������*/
	VECTOR enemyDirection = VTransform(this->enemyFirstDirection, MGetRotY(enemy.GetRigidbody().GetRotation().y));	 //���̃G�l�~�[�̌���
	enemyDirection = VNorm(enemyDirection);
	VECTOR playerDirection = VTransform(this->playerFirstDirection, MGetRotY(_player.GetRigidbody().GetRotation().y));//���̃v���C���[�̌���
	playerDirection = VNorm(playerDirection);
	float  enemyAndPlayerDot = VDot(enemyDirection, playerDirection);
	enemyAndPlayerDot = enemyAndPlayerDot * 180.0f / DX_PI_F;
	//�������ς��}�C�i�X�Ȃ�{�ɖ߂��Ĕ��肷��
	if (enemyAndPlayerDot < 0) { enemyAndPlayerDot *= -1.0f; }
	//���ς����e�͈͂��X�^�~�i������Ă�����K�[�h����
	if (enemyAndPlayerDot >= this->toleranceDot && _player.CanAction(this->staminaConsumption))
	{
		_player.GetPlayerData().isGuard = true;
	}
	else
	{
		_player.GetPlayerData().isGuard = false;
	}

	/*�A�j���[�V�����̍Đ�*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);

	this->isEndAction = true;
}