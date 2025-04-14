#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Mage_Walk.h"
#include "Enemy.h"
#include "MageEnemy.h"
#include "EnemyManager.h"
#include "MageEnemyBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Mage_Walk::Mage_Walk()
	: isInitialize		(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(MageEnemy::AnimationType::WALK_FRONT);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(MageEnemyBehaviorTree::ActionType::WALK);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["WALK_SPEED"];;
	this->accel				= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["DECEL"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Mage_Walk::~Mage_Walk()
{

}

/// <summary>
/// ������
/// </summary>
void Mage_Walk::Initialize()
{
	this->isInitialize = false;
}

/// <summary>
/// �X�V����
/// </summary>
Mage_Walk::NodeState Mage_Walk::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<MageEnemy&>(_chara);

	/*�����t���O������Ă�����ȉ��̏����͍s��Ȃ�*/
	if (!enemy.GetIsAlive())return ActionNode::NodeState::SUCCESS;

	/*�o�^����Ă���A�N�V�����Ǝ��ۂ̃A�N�V�������قȂ��Ă�����*/
	if (_tree.GetNowSelectAction() != this->actionType)
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		enemy.SetNowAnimation(this->animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		//�A�N�V�����̐ݒ�
		_tree.SetNowSelectAction(this->actionType);
	}

	/*�A�j���[�V�����̍Đ�*/
	enemy.PlayAnimation();

	/*�ړ�*/
	enemy.Move(this->maxSpeed, this->accel, this->decel, false);

	/*��Ԃ�Ԃ�*/
	return ActionNode::NodeState::SUCCESS;
}
