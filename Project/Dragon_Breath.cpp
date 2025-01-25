#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Dragon_Breath.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Dragon_Breath::Dragon_Breath()
	: USE_COLLIDER_INDEX(static_cast<short>(Dragon::AttackCollider::BREATH))
	, FIX_ROTATE_FRAME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::DRAGON)["FIX_ROTATE_FRAME"])
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType				= static_cast<int>(Dragon::AnimationType::BREATH);
	vector<float>animationPlayTime	= json.GetJson(JsonManager::FileType::DRAGON)["BREATH_PLAY_TIME"];
	this->animationPlayTime			= animationPlayTime;
	this->actionType				= static_cast<short>(DragonBehaviorTree::ActionType::BREATH);
	this->maxSpeed					= 0.0f;
	this->accel						= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel						= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
	this->isFixRotate				= true;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Dragon_Breath::~Dragon_Breath()
{

}

/// <summary>
/// �X�V����
/// </summary>
Dragon_Breath::NodeState Dragon_Breath::Update()
{
	/*�A�N�V�����̏�Ԃ��Z�b�g*/
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();
	if (this->frameCount == 0)
	{
		//�A�N�V�����̐ݒ�
		rootNode.SetCurrentAction(this->actionType);
		//�A�N�V�����̓o�^
		rootNode.EntryCurrentBattleAction(*this);
		//�i�K���擾
		this->currentDragonStage = rootNode.GetDragonStage();
	}

	/*�t���[���J�E���g�̑���*/
	if (this->isFixRotate)
	{
		this->frameCount++;
		if (this->frameCount >= this->FIX_ROTATE_FRAME)
		{
			this->isFixRotate = false;
		}
	}

	/*�ړ�*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Dragon&>(enemyManager.GetCharacter());
	if (this->isFixRotate)
	{
		enemy.Move(this->maxSpeed, this->accel, this->decel, false);
		
		return ActionNode::NodeState::SUCCESS;
	}
	else
	{
		enemy.UpdateSpeed(0.0f, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*�����蔻��R���C�_�[�̍X�V*/
	enemy.UpdateAttackCollider(this->USE_COLLIDER_INDEX, this->nowTotalPlayTime);

	/*�A�j���[�V�����̍Đ�*/
	float playTime = this->animationPlayTime[this->currentDragonStage];
	this->nowTotalPlayTime += playTime;
	enemy.PlayAnimation(this->animationType, playTime);

	/*��Ԃ�Ԃ�*/
	//�A�j���[�V�������I�����Ă�����
	if (enemy.GetIsChangeAnimation())
	{
		this->isFixRotate = true;
		this->frameCount = 0;
		this->nowTotalPlayTime = 0.0f;
		enemy.OffAttackCollider(this->USE_COLLIDER_INDEX);
		rootNode.ExitCurrentBattleAction();
		rootNode.CalcAttackCount();
		return ActionNode::NodeState::SUCCESS;
	}
	//����ȊO�͎��s����Ԃ�
	return ActionNode::NodeState::RUNNING;
}