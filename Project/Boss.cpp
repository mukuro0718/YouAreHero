#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "ReactionType.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "ColliderData.h"
#include "CharacterColliderData.h"
#include "BitFlag.h"
#include "Animation.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "BossActionHeader.h"
#include "Boss.h"
#include "LoadingAsset.h"
#include "PlayerManager.h"
#include "CameraManager.h"
#include "EffectManager.h"
#include "Debug.h"
#include "HitStopManager.h"
#include "Shadow.h"
#include "MapManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Boss::Boss()
	: Enemy()
	, prevAttack(AttackType::NONE)
	, angryValue			(0.0f)
	, nowAction				(0)
	, angryState			(0)
	, tiredInterval			(0)
	, attackComboCount		(0)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json  = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�����o�N���X�̃C���X�^���X�̍쐬*/
	this->modelHandle	= MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::ENEMY));
	this->normalTexture = asset.GetImage(LoadingAsset::ImageType::BOSS_NORMAL_TEXTURE);
	this->angryTexture  = asset.GetImage(LoadingAsset::ImageType::BOSS_ANGRY_TEXTURE);
	this->tiredTexture  = asset.GetImage(LoadingAsset::ImageType::BOSS_TIRED_TEXTURE);
	/*�A�j���[�V�����̐ݒ�*/
	vector<int>	animationHandle	  = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_HANDLE"];
	vector<int>		animationIndex	  = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_INDEX"];
			  this->nowAnimation	  = static_cast<int>(AnimationType::IDLE);
			  this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	//�A�j���[�V�����̒ǉ�
	for (int i = 0; i < animationHandle.size(); i++)
	{
		this->animation->Add(animationHandle[i], animationIndex[i]);
	}
	//�A�j���[�V�����̃A�^�b�`
	this->animation->Attach(&this->modelHandle);

	/*�A�N�V�����}�b�v�̍쐬*/
	this->actionTypeMap.emplace(static_cast<int>(ActionType::DYING)			,this->DYING);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::IDLE)			,this->IDLE);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::ROAR)			,this->ROAR);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::WALK)			,this->WALK);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::REST)			,this->REST);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::SLASH_1)		,this->SLASH_1);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::SLASH_2)		,this->SLASH_2);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::STAB)			,this->STAB);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::ROTATE_SLASH)	,this->ROTATE_SLASH);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::PUNCH)			,this->PUNCH);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::SLASH_COMBO_1)	,this->SLASH_COMBO_1);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::SLASH_COMBO_2),this->SLASH_COMBO_2);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::JUMP_ATTACK)	,this->JUMP_ATTACK);

	/*�R���C�_�[�f�[�^�̍쐬*/
	this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BOSS, new CharacterData());

	/*�A�N�V�����p�����[�^�[�̍쐬*/
	this->parameters.emplace_back(new BossDeathAction());
	this->parameters.emplace_back(new BossIdleAction());
	this->parameters.emplace_back(new BossRoarAction());
	this->parameters.emplace_back(new BossChaseAction());
	this->parameters.emplace_back(new BossRestAction());
	this->parameters.emplace_back(new BossSlashAction());
	this->parameters.emplace_back(new BossSlash2Action());
	this->parameters.emplace_back(new BossStabAction());
	this->parameters.emplace_back(new BossRotateSlashAction());
	this->parameters.emplace_back(new BossPunchAction());
	this->parameters.emplace_back(new BossSlashComboAction());
	this->parameters.emplace_back(new BossSlashCombo2Action());
	this->parameters.emplace_back(new BossJumpAttackAction());

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Boss::~Boss()
{
	Finalize();
	this->actionTypeMap.clear();
	DeleteMemberInstance(this->collider);
	for (int i = 0; i < this->parameters.size(); i++)
	{
		DeleteMemberInstance(this->parameters[i]);
	}
	this->parameters.clear();
}

