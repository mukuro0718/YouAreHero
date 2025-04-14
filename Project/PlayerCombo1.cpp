#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackData.h"
#include "AttackSphereColliderData.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerCombo1.h"
#include "EffectManager.h"
#include "EnemyManager.h"
#include "HitStop.h"
#include "SoundManager.h"
#include "ReactionType.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerCombo1::PlayerCombo1()
	: PlayerAction				()
	, FIRST_PLAY_TIME			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_START_ANIM_PLAY_TIME"])
	, CANCELABLE_PLAY_TIME		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_CANCEL_PLAY_TIME"])
	, START_HIT_CHECK_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_START_HIT_CHECK_PLAY_TIME"])
	, END_HIT_CHECK_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_END_HIT_CHECK_PLAY_TIME"])
	, POSITION_OFFSET			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET"])
	, Y_OFFSET					(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET_Y"])
	, HIT_STOP_TIME				(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_HIT_STOP_TIME"])
	, HIT_STOP_DELAY			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_HIT_STOP_DELAY"])
	, HIT_STOP_TYPE				(static_cast<int>(HitStop::Type::STOP))
	, SLOW_FACTOR				(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_HIT_STOP_FACTOR"])
	, STAMINA_CONSUMPTION		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_STAMINA_CONSUMPTION"])
	, MOVE_SPEED				(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["RUN_SPEED"])
	, VIBRATION_POWER			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_VIBRATION_POWER"])
	, VIBRATION_TIME			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_VIBRATION_TIME"])
	, firstDirection			(Gori::ORIGIN)
	, isStartHitCheck			(false)
	, collider					(nullptr)
{
	/*�R���C�_�[�f�[�^�̍쐬*/
	this->collider = new AttackSphereColliderData(ColliderData::Priority::STATIC, GameObjectTag::PLAYER, new AttackData());

	/*������*/
	auto& json = Singleton<JsonManager>::GetInstance();
	this->collider->radius			   = json.GetJson(JsonManager::FileType::PLAYER)["COMBO1_ATTACK_RADIUS"];
	this->collider->data->damage	   = json.GetJson(JsonManager::FileType::PLAYER)["W_ATTACK_DAMAGE"][0];	
	this->collider->data->reactionType = static_cast<int>(Gori::PlayerReactionType::NORMAL);
	this->collider->data->hitStopTime  = this->HIT_STOP_TIME;
	this->collider->data->hitStopType  = this->HIT_STOP_TYPE;
	this->collider->data->hitStopDelay = this->HIT_STOP_DELAY;
	this->collider->data->slowFactor   = this->SLOW_FACTOR;
	this->collider->data->isHitAttack  = false;
	this->collider->data->isDoHitCheck = false;

	this->nextAnimation				  = static_cast<int>(Player::AnimationType::COMBO_1);
	this->playTime					  = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->firstDirection			  = Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["FIRST_DIRECTION"]);
	this->totalPlayTime				  = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_TOTAL_PLAY_TIME"][this->nextAnimation];
	this->maxStamina				  = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerCombo1::~PlayerCombo1()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerCombo1::Initialize()
{
	this->isChangeAction	= false;
	this->isEndAction		= false;
	this->frameCount		= 0;
	this->nowTotalPlayTime	= 0;
	this->collider->rigidbody.Initialize(false);
	this->collider->rigidbody.SetPosition(VGet(0.0f, 500.0f, 0.0f));
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerCombo1::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerCombo1::Update(Player& _player)
{
	/*��]����*/
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	float speed = 0.0f;
	if (this->frameCount < 10)
	{
		auto& enemy = Singleton<EnemyManager>::GetInstance();
		if (!_player.GetIsLock())
		{
			enemy.SetNearestEnemyIndent(_player.GetRigidbody().GetPosition());
		}
		VECTOR	nextRotation	= Gori::ORIGIN;
		VECTOR	enemyPosition	= enemy.GetPositionForLockon();
		VECTOR	positionToEnemy = VSub(_player.GetRigidbody().GetPosition(), enemyPosition);
				nextRotation.y	= static_cast<float>(atan2(static_cast<double>(positionToEnemy.x), static_cast<double>(positionToEnemy.z)));
				nowRotation		= Gori::LerpAngle(nowRotation, nextRotation, this->rotateLerpValue);
		if (VSquareSize(VSub(enemy.GetPositionForLockon(), _player.GetRigidbody().GetPosition())) <= 400.0f)
		{
			_player.SetRotation(nowRotation, nextRotation);
			speed = MOVE_SPEED;
		}
	}

	/*�ړ����x�̍X�V*/
	_player.SetSpeed(speed);

	/*�ړ��x�N�g�����o��*/
	VECTOR nowVelocity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelocity, speed, false);
	_player.SetVelocity(newVelocity);

	/*�A�j���[�V�����̍Đ�*/
	float playTime = this->playTime;
	if (this->nowTotalPlayTime == 0)
	{
		//�X�^�~�i���������
		_player.CalcStamina(this->STAMINA_CONSUMPTION, this->maxStamina);
		//�ŏ������Đ����Ԃ�ύX����
		playTime = this->FIRST_PLAY_TIME;
	}
	this->nowTotalPlayTime += playTime;
	_player.PlayAnimation(this->nextAnimation, playTime);

	/*�L�����Z���t���O�������Ă��Ȃ��Ƃ��ɃL�����Z���\�܂ŃA�j���[�V�������i��ł�����L�����Z���t���O�𗧂Ă�*/
	if (!this->isChangeAction)
	{
		if (this->nowTotalPlayTime >= this->CANCELABLE_PLAY_TIME)
		{
			this->isChangeAction = true;
			this->isStartHitCheck = false;
		}
	}

	/*�A�j���[�V�������I�����Ă����瑁�����^�[��*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}

	/*�����蔻��J�n�ȏォ�I���������܂ŃA�j���[�V�������i��ł��Ȃ���Α������^�[��*/
	if (this->nowTotalPlayTime < this->START_HIT_CHECK_PLAY_TIME || this->nowTotalPlayTime > this->END_HIT_CHECK_PLAY_TIME) return;

	/*�����蔻�肪�J�n���Ă���Ƃ��ɁA�����蔻��t���O����x�������Ă��Ȃ�������t���O�𗧂Ă�*/
	if (!this->isStartHitCheck)
	{
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::PLAYER_COMBO_1_SWING);
		this->collider->data->isDoHitCheck = true;
		this->isStartHitCheck = true;
	}

	/*�U�����������Ă�����G�t�F�N�g�ƃT�E���h�̍Đ��ƃq�b�g�X�g�b�v�̐ݒ�*/
	if (this->collider->data->isHitAttack)
	{
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::PLAYER_COMBO_1);
		auto& effect = Singleton<EffectManager>::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::PLAYER_IMPACT);
		effect.SetPosition(EffectManager::EffectType::PLAYER_IMPACT, this->collider->rigidbody.GetPosition());
		this->collider->data->isHitAttack = false;
		_player.SetHitStop(this->HIT_STOP_TIME, this->HIT_STOP_TYPE, 0, this->SLOW_FACTOR);
		StartJoypadVibration(DX_INPUT_PAD1, this->VIBRATION_POWER, this->VIBRATION_TIME);
	}

	/*�����蔻�苖�t���O�������Ă��Ȃ������瑁�����^�[��*/
	if (!this->collider->data->isDoHitCheck) return;

	/*�����蔻����W�̍X�V*/
	VECTOR	direction	= VTransform(this->firstDirection, MGetRotY(_player.GetRigidbody().GetRotation().y));	//�����̐ݒ�
	VECTOR	position	= _player.GetRigidbody().GetPosition();													//�v���C���[�̍��W
			position	= VAdd(position, VScale(direction, this->POSITION_OFFSET));								//�v���C���[�̍��W�ɁA�I�t�Z�b�g�l�𑫂�
			position.y += this->Y_OFFSET;																		//Y���W�I�t�Z�b�g�l�𑫂�
	this->collider->rigidbody.SetPosition(position);

	//�Đ����Ԃ��萔�𒴂��Ă���A�����蔻��t���O���~��Ă����瓖���蔻��J�n�t���O������
	if (this->nowTotalPlayTime > this->END_HIT_CHECK_PLAY_TIME)
	{
		this->collider->data->isDoHitCheck = false;
	}

#ifdef _DEBUG
	DrawSphere3D(this->collider->rigidbody.GetPosition(), this->collider->radius, 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
#endif // _DEBUG

}