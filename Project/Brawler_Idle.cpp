#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Brawler_Idle.h"
#include "Boid.h"
#include "Enemy.h"
#include "BrawlerEnemy.h"
#include "EnemyManager.h"
#include "BrawlerEnemyBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Brawler_Idle::Brawler_Idle()
	: isInitialize		(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(BrawlerEnemy::AnimationType::WALK_FRONT);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(BrawlerEnemyBehaviorTree::ActionType::IDLE);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["WALK_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["DECEL"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Brawler_Idle::~Brawler_Idle()
{

}

/// <summary>
/// ������
/// </summary>
void Brawler_Idle::Initialize()
{
	this->isInitialize = false;
}

/// <summary>
/// �X�V����
/// </summary>
Brawler_Idle::NodeState Brawler_Idle::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<BrawlerEnemy&>(_chara);

	/*�����t���O������Ă�����ȉ��̏����͍s��Ȃ�*/
	if (!enemy.GetIsAlive())return ActionNode::NodeState::SUCCESS;

	/*�o�^����Ă���A�N�V�����Ǝ��ۂ̃A�N�V�������قȂ��Ă�����*/
	if (_tree.GetNowSelectAction() != this->actionType)
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		enemy.SetNowAnimation(this->animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		this->isInitialize = true;
	}

	/*�A�j���[�V�����̍Đ�*/
	enemy.PlayAnimation();

	/*�ړ�*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	VECTOR					 nowVelocity	= enemy.GetRigidbody().GetVelocity();	//���݂̈ړ��x�N�g��
	const vector<Character*> weakEnemyList	= enemyManager.GetWeakEnemyList();		//�G���G�̃��X�g
	VECTOR					 toTarget		= enemy.GetRigidbody().GetVelocity();	//�ړ��ڕW�ւ̃x�N�g��
	//��]���̍X�V
	enemy.UpdateRotation(VScale(toTarget,-1.0f));
	//�ړ����x�̍X�V
	enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
	//�ړ��x�N�g�����o��
	VECTOR v1 = enemy.GetBoid().RuleCohesion(enemy);
	VECTOR v2 = VScale(enemy.GetBoid().RuleSeparation(enemy, weakEnemyList), 1.5f);
	VECTOR v3 = VGet(0, 0, 0);
	VECTOR v4 = enemy.GetBoid().RuleBounding(enemy);
	VECTOR newVelocity = VAdd(nowVelocity, VAdd(v1, VAdd(v2, VAdd(v3, v4))));
	if (VSize(newVelocity) > this->maxSpeed)
	{
		newVelocity = VScale(VNorm(newVelocity), this->maxSpeed);
	}
	//�V�����ړ��x�N�g�����o��
	newVelocity = VGet(newVelocity.x, nowVelocity.y, newVelocity.z);
	enemy.SetVelocity(newVelocity);

	/*��Ԃ�Ԃ�*/
	return ActionNode::NodeState::SUCCESS;
}
