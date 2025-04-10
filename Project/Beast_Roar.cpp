#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Beast_Roar.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"
#include "SoundManager.h"
#include "EffectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_Roar::Beast_Roar()
	: frameCount		 (0)
	, roarFrame			 (0)
	, isFinishedFirstRoar(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Beast::AnimationType::ROAR);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(BeastBehaviorTree::ActionType::ROAR);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->roarFrame			= json.GetJson(JsonManager::FileType::BEAST)["NORMAL_ROAR_COUNT"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_Roar::~Beast_Roar()
{

}

/// <summary>
/// �X�V����
/// </summary>
Beast_Roar::NodeState Beast_Roar::Update(BehaviorTree& _tree, Character& _chara)
{
	/*�A�j���[�V����*/
	auto& enemy = dynamic_cast<Beast&>(_chara);
	//����������Ă��Ȃ����
	if (this->frameCount == 0)
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		enemy.SetNowAnimation(this->animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		//�A�N�V�����̏�Ԃ��Z�b�g
		_tree.SetNowSelectAction(this->actionType);
		//�A�N�V�����̓o�^
		_tree.EntryCurrentBattleAction(*this);
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*���K��炷*/
	this->frameCount++;
	if (this->frameCount == this->roarFrame)
	{
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::LUXURIO_ROAR);
		this->isFinishedFirstRoar = true;
	}
	//�A�j���[�V�����̍Đ�
	enemy.PlayAnimation();

	/*�ړ�*/
	if (enemy.GetSpeed() != 0.0f)
	{
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*��Ԃ�Ԃ�*/
	{
		//�A�j���[�V�������I�����Ă�����
		if (enemy.GetIsChangeAnimation())
		{
			_tree.SetInterval(this->actionType);
			//�A�N�V�����̉���
			_tree.ExitCurrentBattleAction();
			this->frameCount = 0;
			return ActionNode::NodeState::SUCCESS;
		}
		//����ȊO�͎��s����Ԃ�
		else
		{
			return ActionNode::NodeState::RUNNING;
		}
	}
}
