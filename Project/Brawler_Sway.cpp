#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Brawler_Sway.h"
#include "Enemy.h"
#include "BrawlerEnemy.h"
#include "EnemyManager.h"
#include "BrawlerEnemyBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Brawler_Sway::Brawler_Sway()
	: isInitialize		(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(BrawlerEnemy::AnimationType::SWAY);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(BrawlerEnemyBehaviorTree::ActionType::SWAY);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["DECEL"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Brawler_Sway::~Brawler_Sway()
{

}

/// <summary>
/// ������
/// </summary>
void Brawler_Sway::Initialize()
{
	this->isInitialize = false;
}

/// <summary>
/// �X�V����
/// </summary>
Brawler_Sway::NodeState Brawler_Sway::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<BrawlerEnemy&>(_chara);

	/*�����t���O������Ă�����ȉ��̏����͍s��Ȃ�*/
	if (!enemy.GetIsAlive())return ActionNode::NodeState::SUCCESS;

	/*�o�^����Ă���A�N�V�����Ǝ��ۂ̃A�N�V�������قȂ��Ă�����*/
	if (!this->isInitialize)
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		enemy.SetNowAnimation(this->animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(this->animationPlayTime);
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
	//�A�j���[�V�������I�����Ă�����
	if (enemy.GetIsChangeAnimation())
	{
		return ActionNode::NodeState::SUCCESS;
	}
	//����ȊO�͎��s����Ԃ�
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
