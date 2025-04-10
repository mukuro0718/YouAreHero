#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Tank_Run.h"
#include "Enemy.h"
#include "TankEnemy.h"
#include "EnemyManager.h"
#include "TankEnemyBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Tank_Run::Tank_Run()
	: isInitialize		(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(TankEnemy::AnimationType::DYING);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::TANK_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(TankEnemyBehaviorTree::ActionType::DYING);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::TANK_ENEMY)["RUN_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::TANK_ENEMY)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::TANK_ENEMY)["DECEL"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Tank_Run::~Tank_Run()
{

}

/// <summary>
/// ������
/// </summary>
void Tank_Run::Initialize()
{
	this->isInitialize = false;
}

/// <summary>
/// �X�V����
/// </summary>
Tank_Run::NodeState Tank_Run::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<TankEnemy&>(_chara);

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
