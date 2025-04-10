#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Dragon_Dying.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Dragon_Dying::Dragon_Dying()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Dragon::AnimationType::DYING);
	this->animationPlayTime	= json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(DragonBehaviorTree::ActionType::DYING);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Dragon_Dying::~Dragon_Dying()
{

}

/// <summary>
/// �X�V����
/// </summary>
Dragon_Dying::NodeState Dragon_Dying::Update(BehaviorTree& _tree, Character& _chara)
{
	/*���x���O�ȏ�܂��͍ŏ��ɂ��̃A�N�V�����ɂȂ������ړ��������s��*/
	auto& enemy = dynamic_cast<Dragon&>(_chara);
	int prevAction = _tree.GetNowSelectAction();
	if (enemy.GetSpeed() != 0.0f || prevAction != this->actionType)
	{
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*�A�N�V�����̏�Ԃ��Z�b�g*/
	if (prevAction != this->actionType)
	{
		_tree.SetNowSelectAction(this->actionType);
	}

	/*�A�j���[�V�����̍Đ�*/
	enemy.PlayAnimation(this->animationType, this->animationPlayTime);

	/*��Ԃ�Ԃ�*/
	//�A�j���[�V�������I�����Ă�����
	if (enemy.GetIsChangeAnimation())
	{
		//�C���^�[�o���̐ݒ�
		_tree.SetInterval(this->actionType);
		enemy.OffIsAlive();
		return ActionNode::NodeState::SUCCESS;
	}
	//����ȊO�͎��s����Ԃ�
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
