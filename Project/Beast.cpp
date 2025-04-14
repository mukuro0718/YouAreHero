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
#include "HitStop.h"
#include "Character.h"
#include "Enemy.h"
#include "Player.h"
#include "BeastBehaviorTreeHeader.h"
#include "Beast.h"
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
Beast::Beast()
{
	/*�����o�N���X�̃C���X�^���X�̍쐬*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::BEAST));

	/*�A�j���[�V�����̐ݒ�*/
	auto& json = Singleton<JsonManager>::GetInstance();
	int				animationHandle = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_HANDLE"];
	vector<int>		animationIndex = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_INDEX"];
	this->nowAnimation = static_cast<int>(AnimationType::IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	//�A�j���[�V�����̒ǉ�
	for (int i = 0; i < animationIndex.size(); i++)
	{
		this->animation->Add(animationHandle, animationIndex[i]);
	}
	//�A�j���[�V�����̃A�^�b�`
	this->animation->Attach(&this->modelHandle);

	/*�R���C�_�[�f�[�^�̍쐬*/
	this->maxHp = json.GetJson(JsonManager::FileType::BEAST)["HP"];
	this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BEAST, new CharacterData());
	this->maxPartsColliderNum = json.GetJson(JsonManager::FileType::BEAST)["COLLIDER_NUM"];
	this->frameIndexUsePartsColider = json.GetJson(JsonManager::FileType::BEAST)["FRAME_INDEX_USE_PARTS_COLLIDER"];
	for (int i = 0; i < this->maxPartsColliderNum; i++)
	{
		this->partsCollider.emplace_back(new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BEAST, new CharacterData()));
		this->partsCollider[i]->data->hp = this->maxHp;
		this->partsCollider[i]->radius = json.GetJson(JsonManager::FileType::BEAST)["PARTS_COLL_RADIUS"][i];
		this->partsCollider[i]->isUseCollWithChara = false;
		this->partsCollider[i]->isUseCollWithGround = false;
		this->partsCollider[i]->isSetTopPosition = true;
		this->partsCollider[i]->type = CharacterColliderData::CharaType::LUX;
		this->partsCollider[i]->rigidbody.Initialize(false);
		this->prevPartsHp.emplace_back(this->maxHp);
		this->pos1.emplace_back(Gori::ORIGIN);
		this->pos2.emplace_back(Gori::ORIGIN);
	}

	/*�c���[�̍쐬*/
	this->tree = new BeastBehaviorTree();

	/*�ʏ펞�̃J���[�X�P�[�����擾*/
	this->normalColor = MV1GetDifColorScale(this->modelHandle);
	this->tiredColor = this->angryColor = this->normalColor;
	this->tiredColor.b += 10;
	this->angryColor.r += 10;
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast::~Beast()
{
	Finalize();
	for (int i = 0; i < this->maxPartsColliderNum; i++)
	{
		DeleteMemberInstance(this->partsCollider[i]);
	}
	this->partsCollider.clear();
}

/// <summary>
/// ������
/// </summary>
void Beast::Initialize()
{
	/*�ϐ��̏�����*/
	//Character�N���X
	this->nextRotation	= Gori::ORIGIN;
	this->isAlive		= true;
	this->speed			= 0.0f;
	this->entryInterval = 0;
	this->isDraw		= true;
	//Enemy�N���X
	this->moveTarget		= Gori::ORIGIN;
	this->animationPlayTime = 0.0f;
	this->nowAnimation		= static_cast<int>(AnimationType::ROAR);
	this->bossState			= static_cast<int>(BossState::NORMAL);
	this->angryValue		= 0;
	this->tiredValue		= 0;
	this->attackCount		= 0;
	this->tiredDuration		= 0;
	//Beast�N���X
	auto& json = Singleton<JsonManager>::GetInstance();
	float defenisivePower = json.GetJson(JsonManager::FileType::BEAST)["DEFENSIVE_POWER"][this->bossState];
	for (int i = 0; i < this->maxPartsColliderNum; i++)
	{
		this->prevPartsHp[i]						 = this->maxHp;
		this->partsCollider[i]->data->hp			 = this->maxHp;
		this->partsCollider[i]->isUseCollWithChara	 = true;
		this->partsCollider[i]->isSetTopPosition	 = true;
		this->partsCollider[i]->data->defensivePower = defenisivePower;
		this->partsCollider[i]->rigidbody.Initialize(true);
	}
	//�r�w�C�r�A�c���[��������
	this->tree->Initialize();

	/*�R���C�_�[�̏�����*/
	float height						 = json.GetJson(JsonManager::FileType::BEAST)["HIT_HEIGHT"];
	this->collider->topPositon			 = VGet(0.0f, height, 0.0f);
	this->collider->radius				 = json.GetJson(JsonManager::FileType::BEAST)["HIT_RADIUS"];
	this->collider->isUseCollWithChara	 = false;
	this->collider->isUseCollWithGround  = true;
	this->collider->data->defensivePower = defenisivePower;
	this->collider->data->hp			 = this->maxHp;
	this->collider->data->isHit			 = false;
	this->collider->data->damage		 = 0;
	
	/*�R���C�_�[���̃��W�b�h�{�f�B�̏�����*/
	const VECTOR POSITION = Gori::Convert(json.GetJson(JsonManager::FileType::BEAST)["INIT_POSITION"]);//���W
	const VECTOR ROTATION = Gori::Convert(json.GetJson(JsonManager::FileType::BEAST)["INIT_ROTATION"]);//��]��
	const VECTOR DIRECTION = Gori::Convert(json.GetJson(JsonManager::FileType::BEAST)["INIT_DIRECTION"]);//��]��
	const VECTOR SCALE = Gori::Convert(json.GetJson(JsonManager::FileType::BEAST)["INIT_SCALE"]);	 //�g�嗦
	this->collider->rigidbody.Initialize(true);
	this->collider->rigidbody.SetPosition(POSITION);
	this->collider->rigidbody.SetVelocity(DIRECTION);
	this->collider->rigidbody.SetRotation(ROTATION);
	this->collider->rigidbody.SetScale(SCALE);
	MV1SetPosition	 (this->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->modelHandle, this->collider->rigidbody.GetScale());

	/*�A�j���[�V�����̃A�^�b�`*/
	this->animation->Attach(&this->modelHandle);

	SetAttackCount();
	ChangeNormalColor();
}

