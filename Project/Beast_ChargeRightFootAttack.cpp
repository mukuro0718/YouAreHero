#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackCapsuleColliderData.h"
#include "AttackData.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Beast_ChargeRightFootAttack.h"
#include "Player.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "BeastBehaviorTree.h"
#include "ReactionType.h"
#include "HitStop.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_ChargeRightFootAttack::Beast_ChargeRightFootAttack()
	: attackStartCount				 (0)
	, attackEndCount				 (0)
	, frameCount					 (0)
	, frameIndexUsedCapsuleDirection1(0)
	, frameIndexUsedCapsuleDirection2(0)
	, collider						 (nullptr)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationPlayTime				  = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->animationType					  = static_cast<int>(Beast::AnimationType::CHARGE_RIGHT_ATTACK);
	this->actionType					  = static_cast<int>(BeastBehaviorTree::ActionType::CHARGE_RIGHT_ATTACK);
	this->interval						  = json.GetJson(JsonManager::FileType::BEAST)["ACTION_INTERVAL"][this->actionType];
	this->maxSpeed						  = json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->accel							  = json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel							  = json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->attackStartCount				  = json.GetJson(JsonManager::FileType::BEAST)["CHARGE_RIGHT_ATTACK_START_COUNT"];
	this->attackEndCount				  = json.GetJson(JsonManager::FileType::BEAST)["CHARGE_RIGHT_ATTACK_END_COUNT"];
	this->frameIndexUsedCapsuleDirection1 = json.GetJson(JsonManager::FileType::BEAST)["CHARGE_RIGHT_FRAME_INDEX_USED_CAPSULE_DIRECTION"][0];
	this->frameIndexUsedCapsuleDirection2 = json.GetJson(JsonManager::FileType::BEAST)["CHARGE_RIGHT_FRAME_INDEX_USED_CAPSULE_DIRECTION"][1];

	/*�R���C�_�[�̍쐬*/
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BEAST, new AttackData());
	this->collider->radius						  = json.GetJson(JsonManager::FileType::BEAST)["CHARGE_RIGHT_RADIUS"];
	this->collider->data->damage				  = json.GetJson(JsonManager::FileType::BEAST)["CHARGE_RIGHT_DAMAGE"];
	this->collider->data->reactionType			  = static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
	this->collider->data->hitStopTime			  = json.GetJson(JsonManager::FileType::BEAST)["CHARGE_RIGHT_HIT_STOP_TIME"];
	this->collider->data->hitStopType			  = static_cast<int>(HitStop::Type::STOP);
	this->collider->data->hitStopDelay			  = json.GetJson(JsonManager::FileType::BEAST)["CHARGE_RIGHT_HIT_STOP_DELAY"];
	this->collider->data->slowFactor			  = json.GetJson(JsonManager::FileType::BEAST)["CHARGE_RIGHT_SLOW_FACTOR"];
	this->collider->data->isHitAttack			  = false;
	this->collider->data->isDoHitCheck			  = false;
	this->collider->data->blockStaminaConsumption = json.GetJson(JsonManager::FileType::BEAST)["CHARGE_RIGHT_BLOCK_STAMINA_CONSUMPTION"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_ChargeRightFootAttack::~Beast_ChargeRightFootAttack()
{

}

/// <summary>
/// ������
/// </summary>
void Beast_ChargeRightFootAttack::Initialize()
{
	this->frameCount  = 0;
	this->isFixRotate = false;
}

/// <summary>
/// �X�V����
/// </summary>
Beast_ChargeRightFootAttack::NodeState Beast_ChargeRightFootAttack::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<Beast&>(_chara);

	/*�o�^����Ă���A�N�V�����Ǝ��ۂ̃A�N�V�������قȂ��Ă�����*/
	if (_tree.GetNowSelectAction() != this->actionType)
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		enemy.SetNowAnimation(this->animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		//�A�N�V�����̐ݒ�
		_tree.SetNowSelectAction(this->actionType);
		//������Root�ɓo�^
		_tree.EntryCurrentBattleAction(*this);
		enemy.DecAttackComboCount();
	}

	/*�R���C�_�[�̍X�V*/
	//�w��t���[���𒴂��Ă��Ȃ���΃t���[���̑���
	if (this->frameCount < this->attackEndCount)
	{
		this->frameCount++;
		//�w��t���[���𒴂��Ă�����
		if (this->frameCount == this->attackStartCount)
		{
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_SWING_2);
			this->collider->data->isDoHitCheck = true;
		}
		if (this->frameCount >= this->attackEndCount)
		{
			this->collider->data->isDoHitCheck = false;
			this->collider->data->isHitAttack = false;
		}
	}
	//�����蔻�肪�����Ԃ�������
	if (this->collider->data->isDoHitCheck)
	{
		VECTOR position1 = MV1GetFramePosition(enemy.GetModelHandle(), this->frameIndexUsedCapsuleDirection1);
		VECTOR position2 = MV1GetFramePosition(enemy.GetModelHandle(), this->frameIndexUsedCapsuleDirection2);
		this->collider->rigidbody.SetPosition(position1);
		this->collider->topPositon = position2;
	}
	//�������Ă�����q�b�g�X�g�b�v��ݒ肷��
	if (this->collider->data->isHitAttack)
	{
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_HEAVY_ATTACK);
		//�U���q�b�g�t���O������
		this->collider->data->isHitAttack = false;
	}

	/*�A�j���[�V�����̍Đ�*/
	enemy.PlayAnimation();

	/*�ړ�*/
	float maxSpeed = 0.0f;
	if (this->frameCount < this->attackStartCount)
	{
		maxSpeed = this->maxSpeed;
		//�ڕW�܂ł̃x�N�g�����o��
		auto& player = Singleton<PlayerManager>::GetInstance();
		VECTOR toTarget = VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition());
		//���K��
		//toTarget = VNorm(toTarget);
		enemy.UpdateRotation(toTarget);
	}
	//�ړ����x�̍X�V
	enemy.UpdateSpeed(maxSpeed, this->accel, this->decel);
	//�ړ��x�N�g�����o��
	enemy.UpdateVelocity(false);

	/*��Ԃ�Ԃ�*/
	{
		//�A�j���[�V�������I�����Ă�����
		if (enemy.GetIsChangeAnimation())
		{
			//�C���^�[�o���̐ݒ�
			_tree.SetInterval(this->actionType, this->interval);
			//�o�^������
			_tree.ExitCurrentBattleAction();
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
}

/// <summary>
/// �`��
/// </summary>
const void Beast_ChargeRightFootAttack::Draw()const
{
#ifdef _DEBUG
	DrawCapsule3D(this->collider->rigidbody.GetPosition(), this->collider->topPositon, this->collider->radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
#endif // _DEBUG
}