/// <summary>
/// ������
/// </summary>
void Boss::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�ϐ��̏�����*/
	this->isAlive						 = true;
	this->isGround						 = true;
	this->isDraw						 = true;
	this->speed							 = 0.0f;
	this->animationPlayTime				 = 0.0f;
	this->entryInterval					 = 0;
	this->moveTarget					 = Gori::ORIGIN;
	this->nowAnimation					 = static_cast<int>(AnimationType::ROAR);
	this->nowAction						 = static_cast<int>(ActionType::ROAR);
	this->attackComboCount				 = 0;
	this->angryState					 = static_cast<int>(BossState::NORMAL);
	float height						 = json.GetJson(JsonManager::FileType::ENEMY)["HIT_HEIGHT"];
	this->collider->topPositon			 = VGet(0.0f, height, 0.0f);
	this->collider->radius				 = json.GetJson(JsonManager::FileType::ENEMY)["HIT_RADIUS"];
	this->collider->isUseCollWithGround	 = true;
	this->collider->isUseCollWithChara	 = true;
	this->collider->data->defensivePower = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DEFENSIVE_POWER"];
	this->collider->data->hp			 = json.GetJson(JsonManager::FileType::ENEMY)["HP"];
	this->collider->data->isHit			 = false;
	this->tiredInterval = 0;

	/*�R���{�̐ݒ�*/
	SetAttackComboCount();

	/*�p�����[�^�̏�����*/
	for (int i = 0; i < this->parameters.size(); i++)
	{
		this->parameters[i]->Initialize();
	}

	/*���������̏�����*/
	//json�f�[�^��萔�ɑ��
	const VECTOR POSITION = Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_POSITION"]);//���W
	const VECTOR ROTATION = Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_ROTATION"]);//��]��
	const VECTOR SCALE = Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_SCALE"]);	 //�g�嗦
	//������
	this->collider->rigidbody.Initialize(true);
	this->collider->rigidbody.SetPosition(POSITION);
	this->collider->rigidbody.SetRotation(ROTATION);
	this->collider->rigidbody.SetScale(SCALE);
	MV1SetPosition	 (this->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->modelHandle, this->collider->rigidbody.GetScale());

	this->state->ClearFlag(this->MASK_ALL);
	this->state->SetFlag(this->ROAR);

	/*�A�j���[�V�����̃A�^�b�`*/
	this->animation->Attach(&this->modelHandle);
	MV1SetTextureGraphHandle(this->modelHandle, 0, this->normalTexture, FALSE);

}

/// <summary>
/// �㏈��
/// </summary>
void Boss::Finalize()
{
	this->collider->isUseCollWithGround = false;
}

/// <summary>
/// �X�V
/// </summary>
void Boss::Update()
{
	//int startTime = GetNowCount();
	
	/*�X�e�[�W�O�ɏo����f�X*/
	if (this->collider->rigidbody.GetPosition().y < -30.0f)
	{
		DyingIfOutOfStage();
	}

	/*�{���Ԃ̐ݒ�*/
	SetAngryState();
	/*��Ԃ̐؂�ւ�*/
	ChangeState();


	/*�����Ɋe�A�N�V�������Ƃ̍X�V��������ꂽ��*/
	this->parameters[this->nowAction]->Update(*this);
	//int endTime = GetNowCount();
	//this->frameTime = endTime - startTime;
}

/// <summary>
/// �A�j���[�V�����̍Đ�
/// </summary>
void Boss::PlayAnimation()
{
	//�A�j���[�V�����̍Đ�
	if (this->isAlive)
	{
		VECTOR position = this->collider->rigidbody.GetPosition();
		this->animation->Play(&this->modelHandle, this->nowAnimation, this->animationPlayTime);
		this->collider->rigidbody.SetPosition(position);
	}
}

/// <summary>
/// �U���R���{�񐔂̃Z�b�g
/// </summary>
void Boss::SetAttackComboCount()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�R���{���̐ݒ�*/
	this->attackComboCount = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_COMBO_COUNT"][this->angryState];
}

