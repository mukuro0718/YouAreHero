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
#include "Mage_Attack.h"
#include "Player.h"
#include "Enemy.h"
#include "MageEnemy.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "MageEnemyBehaviorTree.h"
#include "ReactionType.h"
#include "HitStop.h"
#include "EffectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Mage_Attack::Mage_Attack()
	: attackStartCount(0)
	, attackEndCount(0)
	, frameCount(0)
	, collider(nullptr)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType						= static_cast<int>(MageEnemy::AnimationType::ATTACK);
	this->animationPlayTime					= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType						= static_cast<int>(MageEnemyBehaviorTree::ActionType::ATTACK);
	this->interval							= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ACTION_INTERVAL"][this->actionType];
	this->maxSpeed							= 0.0f;
	this->accel								= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ACCEL"];
	this->decel								= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["DECEL"];
	this->attackStartCount					= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ATTACK_START_COUNT"];
	this->attackEndCount					= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ATTACK_END_COUNT"];
	this->frameIndexUsedCapsuleDirection1	= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ATTACK_COLLIDER_INDEX_1"];
	this->frameIndexUsedCapsuleDirection2	= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ATTACK_COLLIDER_INDEX_2"];

	/*�R���C�_�[�̍쐬*/
	this->collider								  = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BRAWLER, new AttackData());
	this->collider->radius						  = json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ATTACK_RADIUS"];
	this->collider->data->damage				  = json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ATTACK_DAMAGE"];
	this->collider->data->reactionType			  = static_cast<int>(Gori::PlayerReactionType::NORMAL);
	this->collider->data->hitStopTime			  = json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ATTACK_HIT_STOP_TIME"];
	this->collider->data->hitStopType			  = static_cast<int>(HitStop::Type::STOP);
	this->collider->data->hitStopDelay			  = json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ATTACK_HIT_STOP_DELAY"];
	this->collider->data->slowFactor			  = json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ATTACK_SLOW_FACTOR"];
	this->collider->data->isHitAttack			  = false;
	this->collider->data->isDoHitCheck			  = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Mage_Attack::~Mage_Attack()
{

}

/// <summary>
/// ������
/// </summary>
void Mage_Attack::Initialize()
{
	this->frameCount = 0;
	this->isFixRotate = false;
}

/// <summary>
/// �X�V����
/// </summary>
Mage_Attack::NodeState Mage_Attack::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<MageEnemy&>(_chara);
	auto& effect = Singleton<EffectManager>::GetInstance();

	/*�����t���O������Ă�����ȉ��̏����͍s��Ȃ�*/
	if (!enemy.GetIsAlive())return ActionNode::NodeState::SUCCESS;

	/*�o�^����Ă���A�N�V�����Ǝ��ۂ̃A�N�V�������قȂ��Ă�����*/
	if (_tree.GetNowSelectAction() != this->actionType)
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		enemy.SetNowAnimation(this->animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(this->startAnimationPlayTime);
		//�A�N�V�����̐ݒ�
		_tree.SetNowSelectAction(this->actionType);
		//������Root�ɓo�^
		_tree.EntryCurrentBattleAction(*this);
	}
	else
	{
		enemy.SetAnimationPlayTime(this->animationPlayTime);
	}

	/*�R���C�_�[�̍X�V*/
	//�w��t���[���𒴂��Ă��Ȃ���΃t���[���̑���
	if (this->frameCount < this->attackEndCount)
	{
		this->frameCount++;
		//�w��t���[���𒴂��Ă�����
		if (this->frameCount == this->attackStartCount)
		{
			//auto& sound = Singleton<SoundManager>::GetInstance();
			//sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_SWING_2);
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
		//auto& sound = Singleton<SoundManager>::GetInstance();
		//sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_HEAVY_ATTACK);
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
