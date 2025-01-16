#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_Breath.h"
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

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_Breath::Beast_Breath()
	: stage(AnimationStage::START)
	, animationSet()
	, nextStageSet()
{
	this->animationSet.emplace(AnimationStage::START, static_cast<int>(Beast::AnimationType::BREATH_START));
	this->animationSet.emplace(AnimationStage::LOOP, static_cast<int>(Beast::AnimationType::BREATH_LOOP));
	this->animationSet.emplace(AnimationStage::END, static_cast<int>(Beast::AnimationType::BREATH_END));
	this->nextStageSet.emplace(AnimationStage::START, AnimationStage::LOOP);
	this->nextStageSet.emplace(AnimationStage::LOOP, AnimationStage::END);
	this->nextStageSet.emplace(AnimationStage::END, AnimationStage::START);
	auto& json = Singleton<JsonManager>::GetInstance();
	this->actionType		= static_cast<int>(BeastBehaviorTree::ActionType::BREATH);
	this->interval			= json.GetJson(JsonManager::FileType::BEAST)["ACTION_INTERVAL"][this->actionType];
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->attackStartCount  = json.GetJson(JsonManager::FileType::BEAST)["BREATH_ATTACK_START_COUNT"];
	this->attackEndCount	= json.GetJson(JsonManager::FileType::BEAST)["BREATH_ATTACK_END_COUNT"];
	this->breathLength		= json.GetJson(JsonManager::FileType::BEAST)["BREATH_LENGTH"];
	this->frameIndexUsedCapsuleDirection1 = json.GetJson(JsonManager::FileType::BEAST)["BREATH_FRAME_INDEX_USED_CAPSULE_DIRECTION"][0];
	this->frameIndexUsedCapsuleDirection2 = json.GetJson(JsonManager::FileType::BEAST)["BREATH_FRAME_INDEX_USED_CAPSULE_DIRECTION"][1];

	/*�R���C�_�[�̍쐬*/
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BOSS_ATTACK, new AttackData());
	this->collider->radius				= json.GetJson(JsonManager::FileType::BEAST)["BREATH_RADIUS"];
	this->collider->data->damage		= json.GetJson(JsonManager::FileType::BEAST)["BREATH_DAMAGE"];
	this->collider->data->reactionType	= static_cast<int>(Gori::PlayerReactionType::NORMAL);
	this->collider->data->hitStopTime	= json.GetJson(JsonManager::FileType::BEAST)["BREATH_HIT_STOP_TIME"];
	this->collider->data->hitStopType	= static_cast<int>(HitStop::Type::STOP);
	this->collider->data->hitStopDelay  = json.GetJson(JsonManager::FileType::BEAST)["BREATH_HIT_STOP_DELAY"];
	this->collider->data->slowFactor	= json.GetJson(JsonManager::FileType::BEAST)["BREATH_SLOW_FACTOR"];
	this->collider->data->isHitAttack	= false;
	this->collider->data->isDoHitCheck	= false;

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_Breath::~Beast_Breath()
{

}

/// <summary>
/// �X�V����
/// </summary>
Beast_Breath::NodeState Beast_Breath::Update()
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
	//�w��t���[���𒴂��Ă��Ȃ���΃t���[���̑���
	if (this->frameCount < this->attackEndCount)
	{
		this->frameCount++;
		//�w��t���[���𒴂��Ă�����
		if (this->frameCount == this->attackStartCount)
		{
			this->collider->data->isDoHitCheck = true;
			auto& effect = Singleton<EffectManager>::GetInstance();
			effect.OnIsEffect(EffectManager::EffectType::BEAST_BREATH);
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
		VECTOR direction = VSub(position2, position1);
		direction.y = 0.0f;
		VECTOR directionScaling = VScale(direction, this->breathLength);
		this->collider->topPositon = VAdd(position1, directionScaling);
	}
	//�������Ă�����q�b�g�X�g�b�v��ݒ肷��
	if (this->collider->data->isHitAttack)
	{
		//�U���q�b�g�t���O������
		this->collider->data->isHitAttack = false;
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
	if (enemy.GetSpeed() != 0.0f || this->frameCount == 1)
	{
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*��Ԃ�Ԃ�*/
	//�A�j���[�V�������I�����Ă�����
	if (enemy.GetIsChangeAnimation())
	{
		//�X�e�[�W���X�V
		this->stage = this->nextStageSet[this->stage];
		//�����ŃX�e�[�W��START�Ȃ�A�j���[�V���������ׂďI�������Ƃ�������
		if (this->stage == AnimationStage::START)
		{
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
const void Beast_Breath::Draw()const
{
#ifdef _DEBUG
	DrawCapsule3D(this->collider->rigidbody.GetPosition(), this->collider->topPositon, this->collider->radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
#endif // _DEBUG

}