/// <summary>
/// ��Ԃ̕ύX
/// </summary>
void Boss::ChangeState()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& debug = Singleton<Debug>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	int debugActionType = json.GetJson(JsonManager::FileType::DEBUG)["BOSS_ACTION"];
	if (debug.IsShowDebugInfo(Debug::ItemType::ENEMY) && debugActionType != static_cast<int>(Boss::ActionType::NONE))
	{
		/*�������Ă���t���O������*/
		unsigned int clearFlag = this->actionTypeMap[this->nowAction];
		this->state->ClearFlag(clearFlag);
		this->nowAction = debugActionType;
		this->parameters[this->nowAction]->OnIsSelect();
	}
	else
	{
		/*�I������Ă��邩���ׂ�*/
		bool isSelect = false;//�I������Ă��邩
		int  sumDesireValue = 0;	//�~���l�̍��v
		for (int i = 0; i < this->parameters.size(); i++)
		{
			if (i == this->nowAction)
			{
				isSelect = this->parameters[i]->GetIsSelect();
				if (isSelect)return;
			}
			sumDesireValue += parameters[i]->GetDesireValue();
		}
		
		/*�����ł��ׂẴp�����[�^�̌v�Z���s��*/
		bool isPriority = false;//�I������Ă��邩
		for (int i = 0; i < this->parameters.size(); i++)
		{
			this->parameters[i]->CalcParameter(*this);
			//�D��t���O�������Ă��邩�����ׂ�
			if (this->parameters[i]->GetIsPriority())
			{
				isPriority = true;
			}
		}

		/*�������Ă���t���O������*/
		unsigned int clearFlag = this->actionTypeMap[this->nowAction];
		this->state->ClearFlag(clearFlag);

		/*�I������Ă��Ȃ�������*/
		if (!isSelect)
		{
			//�e�s���̊��Ғl�����߂�
			std::vector<int> actionWeight;//�d��
			int count = 0;
			for (int i = 0; i < this->parameters.size(); i++)
			{
				actionWeight.emplace_back(this->parameters[i]->GetWeight(sumDesireValue));
				//�������X�g�̒��őI���t���O����ł������Ă�����
				if (isPriority)
				{
					//���݂�item���I���t���O�������Ă��Ȃ�������d�݂�0�ɂ���
					if (!this->parameters[i]->GetIsPriority())
					{
						actionWeight[count] = 0;
					}
				}
				count++;
			}
			//�d�݂������_���ŏo��
			int randomWeight = GetRand(this->parameters[0]->GetBaseWeight() - 1);
			//for��vector���񂵁A�d�݂��O�ȉ��ɂȂ����Ƃ���̃A�N�V�������s��
			for (int i = 0; i < actionWeight.size(); i++)
			{
				randomWeight -= actionWeight[i];
				if (randomWeight < 0 || this->parameters[i]->GetIsPriority())
				{
					this->nowAction = i;
					this->parameters[i]->OnIsSelect();
					isSelect = true;
					break;
				}
			}
		}
		if (!isSelect)
		{
			this->nowAction = static_cast<int>(ActionType::IDLE);
		}
	}
	unsigned int setFlag = this->actionTypeMap[this->nowAction];
	this->state->SetFlag(setFlag);
}

/// <summary>
/// �`��
/// </summary>
const void Boss::DrawCharacterInfo()const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& shadow = Singleton<Shadow>::GetInstance();
	auto& map = Singleton<MapManager>::GetInstance();
