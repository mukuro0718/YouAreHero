#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_RaiseLevel.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"
#include "EffectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_RaiseLevel::Beast_RaiseLevel()
	: effectStartCount	(false)
	, frameCount		(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Beast::AnimationType::RAISE_LEVEL);
	this->actionType		= static_cast<int>(BeastBehaviorTree::ActionType::RAISE_LEVEL);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->interval			= json.GetJson(JsonManager::FileType::BEAST)["ACTION_INTERVAL"][this->actionType];
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->effectStartCount	= json.GetJson(JsonManager::FileType::BEAST)["RAISE_LEVEL_EFFECT_START_COUNT"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_RaiseLevel::~Beast_RaiseLevel()
{

}

/// <summary>
/// �X�V����
/// </summary>
Beast_RaiseLevel::NodeState Beast_RaiseLevel::Update()
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
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*�G�t�F�N�g�̐���*/
	if (this->frameCount < this->effectStartCount)
	{
		this->frameCount++;
		if (this->frameCount == this->effectStartCount)
		{
			auto& effect = Singleton<EffectManager>::GetInstance();
			effect.OnIsEffect(EffectManager::EffectType::BEAST_RAISE_LEVEL);
		}
	}

	/*�A�j���[�V�����̍Đ�*/
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
			//�C���^�[�o���̐ݒ�
			rootNode.SetInterval(this->actionType, this->interval);
			//�o�^������
			rootNode.ExitCurrentBattleAction();
			//���x���̏㏸
			rootNode.RaiseLevel();
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