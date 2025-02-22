#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackCapsuleColliderData.h"
#include "AttackData.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "Dragon_Rotate.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"
#include "ReactionType.h"
#include "HitStop.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Dragon_Rotate::Dragon_Rotate()
	: USE_COLLIDER_INDEX(static_cast<short>(Dragon::AttackCollider::ROTATE))
	, frameCount		(0)
	, currentDragonState(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType				= static_cast<int>(Dragon::AnimationType::ROTATE_ATTACK);
	vector<float>animationPlayTime	= json.GetJson(JsonManager::FileType::DRAGON)["ROTATE_PLAY_TIME"];
	this->animationPlayTime			= animationPlayTime;
	this->actionType				= static_cast<short>(DragonBehaviorTree::ActionType::ROTATE);
	this->maxSpeed					= 0.0f;
	this->accel						= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel						= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
	this->isFixRotate				= true;
	this->interval					= json.GetJson(JsonManager::FileType::DRAGON)["ROTATE_INTERVAL"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Dragon_Rotate::~Dragon_Rotate()
{

}

/// <summary>
/// �X�V����
/// </summary>
Dragon_Rotate::NodeState Dragon_Rotate::Update()
{
	/*�A�N�V�����̏�Ԃ��Z�b�g*/
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Dragon&>(enemyManager.GetCharacter());
	if (this->frameCount == 0)
	{
		//�A�N�V�����̐ݒ�
		rootNode.SetCurrentAction(this->actionType);
		//�A�N�V�����̓o�^
		rootNode.EntryCurrentBattleAction(*this);
		//�i�K���擾
		this->currentDragonState = enemy.GetBossState();
		this->frameCount++;
	}

	/*�ړ�*/
	enemy.UpdateSpeed(0.0f, this->accel, this->decel);
	enemy.UpdateVelocity(false);

	/*�����蔻��R���C�_�[�̍X�V*/
	enemy.UpdateAttackCollider(this->USE_COLLIDER_INDEX, this->nowTotalPlayTime);

	/*�A�j���[�V�����̍Đ�*/
	float playTime = this->animationPlayTime[this->currentDragonState];
	this->nowTotalPlayTime += playTime;
	enemy.PlayAnimation(this->animationType, playTime);

	/*��Ԃ�Ԃ�*/
	//�A�j���[�V�������I�����Ă�����
	if (enemy.GetIsChangeAnimation())
	{
		this->nowTotalPlayTime = 0.0f;
		this->frameCount = 0;
		enemy.OffAttackCollider(this->USE_COLLIDER_INDEX);
		rootNode.ExitCurrentBattleAction();
		rootNode.SetInterval(this->actionType, this->interval);
		enemy.DecAttackComboCount();
		return ActionNode::NodeState::SUCCESS;
	}
	//����ȊO�͎��s����Ԃ�
	return ActionNode::NodeState::RUNNING;
}
