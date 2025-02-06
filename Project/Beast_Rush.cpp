#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackCapsuleColliderData.h"
#include "AttackData.h"
#include "Character.h"
#include "Enemy.h"
#include "Player.h"
#include "Beast.h"
#include "Beast_Rush.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "BeastBehaviorTree.h"
#include "ReactionType.h"
#include "HitStop.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_Rush::Beast_Rush()
	: attackStartCount				 (0)
	, attackEndCount				 (0)
	, frameCount					 (0)
	, frameIndexUsedCapsuleDirection1(0)
	, frameIndexUsedCapsuleDirection2(0)
	, stage							 (AnimationStage::START)
	, collider						 (nullptr)
	, STOP_DISTANCE					 (Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::BEAST)["STOP_RUSH_DISTANCE"])
	, FOOTSTEPS_SOUND_FRAME			 (Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::BEAST)["RUSH_FOOTSTEPS_SOUND_FRAME"])
{
	this->animationSet.emplace(AnimationStage::START, static_cast<int>(Beast::AnimationType::RUSH_START));
	this->animationSet.emplace(AnimationStage::LOOP, static_cast<int>(Beast::AnimationType::RUSH_LOOP));
	this->animationSet.emplace(AnimationStage::NORMAL_END, static_cast<int>(Beast::AnimationType::RUSH_NORMAL_END));
	this->animationSet.emplace(AnimationStage::ANGRY_END, static_cast<int>(Beast::AnimationType::RUSH_ROTATE_END));
	this->nextStageSet.emplace(AnimationStage::START, AnimationStage::LOOP);
	this->nextStageSet.emplace(AnimationStage::LOOP, AnimationStage::NORMAL_END);
	this->nextStageSet.emplace(AnimationStage::NORMAL_END, AnimationStage::START);
	this->nextStageSet.emplace(AnimationStage::ANGRY_END, AnimationStage::START);
	auto& json = Singleton<JsonManager>::GetInstance();
	this->actionType					  = static_cast<int>(BeastBehaviorTree::ActionType::BREATH);
	this->interval						  = json.GetJson(JsonManager::FileType::BEAST)["ACTION_INTERVAL"][this->actionType];
	this->maxSpeed						  = json.GetJson(JsonManager::FileType::BEAST)["RUSH_SPEED"];
	this->accel							  = json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel							  = json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->attackStartCount				  = json.GetJson(JsonManager::FileType::BEAST)["RUSH_ATTACK_START_COUNT"];
	this->attackEndCount				  = json.GetJson(JsonManager::FileType::BEAST)["RUSH_ATTACK_END_COUNT"];
	this->frameIndexUsedCapsuleDirection1 = json.GetJson(JsonManager::FileType::BEAST)["RUSH_FRAME_INDEX_USED_CAPSULE_DIRECTION"][0];
	this->frameIndexUsedCapsuleDirection2 = json.GetJson(JsonManager::FileType::BEAST)["RUSH_FRAME_INDEX_USED_CAPSULE_DIRECTION"][1];

	/*�R���C�_�[�̍쐬*/
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BOSS_ATTACK, new AttackData());
	this->collider->radius						  = json.GetJson(JsonManager::FileType::BEAST)["RUSH_RADIUS"];
	this->collider->data->damage				  = json.GetJson(JsonManager::FileType::BEAST)["RUSH_DAMAGE"];
	this->collider->data->reactionType			  = static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
	this->collider->data->hitStopTime			  = json.GetJson(JsonManager::FileType::BEAST)["RUSH_HIT_STOP_TIME"];
	this->collider->data->hitStopType			  = static_cast<int>(HitStop::Type::STOP);
	this->collider->data->hitStopDelay			  = json.GetJson(JsonManager::FileType::BEAST)["RUSH_STOP_DELAY"];
	this->collider->data->slowFactor			  = json.GetJson(JsonManager::FileType::BEAST)["RUSH_SLOW_FACTOR"];
	this->collider->data->isHitAttack			  = false;
	this->collider->data->isDoHitCheck			  = false;
	this->collider->data->blockStaminaConsumption = json.GetJson(JsonManager::FileType::BEAST)["RUSH_BLOCK_STAMINA_CONSUMPTION"];

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_Rush::~Beast_Rush()
{

}
/// <summary>
/// ������
/// </summary>
void Beast_Rush::Initialize()
{
	this->frameCount  = 0;
	this->isFixRotate = false;
	this->stage		  = AnimationStage::START;
}

