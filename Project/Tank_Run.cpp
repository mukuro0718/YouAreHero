#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Rigidbody.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Tank_Run.h"
#include "Boid.h"
#include "Enemy.h"
#include "Player.h"
#include "TankEnemy.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "TankEnemyBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Tank_Run::Tank_Run()
	: isInitialize		(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(TankEnemy::AnimationType::WALK_FRONT);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::TANK_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(TankEnemyBehaviorTree::ActionType::WALK);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::TANK_ENEMY)["WALK_SPEED"];
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

	//auto& sound = Singleton<SoundManager>::GetInstance();
	//this->frameCount++;
	//if (this->frameCount == this->walkSoundCount)
	//{
	//	sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_FOOTSTEPS);
	//	this->frameCount = 0;
	//}

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
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	VECTOR					 nowVelocity = enemy.GetRigidbody().GetVelocity();													//���݂̈ړ��x�N�g��
	const vector<Character*> weakEnemyList = enemyManager.GetWeakEnemyList();														//�G���G�̃��X�g
	VECTOR					 toTarget = VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition());	//�ړ��ڕW�ւ̃x�N�g��
	//��]���̍X�V
	enemy.UpdateRotation(toTarget);
	//�ړ����x�̍X�V
	enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);

	if (VSize(toTarget) >= 8.0f)
	{
		//�ړ��x�N�g�����o��
		VECTOR v1 = enemy.GetBoid().RuleSeparation(enemy, weakEnemyList);
		VECTOR v2 = enemy.GetBoid().RuleBounding(enemy);
		VECTOR v3 = enemy.GetBoid().RuleTargetAttraction(enemy, player.GetRigidbody().GetPosition());
		VECTOR newVelocity = VAdd(nowVelocity, VAdd(v1, VAdd(v2, v3)));
		if (VSize(newVelocity) > this->maxSpeed)
		{
			newVelocity = VScale(VNorm(newVelocity), this->maxSpeed);
		}
		//�V�����ړ��x�N�g�����o��
		newVelocity = VGet(newVelocity.x, nowVelocity.y, newVelocity.z);
		enemy.SetVelocity(newVelocity);
	}
	else
	{
		enemy.SetVelocity(VGet(0.0f, nowVelocity.y, 0.0f));
	}

	/*��Ԃ�Ԃ�*/
	return ActionNode::NodeState::SUCCESS;
}