/// <summary>
/// �㏈��
/// </summary>
void Beast::Finalize()
{
	for (int i = 0; i < this->maxPartsColliderNum; i++)
	{
		this->partsCollider[i]->isUseCollWithChara = false;
	}
}

/// <summary>
/// �X�V
/// </summary>
void Beast::Update()
{
	/*�R���C�_�[�̍X�V*/
	for (int i = 0; i < this->maxPartsColliderNum; i++)
	{
		//���W�̐ݒ�
		this->pos1[i] = MV1GetFramePosition(this->modelHandle, this->frameIndexUsePartsColider[i][0]);
		this->pos2[i] = MV1GetFramePosition(this->modelHandle, this->frameIndexUsePartsColider[i][1]);
		this->partsCollider[i]->rigidbody.SetPosition(this->pos1[i]);
		this->partsCollider[i]->topPositon = this->pos2[i];
		//�_���[�W�̌v�Z
		if (this->partsCollider[i]->data->isHit)
		{
			this->partsCollider[i]->data->isHit = false;
			int damage = this->prevPartsHp[i] - this->partsCollider[i]->data->hp;
			this->prevPartsHp[i] = this->partsCollider[i]->data->hp;
			this->collider->data->damage = static_cast<float>(damage);
			this->collider->data->hp -= damage;
			this->collider->data->isHit = true;
			this->collider->data->hitStopTime = this->partsCollider[i]->data->hitStopTime;
			this->collider->data->hitStopType = this->partsCollider[i]->data->hitStopType;
			this->collider->data->hitStopDelay = this->partsCollider[i]->data->hitStopDelay;
			this->collider->data->slowFactor = this->partsCollider[i]->data->slowFactor;
		}
	}

	UpdateBossState();

	/*�q�b�g�X�g�b�v*/
	if (this->collider->data->isHit)
	{
		this->hitStop->SetHitStop(this->collider->data->hitStopTime, this->collider->data->hitStopType, this->collider->data->hitStopDelay, this->collider->data->slowFactor);
		this->collider->data->isHit = false;
	}
	if (this->hitStop->IsHitStop()) return;


	/*�X�e�[�W�O�ɏo����f�X*/
	if (this->collider->rigidbody.GetPosition().y < -30.0f)
	{
		RespawnIfOutOfStage();
	}

	/*�r�w�C�r�A�c���[�̍X�V*/
	this->tree->Update(*this);

	this->positionForLockon = MV1GetFramePosition(this->modelHandle, 8);
	this->positionForLockon.y = this->collider->rigidbody.GetPosition().y;
	this->positionForLockon.y += this->LOCKON_OFFSET;
}

/// <summary>
/// �A�j���[�V�����̍Đ�
/// </summary>
void Beast::PlayAnimation()
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
/// �`��
/// </summary>
const void Beast::DrawCharacterInfo()const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& shadow = Singleton<Shadow>::GetInstance();
	auto& map = Singleton<MapManager>::GetInstance();

#ifdef _DEBUG
	//auto& debug = Singleton<Debug>::GetInstance();
	//auto& tree = Singleton<BeastBehaviorTree>::GetInstance();
	//tree.Draw();
	//if (debug.IsShowDebugInfo(Debug::ItemType::ENEMY))
	//{
	//	//VECTOR position = this->collider->rigidbody.GetPosition();
	//	//VECTOR rotation = this->collider->rigidbody.GetRotation();
	//	//printfDx("Beast_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	//	//printfDx("Beast_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
	//}

	//for (int i = 0; i < this->maxPartsColliderNum; i++)
	//{
	//	VECTOR pos1 = this->pos1[i];
	//	VECTOR pos2 = this->pos2[i];
	//	DrawCapsule3D(pos1, pos2, this->partsCollider[i]->radius, 16, GetColor(0, 0, 255), GetColor(0, 0, 255), FALSE);
	//	VECTOR underPos = this->partsCollider[i]->rigidbody.GetPosition();
	//	VECTOR topPos = this->partsCollider[i]->topPositon;
	//	DrawCapsule3D(underPos, topPos, this->partsCollider[i]->radius, 16, GetColor(0, 255, 0), GetColor(0, 255, 0), FALSE);
	//}