#ifdef _DEBUG


	//auto& debug = Singleton<Debug>::GetInstance();
	//if (debug.IsShowDebugInfo(Debug::ItemType::ENEMY))
	//{
	//	VECTOR position = this->collider->rigidbody.GetPosition();
	//	VECTOR rotation = this->collider->rigidbody.GetRotation();
	//	printfDx("Boss_POSITION X:%f,Y:%f,Z:%f	\n", position.x, position.y, position.z);
	//	printfDx("Boss_ROTATION X:%f,Y:%f,Z:%f	\n", rotation.x, rotation.y, rotation.z);
	//	printfDx("%d:DYING						\n", this->state->CheckFlag(this->DYING));
	//	printfDx("%d:IDLE						\n", this->state->CheckFlag(this->IDLE));
	//	printfDx("%d:ROAR						\n", this->state->CheckFlag(this->ROAR));
	//	printfDx("%d:WALK						\n", this->state->CheckFlag(this->WALK));
	//	printfDx("%d:REST						\n", this->state->CheckFlag(this->REST));
	//	printfDx("%d:SLASH_1					\n", this->state->CheckFlag(this->SLASH_1));
	//	printfDx("%d:SLASH_2					\n", this->state->CheckFlag(this->SLASH_2));
	//	printfDx("%d:STAB						\n", this->state->CheckFlag(this->STAB));
	//	printfDx("%d:ROTATE_SLASH				\n", this->state->CheckFlag(this->ROTATE_SLASH));
	//	printfDx("%d:PUNCH						\n", this->state->CheckFlag(this->PUNCH));
	//	printfDx("%d:SLASH_COMBO_1				\n", this->state->CheckFlag(this->SLASH_COMBO_1));
	//	printfDx("%d:SLASH_COMBO_2				\n", this->state->CheckFlag(this->SLASH_COMBO_2));
	//	printfDx("%d:JUMP_ATTACK				\n", this->state->CheckFlag(this->JUMP_ATTACK));
	//	printfDx("%d:STATE						\n", this->angryState);
	//	/*�e�A�N�V�����̓����蔻��}�`�̕`��*/
	//	this->parameters[this->nowAction]->Draw();
	//}

#endif // _DEBUG
	if (this->isDraw)
	{
		/*�����̕`��*/
		shadow.Draw(map.GetStageModelHandle(), this->collider->rigidbody.GetPosition(), this->SHADOW_HEIGHT, this->SHADOW_SIZE);
	}
	//printfDx("ENEMY_FRAMETIME:%d\n", this->frameTime);
}

const bool Boss::GetIsAttack()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))return true;
	return false;
}


/// <summary>
/// �{���Ԃ̐ݒ�
/// </summary>
void Boss::SetAngryState()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	
	/*�{����*/
	switch (this->angryState)
	{
		//�{��
	case static_cast<int>(BossState::ANGRY):
		this->attackComboCount = 2;
		this->angryValue--;
		if (this->angryValue < 0)
		{
			this->angryState = static_cast<int>(BossState::TIRED);
			this->angryValue = 0;
			MV1SetTextureGraphHandle(this->modelHandle, 0, this->tiredTexture, FALSE);
			this->collider->data->defensivePower = json.GetJson(JsonManager::FileType::ENEMY)["TIRED_DEFENSIVE_POWER"];
		}
		break;
		//�ʏ�
	case static_cast<int>(BossState::NORMAL):
		//�{��l�𑝉�
		this->angryValue++;
		//�U�����������Ă�����{��l������ɑ���
		if (this->collider->data->isHit)
		{
			this->angryValue++;
		}
		//�{��l���ő�ȏゾ�������Ԃ�ANGRY�ɂ���
		if (this->angryValue >= json.GetJson(JsonManager::FileType::ENEMY)["MAX_ANGRY_VALUE"])
		{
			this->angryState = static_cast<int>(BossState::ANGRY);
			MV1SetTextureGraphHandle(this->modelHandle, 0, this->angryTexture, FALSE);
			this->collider->data->defensivePower = json.GetJson(JsonManager::FileType::ENEMY)["ANGRY_DEFENSIVE_POWER"];
		}
		break;
	//���
	case static_cast<int>(BossState::TIRED):
		//��ꎞ�Ԃ𑝉�
		this->tiredInterval++;
		//�ő�l�𒴂������Ԃ�ʏ�ɕύX
		if (this->tiredInterval >= json.GetJson(JsonManager::FileType::ENEMY)["MAX_TIRED_INTERVAL"])
		{
			this->angryState = static_cast<int>(BossState::NORMAL);
			MV1SetTextureGraphHandle(this->modelHandle, 0, this->normalTexture, FALSE);
			this->tiredInterval = 0;
			this->collider->data->defensivePower = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DEFENSIVE_POWER"];
		}
		break;
	}
}

