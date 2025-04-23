#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Mage_Walk.h"
#include "Boid.h"
#include "Enemy.h"
#include "Player.h"
#include "MageEnemy.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "MageEnemyBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Mage_Walk::Mage_Walk()
	: frameCount(0)
	, walkSoundCount(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(MageEnemy::AnimationType::WALK_FRONT);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(MageEnemyBehaviorTree::ActionType::WALK);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["WALK_SPEED"];;
	this->accel				= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["DECEL"];
	this->walkSoundCount	= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["WALK_SOUND_COUNT"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Mage_Walk::~Mage_Walk()
{

}

/// <summary>
/// �X�V����
/// </summary>
Mage_Walk::NodeState Mage_Walk::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
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
		this->frameCount = 0;
	}

	/*�ړ�*/
	VECTOR					 nowVelocity = enemy.GetRigidbody().GetVelocity();													//���݂̈ړ��x�N�g��
	const vector<Character*> weakEnemyList = enemyManager.GetWeakEnemyList();														//�G���G�̃��X�g
	VECTOR					 toTarget = VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition());	//�ړ��ڕW�ւ̃x�N�g��
	//��]���̍X�V
	enemy.UpdateRotation(toTarget);
	//�ړ����x�̍X�V
	enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
	//�ړ��x�N�g�����o��
	VECTOR v1 = enemy.GetBoid().RuleSurroundPlayer(enemy, player.GetRigidbody().GetPosition());
	VECTOR v2 = enemy.GetBoid().RuleSeparation(enemy, weakEnemyList);
	VECTOR v3 = enemy.GetBoid().RuleAlignment(enemy, weakEnemyList);
	VECTOR v4 = enemy.GetBoid().RuleBounding(enemy);
	VECTOR newVelocity = VAdd(nowVelocity, VAdd(v1, VAdd(v2, VAdd(v3, v4))));
	if (VSize(newVelocity) > this->maxSpeed)
	{
		newVelocity = VScale(VNorm(newVelocity), this->maxSpeed);
	}
	//�V�����ړ��x�N�g�����o��
	newVelocity = VGet(newVelocity.x, nowVelocity.y, newVelocity.z);
	enemy.SetVelocity(newVelocity);

	/*�A�j���[�V�����̎�ނ����߂�*/
	VECTOR forward = VNorm(toTarget);
	VECTOR velocity = newVelocity;
	float dot = VDot(forward, velocity);
	VECTOR cross = VCross(forward, velocity);
	//����
	MageEnemy::AnimationType animationType = MageEnemy::AnimationType::IDLE;
	if (VSize(newVelocity) > 0.01f)
	{
		if (dot > this->THRESHOLD)
		{
			animationType = MageEnemy::AnimationType::WALK_FRONT;
		}
		else if (dot < -this->THRESHOLD)
		{
			animationType = MageEnemy::AnimationType::WALK_BACK;
		}
		else if (cross.y > this->THRESHOLD)
		{
			animationType = MageEnemy::AnimationType::WALK_LEFT;
		}
		else if (cross.y < -this->THRESHOLD)
		{
			animationType = MageEnemy::AnimationType::WALK_RIGHT;
		}
	}
	enemy.SetNowAnimation(static_cast<int>(animationType));
	
	/*�A�j���[�V�����̍Đ�*/
	enemy.PlayAnimation();

	/*��Ԃ�Ԃ�*/
	return ActionNode::NodeState::SUCCESS;
}
