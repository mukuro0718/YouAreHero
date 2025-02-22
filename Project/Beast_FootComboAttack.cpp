#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_FootComboAttack.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackCapsuleColliderData.h"
#include "AttackData.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"
#include "ReactionType.h"
#include "HitStop.h"
#include "PlayerManager.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_FootComboAttack::Beast_FootComboAttack()
	: frameCount	(0)
	, maxAttackCount(0)
	, attackCount	(0)
	, collider		(nullptr)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationPlayTime						  = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->animationType							  = static_cast<int>(Beast::AnimationType::COMBO_ATTACK);
	this->actionType							  = static_cast<int>(BeastBehaviorTree::ActionType::COMBO_ATTACK);
	this->interval								  = json.GetJson(JsonManager::FileType::BEAST)["ACTION_INTERVAL"][this->actionType];
	this->maxSpeed								  = json.GetJson(JsonManager::FileType::BEAST)["ATTACKING_SPEED"];
	this->accel									  = json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel									  = json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	vector<short> attackStartCount				  = json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_ATTACK_START_COUNT"];
	vector<short> attackEndCount				  = json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_ATTACK_END_COUNT"];
	this->attackStartCount						  = attackStartCount;
	this->attackEndCount						  = attackEndCount;
	vector<short> moveStartCount				  = json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_MOVE_START_COUNT"];
	vector<short> moveEndCount					  = json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_MOVE_END_COUNT"];
	this->moveStartCount						  = moveStartCount;
	this->moveEndCount							  = moveEndCount;
	this->maxAttackCount						  = json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_MAX_ATTACK_COUNT"];
	vector<short> frameIndexUsedCapsuleDirection1 = json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_FRAME_INDEX_USED_CAPSULE_DIRECTION_1"];
	vector<short> frameIndexUsedCapsuleDirection2 = json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_FRAME_INDEX_USED_CAPSULE_DIRECTION_2"];
	this->frameIndexUsedCapsuleDirection1		  = frameIndexUsedCapsuleDirection1;
	this->frameIndexUsedCapsuleDirection2		  = frameIndexUsedCapsuleDirection2;

	/*�R���C�_�[�̍쐬*/
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BOSS_ATTACK, new AttackData());
	this->collider->radius							= json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_RADIUS"];
	this->collider->data->damage					= json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_DAMAGE"];
	this->collider->data->reactionType				= static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
	this->collider->data->hitStopTime				= json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_HIT_STOP_TIME"];
	this->collider->data->hitStopType				= static_cast<int>(HitStop::Type::STOP);
	this->collider->data->hitStopDelay				= json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_HIT_STOP_DELAY"];
	this->collider->data->slowFactor				= json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_SLOW_FACTOR"];
	this->collider->data->isHitAttack				= false;
	this->collider->data->isDoHitCheck				= false;
	this->collider->data->blockStaminaConsumption	= json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_BLOCK_STAMINA_CONSUMPTION"];

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_FootComboAttack::~Beast_FootComboAttack()
{

}
/// <summary>
/// ������
/// </summary>
void Beast_FootComboAttack::Initialize()
{
	this->frameCount  = 0;
	this->attackCount = 0;
	this->isFixRotate = false;
}

/// <summary>
/// �X�V����
/// </summary>
Beast_FootComboAttack::NodeState Beast_FootComboAttack::Update()
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
		enemy.DecAttackComboCount();
	}

	/*�R���C�_�[�̍X�V*/
	bool isMove = false;
	if (this->attackCount < this->maxAttackCount)
	{
		//�t���[���̑���
		this->frameCount++;
		//�w��t���[���𒴂��Ă�����
		if (this->frameCount >= this->moveStartCount[this->attackCount])
		{
			isMove = true;
		}
		if (this->frameCount >= this->moveEndCount[this->attackCount])
		{
			isMove = false;
		}
		if (this->frameCount == this->attackStartCount[this->attackCount])
		{
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_SWING_1);
			this->collider->data->isDoHitCheck = true;
		}
		if (this->frameCount >= this->attackEndCount[this->attackCount])
		{
			this->collider->data->isDoHitCheck = false;
			this->collider->data->isHitAttack = false;
			this->attackCount++;
		}
		//�����蔻�肪�����Ԃ�������
		if (this->collider->data->isDoHitCheck)
		{
			VECTOR position1 = MV1GetFramePosition(enemy.GetModelHandle(), this->frameIndexUsedCapsuleDirection1[this->attackCount]);
			VECTOR position2 = MV1GetFramePosition(enemy.GetModelHandle(), this->frameIndexUsedCapsuleDirection2[this->attackCount]);
			this->collider->rigidbody.SetPosition(position1);
			this->collider->topPositon = position2;
		}
		//�������Ă�����q�b�g�X�g�b�v��ݒ肷��
		if (this->collider->data->isHitAttack)
		{
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_LIGHT_ATTACK);
			//�U���q�b�g�t���O������
			this->collider->data->isHitAttack = false;
		}
	}

	/*�A�j���[�V�����̍Đ�*/
	enemy.PlayAnimation();

	/*�ړ�*/
	float maxSpeed = 0.0f;
	if (isMove)
	{
		maxSpeed = this->maxSpeed;
		//�ڕW�܂ł̃x�N�g�����o��
		auto& player = Singleton<PlayerManager>::GetInstance();
		VECTOR toTarget = VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition());
		//���K��
		enemy.UpdateRotation(toTarget);
	}
	//�ړ����x�̍X�V
	enemy.UpdateSpeed(maxSpeed, this->accel, this->decel);
	//�ړ��x�N�g�����o��
	enemy.UpdateVelocity(false);

	/*��Ԃ�Ԃ�*/
	//�A�j���[�V�������I�����Ă�����
	if (enemy.GetIsChangeAnimation())
	{
		//�C���^�[�o���̐ݒ�
		rootNode.SetInterval(this->actionType, this->interval);
		//�o�^������
		rootNode.ExitCurrentBattleAction();
		this->frameCount = 0;
		this->collider->data->isDoHitCheck = false;
		this->collider->data->isHitAttack = false;
		this->attackCount = 0;
		return ActionNode::NodeState::SUCCESS;
	}
	return ActionNode::NodeState::RUNNING;
}

/// <summary>
/// �`��
/// </summary>
const void Beast_FootComboAttack::Draw()const
{
#ifdef _DEBUG
	DrawCapsule3D(this->collider->rigidbody.GetPosition(), this->collider->topPositon, this->collider->radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
#endif // _DEBUG
}
