#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_BackingBreath.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackCapsuleColliderData.h"
#include "AttackData.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"
#include "ReactionType.h"
#include "HitStop.h"
#include "EffectManager.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_BackingBreath::Beast_BackingBreath()
	: frameCount					 (0)
	, attackStartCount				 (0)
	, frameIndexUsedCapsuleDirection1(0)
	, frameIndexUsedCapsuleDirection2(0)
	, collider						 (nullptr)
	, breathLength					 (0.0f)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->actionType					  = static_cast<int>(BeastBehaviorTree::ActionType::BACKING_BREATH);
	this->interval						  = json.GetJson(JsonManager::FileType::BEAST)["ACTION_INTERVAL"][this->actionType];
	this->animationType					  = static_cast<int>(Beast::AnimationType::BACKING_BREATH);
	this->animationPlayTime				  = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->maxSpeed						  = json.GetJson(JsonManager::FileType::BEAST)["BACKING_BREATH_SPEED"];
	this->accel							  = json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel							  = json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->attackStartCount				  = json.GetJson(JsonManager::FileType::BEAST)["BACKING_BREATH_ATTACK_START_COUNT"];
	this->breathLength					  = json.GetJson(JsonManager::FileType::BEAST)["BACKING_BREATH_LENGTH"];
	this->frameIndexUsedCapsuleDirection1 = json.GetJson(JsonManager::FileType::BEAST)["BACKING_BREATH_FRAME_INDEX_USED_CAPSULE_DIRECTION"][0];
	this->frameIndexUsedCapsuleDirection2 = json.GetJson(JsonManager::FileType::BEAST)["BACKING_BREATH_FRAME_INDEX_USED_CAPSULE_DIRECTION"][1];

	/*�R���C�_�[�̍쐬*/
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BOSS_ATTACK, new AttackData());
	this->collider->radius						  = json.GetJson(JsonManager::FileType::BEAST)["BACKING_BREATH_RADIUS"];
	this->collider->data->damage				  = json.GetJson(JsonManager::FileType::BEAST)["BACKING_BREATH_DAMAGE"];
	this->collider->data->reactionType			  = static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
	this->collider->data->hitStopTime			  = json.GetJson(JsonManager::FileType::BEAST)["BACKING_BREATH_HIT_STOP_TIME"];
	this->collider->data->hitStopType			  = static_cast<int>(HitStop::Type::STOP);
	this->collider->data->hitStopDelay			  = json.GetJson(JsonManager::FileType::BEAST)["BACKING_BREATH_HIT_STOP_DELAY"];
	this->collider->data->slowFactor			  = json.GetJson(JsonManager::FileType::BEAST)["BACKING_BREATH_SLOW_FACTOR"];
	this->collider->data->isHitAttack			  = false;
	this->collider->data->isDoHitCheck			  = false;
	this->collider->data->blockStaminaConsumption = json.GetJson(JsonManager::FileType::BEAST)["SUPER_NOVA_BLOCK_STAMINA_CONSUMPTION"];

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_BackingBreath::~Beast_BackingBreath()
{

}
/// <summary>
/// ������
/// </summary>
void Beast_BackingBreath::Initialize()
{
	this->frameCount = 0;
	this->isFixRotate = false;
}

/// <summary>
/// �X�V����
/// </summary>
Beast_BackingBreath::NodeState Beast_BackingBreath::Update()
{
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	/*������*/
	if (rootNode.GetNowSelectAction() != this->actionType)
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		enemy.SetNowAnimation(this->animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		//�A�N�V�����̐ݒ�
		rootNode.SetSelectAction(this->actionType);
		//�A�N�V�����̓o�^
		rootNode.EntryCurrentBattleAction(*this);
		enemy.DecAttackComboCount();
	}

	/*�R���C�_�[�̍X�V*/
	//�w��t���[���𒴂��Ă��Ȃ���΃t���[���̑���
	if (this->frameCount < this->attackStartCount)
	{
		this->frameCount++;
		//�w��t���[���𒴂��Ă�����
		if (this->frameCount >= this->attackStartCount)
		{
			this->collider->data->isDoHitCheck = true;
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_BREATH);
			auto& effect = Singleton<EffectManager>::GetInstance();
			effect.OnIsEffect(EffectManager::EffectType::BEAST_BACKING_BREATH);
		}
		else if (this->frameCount % this->BREATH_HIT_COUNT == 0)
		{
			this->collider->data->isDoHitCheck = true;
		}
	}
	//�����蔻�肪�����Ԃ�������
	if (this->collider->data->isDoHitCheck)
	{
		VECTOR position1 = MV1GetFramePosition(enemy.GetModelHandle(), this->frameIndexUsedCapsuleDirection1);
		VECTOR position2 = MV1GetFramePosition(enemy.GetModelHandle(), this->frameIndexUsedCapsuleDirection2);
		this->collider->rigidbody.SetPosition(position1);
		VECTOR direction = VSub(position2, position1);
		direction.y = 0.0f;
		VECTOR directionScaling = VScale(direction, this->breathLength);
		this->collider->topPositon = VAdd(position1, directionScaling);
	}
	//�������Ă�����
	if (this->collider->data->isHitAttack)
	{
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_LIGHT_ATTACK);
		//�U���q�b�g�t���O������
		this->collider->data->isHitAttack = false;
	}

	/*�A�j���[�V�����̍Đ�*/
	enemy.PlayAnimation();

	/*�ړ�*/
	//�ړ����x�̍X�V
	enemy.UpdateSpeed(this->maxSpeed, this->decel, this->accel);
	//�ړ��x�N�g�����o��
	enemy.UpdateVelocity(false);

	/*��Ԃ�Ԃ�*/
	//�A�j���[�V�������I�����Ă�����
	if (enemy.GetIsChangeAnimation())
	{
		//�C���^�[�o���̐ݒ�
		rootNode.SetInterval(this->actionType, this->interval);
		//�A�N�V�����̉���
		rootNode.ExitCurrentBattleAction();
		this->frameCount = 0;
		this->collider->data->isDoHitCheck = false;
		this->collider->data->isHitAttack = false;
		return ActionNode::NodeState::SUCCESS;
	}
	//����ȊO�͎��s����Ԃ�
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}

/// <summary>
/// �`��
/// </summary>
const void Beast_BackingBreath::Draw()const
{
#ifdef _DEBUG
	DrawCapsule3D(this->collider->rigidbody.GetPosition(), this->collider->topPositon, this->collider->radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
#endif // _DEBUG

}
