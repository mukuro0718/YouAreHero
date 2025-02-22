#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_RightFootAttack.h"
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
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_RightFootAttack::Beast_RightFootAttack()
	: attackStartCount				 (0)
	, attackEndCount				 (0)
	, frameCount					 (0)
	, frameIndexUsedCapsuleDirection1(0)
	, frameIndexUsedCapsuleDirection2(0)
	, collider						 (nullptr)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationPlayTime				  = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->animationType					  = static_cast<int>(Beast::AnimationType::RIGHT_FOOT_ATTACK);
	this->actionType					  = static_cast<int>(BeastBehaviorTree::ActionType::RIGHT_FOOT_ATTACK);
	this->interval						  = json.GetJson(JsonManager::FileType::BEAST)["ACTION_INTERVAL"][this->actionType];
	this->maxSpeed						  = 0.0f;
	this->accel							  = json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel							  = json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->attackStartCount				  = json.GetJson(JsonManager::FileType::BEAST)["RIGHT_ATTACK_START_COUNT"];
	this->attackEndCount				  = json.GetJson(JsonManager::FileType::BEAST)["RIGHT_ATTACK_END_COUNT"];
	this->frameIndexUsedCapsuleDirection1 = json.GetJson(JsonManager::FileType::BEAST)["RIGHT_FRAME_INDEX_USED_CAPSULE_DIRECTION"][0];
	this->frameIndexUsedCapsuleDirection2 = json.GetJson(JsonManager::FileType::BEAST)["RIGHT_FRAME_INDEX_USED_CAPSULE_DIRECTION"][1];

	/*�R���C�_�[�̍쐬*/
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BOSS_ATTACK, new AttackData());
	this->collider->radius							= json.GetJson(JsonManager::FileType::BEAST)["RIGHT_RADIUS"];
	this->collider->data->damage					= json.GetJson(JsonManager::FileType::BEAST)["RIGHT_DAMAGE"];
	this->collider->data->reactionType				= static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
	this->collider->data->hitStopTime				= json.GetJson(JsonManager::FileType::BEAST)["RIGHT_HIT_STOP_TIME"];
	this->collider->data->hitStopType				= static_cast<int>(HitStop::Type::STOP);
	this->collider->data->hitStopDelay				= json.GetJson(JsonManager::FileType::BEAST)["RIGHT_HIT_STOP_DELAY"];
	this->collider->data->slowFactor				= json.GetJson(JsonManager::FileType::BEAST)["RIGHT_SLOW_FACTOR"];
	this->collider->data->isHitAttack				= false;
	this->collider->data->isDoHitCheck				= false;
	this->collider->data->blockStaminaConsumption	= json.GetJson(JsonManager::FileType::BEAST)["RIGHT_BLOCK_STAMINA_CONSUMPTION"];

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_RightFootAttack::~Beast_RightFootAttack()
{

}
/// <summary>
/// ������
/// </summary>
void Beast_RightFootAttack::Initialize()
{
	this->frameCount  = 0;
	this->isFixRotate = false;
}

/// <summary>
/// �X�V����
/// </summary>
Beast_RightFootAttack::NodeState Beast_RightFootAttack::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());

	/*�o�^����Ă���A�N�V�����Ǝ��ۂ̃A�N�V�������قȂ��Ă�����*/
	if (rootNode.GetNowSelectAction() != this->actionType)
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		enemy.SetNowAnimation(this->animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		//�A�N�V�����̐ݒ�
		rootNode.SetSelectAction(this->actionType);
		//������Root�ɓo�^
		rootNode.EntryCurrentBattleAction(*this);
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
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_SWING_1);
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
	if (enemy.GetSpeed() != 0.0f || this->frameCount == 1)
	{
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*��Ԃ�Ԃ�*/
	//�A�j���[�V�������I�����Ă�����
	if (enemy.GetIsChangeAnimation())
	{
		//�C���^�[�o���̐ݒ�
		rootNode.SetInterval(this->actionType, this->interval);
		//�o�^������
		rootNode.ExitCurrentBattleAction();
		this->frameCount = 0;
		this->collider->data->isDoHitCheck = false;
		this->collider->data->isHitAttack = false;
		return ActionNode::NodeState::SUCCESS;
	}
	return ActionNode::NodeState::RUNNING;
}

/// <summary>
/// �`��
/// </summary>
const void Beast_RightFootAttack::Draw()const
{
#ifdef _DEBUG
	DrawCapsule3D(this->collider->rigidbody.GetPosition(), this->collider->topPositon, this->collider->radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
#endif // _DEBUG
}