#endif // _DEBUG

	if (this->isDraw)
	{
		/*�����̕`��*/
		shadow.Draw(map.GetStageModelHandle(), this->collider->rigidbody.GetPosition(), this->SHADOW_HEIGHT, this->SHADOW_SIZE);
	}
}

const bool Beast::GetIsAttack()const
{
	return false;
}

/// <summary>
/// �A�j���[�V�����Đ����Ԃ̎擾
/// </summary>
const float Beast::GetAnimationPlayTime()const
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	return json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nowAnimation];
}

/// <summary>
/// ��]���̐ݒ�
/// </summary>
void Beast::SetRotation(const VECTOR _rotation)
{
	this->collider->rigidbody.SetRotation(_rotation);
}

/// <summary>
/// �ړ��x�N�g���̐ݒ�
/// </summary>
void Beast::SetVelocity(const VECTOR _velocity)
{
	this->collider->rigidbody.SetVelocity(_velocity);
}

/// <summary>
/// �U���R���{�񐔂̃Z�b�g
/// </summary>
void Beast::SetAttackCount()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�R���{���̐ݒ�*/
	if (this->bossState == static_cast<int>(BossState::NORMAL))
	{
		this->attackCount = json.GetJson(JsonManager::FileType::BEAST)["ATTACK_COMBO_COUNT"][this->bossState];
	}
}

/// <summary>
/// �{���Ԃ̐ݒ�
/// </summary>
void Beast::UpdateBossState()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*��Ԃ̐؂�ւ�*/
	switch (this->bossState)
	{
		//�{��
	case static_cast<int>(BossState::ANGRY):
		//�U���񐔂��O���������Ԃ�ʏ�ɖ߂�
		if (this->attackCount <= 0)
		{
			this->bossState = static_cast<int>(BossState::NORMAL);
			this->angryValue = 0;
			ChangeNormalColor();
			auto& json = Singleton<JsonManager>::GetInstance();
			float defenisivePower = json.GetJson(JsonManager::FileType::BEAST)["DEFENSIVE_POWER"][this->bossState];
			for (auto& collider : this->partsCollider)
			{
				collider->data->defensivePower = defenisivePower;
			}
		}
		break;
		//�ʏ�
	case static_cast<int>(BossState::NORMAL):
		//�U�����������Ă�����e��ԕω��p�̒l�𑝉�������
		if (this->collider->data->isHit)
		{
			this->angryValue += this->collider->data->damage;
			this->tiredValue += this->collider->data->damage;
		}
		//���Q�[�W���ő�ȏゾ�������Ԃ�TIRED�ɂ���
		if (this->tiredValue >= json.GetJson(JsonManager::FileType::ENEMY)["MAX_TIRED_VALUE"])
		{
			this->bossState = static_cast<int>(BossState::TIRED);
			this->attackCount = 0;
			this->tiredValue = 0;
			float defenisivePower = json.GetJson(JsonManager::FileType::BEAST)["DEFENSIVE_POWER"][this->bossState];
			for (auto& collider : this->partsCollider)
			{
				collider->data->defensivePower = defenisivePower;
			}
			ChangeTiredColor();
		}
		//�{��l���ő�ȏゾ�������Ԃ�ANGRY�ɂ���
		if (this->angryValue >= json.GetJson(JsonManager::FileType::ENEMY)["MAX_ANGRY_VALUE"])
		{
			this->tree->SetInterval(static_cast<int>(BeastBehaviorTree::ActionType::ROAR),0);
			this->bossState = static_cast<int>(BossState::ANGRY);
			this->attackCount = json.GetJson(JsonManager::FileType::BEAST)["ATTACK_COMBO_COUNT"][this->bossState];
			float defenisivePower = json.GetJson(JsonManager::FileType::BEAST)["DEFENSIVE_POWER"][this->bossState];
			for (auto& collider : this->partsCollider)
			{
				collider->data->defensivePower = defenisivePower;
			}
			ChangeAngryColor();
		}
		break;
		//���
	case static_cast<int>(BossState::TIRED):
		//��ꎞ�Ԃ𑝉�
		this->tiredDuration++;
		//�ő�l�𒴂������Ԃ�ʏ�ɕύX
		if (this->tiredDuration >= json.GetJson(JsonManager::FileType::ENEMY)["TIRED_DURATION"])
		{
			this->bossState = static_cast<int>(BossState::NORMAL);
			ChangeNormalColor();
			this->tiredDuration = 0;
			float defenisivePower = json.GetJson(JsonManager::FileType::BEAST)["DEFENSIVE_POWER"][this->bossState];
			for (auto& collider : this->partsCollider)
			{
				collider->data->defensivePower = defenisivePower;
			}
		}
		break;
	}
}