/// <summary>
/// �X�V����
/// </summary>
Beast_Rush::NodeState Beast_Rush::Update()
{
	/*�I������Ă���A�N�V�����Ǝ��ۂ̃A�N�V�������قȂ��Ă����珉����*/
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	if (rootNode.GetNowSelectAction() != this->actionType)
	{
		//�A�N�V�����̐ݒ�
		rootNode.SetSelectAction(this->actionType);
		//�A�N�V�����̓o�^
		rootNode.EntryCurrentBattleAction(*this);
	}

	/*�R���C�_�[�̍X�V*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());
	//�������Ă�����
	if (this->collider->data->isHitAttack)
	{
		//�U���q�b�g�t���O������
		this->collider->data->isHitAttack = false;
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_HEAVY_ATTACK);
	}
	//�A�j���[�V������LOOP�������炩�t���[���J�E���g��1�������瓖���蔻��t���O�𗧂Ă�
	if (this->stage == AnimationStage::LOOP)
	{
		this->frameCount++;
		//�w��t���[���𒴂��Ă�����
		if (this->frameCount == 1)
		{
			this->collider->data->isDoHitCheck = true;
		}
		//�Ԋu�ő�����炷
		if (this->frameCount % this->FOOTSTEPS_SOUND_FRAME == 0)
		{
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_FOOTSTEPS);
		}
	}
	else
	{
		this->collider->data->isDoHitCheck = false;
		this->collider->data->isHitAttack = false;
	}
	//�����蔻�肪�����Ԃ�������
	if (this->collider->data->isDoHitCheck)
	{
		VECTOR position1 = MV1GetFramePosition(enemy.GetModelHandle(), this->frameIndexUsedCapsuleDirection1);
		VECTOR position2 = MV1GetFramePosition(enemy.GetModelHandle(), this->frameIndexUsedCapsuleDirection2);
		this->collider->rigidbody.SetPosition(position1);
		this->collider->topPositon = position2;
	}

	/*�A�j���[�V����*/
	//�A�j���[�V�����̎�ނ�ݒ�
	int nowAnimationType = this->animationSet[this->stage];
	if (this->animationType != nowAnimationType)
	{
		//�Đ�����A�j���[�V������ݒ�
		this->animationType = nowAnimationType;
		enemy.SetNowAnimation(this->animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		auto& json = Singleton<JsonManager>::GetInstance();
		enemy.SetAnimationPlayTime(json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType]);
	}
	//�A�j���[�V�����̍Đ�
	enemy.PlayAnimation();

	/*�ړ�*/
	float maxSpeed = 0.0f;
	//�����A�j���[�V������START�Ȃ�ړ��ڕW���X�V���ĉ�]
	if (this->stage == AnimationStage::START)
	{
		auto& player = Singleton<PlayerManager>::GetInstance();
		enemy.SetNowMoveTarget(player.GetRigidbody().GetPosition());
		VECTOR toTarget = VSub(enemy.GetRigidbody().GetPosition(), enemy.GetNowMoveTarget());
		enemy.UpdateRotation(toTarget);
	}
	//�����A�j���[�V������LOOP���Ȃ瑬�x������
	else if (this->stage == AnimationStage::LOOP)
	{
		maxSpeed = this->maxSpeed;
	}
	//�ړ����x�̍X�V
	enemy.UpdateSpeed(maxSpeed, this->accel, this->decel);
	//�ړ��x�N�g�����o��
	enemy.UpdateVelocity(false);

	/*��Ԃ�Ԃ�*/
	//�A�j���[�V�������I�����Ă�����
	if (this->stage == AnimationStage::LOOP)
	{
		VECTOR toTarget = VSub(enemy.GetRigidbody().GetPosition(), enemy.GetNowMoveTarget());
		if (VSquareSize(toTarget) < this->STOP_DISTANCE)
		{
			this->stage = AnimationStage::NORMAL_END;
		}
		return ActionNode::NodeState::RUNNING;
	}
	else if (enemy.GetIsChangeAnimation())
	{
		this->stage = this->nextStageSet[this->stage];
		//�����ŃX�e�[�W��START�Ȃ�A�j���[�V���������ׂďI�������Ƃ�������
		if (this->stage == AnimationStage::START)
		{
			auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
			//�C���^�[�o���̃Z�b�g
			rootNode.SetInterval(this->actionType, this->interval);
			//�A�N�V�����̉���
			rootNode.ExitCurrentBattleAction();
			this->frameCount = 0;
			this->collider->data->isDoHitCheck = false;
			this->collider->data->isHitAttack = false;
			return ActionNode::NodeState::SUCCESS;
		}
		return ActionNode::NodeState::RUNNING;
	}
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}

/// <summary>
/// �`��
/// </summary>
const void Beast_Rush::Draw()const
{
#ifdef _DEBUG
	DrawCapsule3D(this->collider->rigidbody.GetPosition(), this->collider->topPositon, this->collider->radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
#endif // _DEBUG
}
