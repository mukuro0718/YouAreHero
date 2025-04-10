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
#include "Animation.h"
#include "Character.h"
#include "Enemy.h"
#include "TankEnemyBehaviorTreeHeader.h"
#include "HitStop.h"
#include "TankEnemy.h"
#include "LoadingAsset.h"
#include "HitStopManager.h"
#include "Shadow.h"
#include "MapManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
TankEnemy::TankEnemy(const int _indentNum, const int _bossType)
	: indentNum(_indentNum)
	, bossType(_bossType)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�����o�N���X�̃C���X�^���X�̍쐬*/
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::TANK_ENEMY));


	/*�A�j���[�V�����̐ݒ�*/
	vector<string>	animationHandle = json.GetJson(JsonManager::FileType::TANK_ENEMY)["ANIMATION_HANDLE"];
	int				animationIndex	= json.GetJson(JsonManager::FileType::TANK_ENEMY)["ANIMATION_INDEX"];
	this->nowAnimation = static_cast<int>(AnimationType::IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::TANK_ENEMY)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	//�A�j���[�V�����̒ǉ�
	for (int i = 0; i < animationHandle.size(); i++)
	{
		int handle = MV1LoadModel(animationHandle[i].c_str());
		this->animation->Add(handle, animationIndex);
	}
	//�A�j���[�V�����̃A�^�b�`
	this->animation->Attach(&this->modelHandle);

	/*�R���C�_�[�f�[�^�̍쐬*/
	this->collider = new CharacterColliderData(ColliderData::Priority::LOW, GameObjectTag::BOSS, new CharacterData());

	this->tree = new TankEnemyBehaviorTree();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TankEnemy::~TankEnemy()
{
	Finalize();
}

void TankEnemy::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	//auto& player = Singleton<PlayerManager>::GetInstance();

	/*�ϐ��̏�����*/
	this->bossState						 = static_cast<int>(BossState::NORMAL);
	this->isAlive						 = true;
	this->isDraw						 = true;
	this->speed							 = 0.0f;
	this->animationPlayTime				 = 0.0f;
	this->entryInterval					 = 0;
	this->moveTarget					 = Gori::ORIGIN;
	this->nowAnimation					 = static_cast<int>(AnimationType::IDLE);
	float height						 = json.GetJson(JsonManager::FileType::TANK_ENEMY)["HIT_HEIGHT"];
	this->collider->topPositon			 = VGet(0.0f, height, 0.0f);
	this->collider->radius				 = json.GetJson(JsonManager::FileType::TANK_ENEMY)["HIT_RADIUS"];
	this->collider->isUseCollWithChara	 = true;
	this->collider->isUseCollWithGround	 = true;
	this->collider->data->defensivePower = json.GetJson(JsonManager::FileType::TANK_ENEMY)["DEFENSIVE_POWER"];
	this->collider->data->hp			 = json.GetJson(JsonManager::FileType::TANK_ENEMY)["HP"];
	this->collider->data->isHit			 = false;
	this->collider->type				 = CharacterColliderData::CharaType::BRAWLER;

	/*���������̏�����*/
	//json�f�[�^��萔�ɑ��
	const VECTOR POSITION	= Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["DUNGEON_SPOWN_POSITION"][this->bossType][this->indentNum]);//
	const VECTOR ROTATION	= Gori::Convert(json.GetJson(JsonManager::FileType::TANK_ENEMY)["INIT_ROTATION"]);//��]��
	const VECTOR DIRECTION	= Gori::Convert(json.GetJson(JsonManager::FileType::TANK_ENEMY)["INIT_DIRECTION"]);//��]��
	const VECTOR SCALE		= Gori::Convert(json.GetJson(JsonManager::FileType::TANK_ENEMY)["INIT_SCALE"]);	 //�g�嗦
	//������
	this->collider->rigidbody.Initialize(true);
	this->collider->rigidbody.SetPosition(POSITION);
	this->collider->rigidbody.SetVelocity(DIRECTION);
	this->collider->rigidbody.SetRotation(ROTATION);
	this->collider->rigidbody.SetScale(SCALE);
	MV1SetPosition		(this->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ	(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale			(this->modelHandle, this->collider->rigidbody.GetScale());

	/*�A�j���[�V�����̃A�^�b�`*/
	this->animation->Attach(&this->modelHandle);
}

/// <summary>
/// �㏈��
/// </summary>
void TankEnemy::Finalize()
{
	this->collider->isUseCollWithChara = false;
}

/// <summary>
/// �X�V
/// </summary>
void TankEnemy::Update()
{
	VECTOR pos = this->collider->rigidbody.GetPosition();
	printfDx("Indent%d Tank X:%f,Y:%f,Z:%f\n", this->indentNum, pos.x, pos.y, pos.z);

	/*�X�e�[�W�O�ɏo���璆���ɖ߂�*/
	if (this->collider->rigidbody.GetPosition().y < -30.0f)
	{
		RespawnIfOutOfStage();
	}

	if (this->collider->data->isHit)
	{
		this->hitStop->SetHitStop(this->collider->data->hitStopTime, this->collider->data->hitStopType, this->collider->data->hitStopDelay, this->collider->data->slowFactor);
		this->collider->data->isHit = false;
	}
	if (this->hitStop->IsHitStop()) return;

	/*�r�w�C�r�A�c���[�̍X�V*/
	this->tree->Update(*this);

	this->positionForLockon = this->collider->rigidbody.GetPosition();
	this->positionForLockon.y += this->LOCKON_OFFSET;

}

/// <summary>
/// �A�j���[�V�����̍Đ�
/// </summary>
void TankEnemy::PlayAnimation()
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
const void TankEnemy::DrawCharacterInfo()const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& shadow = Singleton<Shadow>::GetInstance();
	auto& map = Singleton<MapManager>::GetInstance();

#ifdef _DEBUG
	//auto& debug = Singleton<Debug>::GetInstance();
	//auto& tree = Singleton<TankEnemyBehaviorTree>::GetInstance();
	//tree.Draw();
	//if (debug.IsShowDebugInfo(Debug::ItemType::ENEMY))
	//{
	//	//VECTOR position = this->collider->rigidbody.GetPosition();
	//	//VECTOR rotation = this->collider->rigidbody.GetRotation();
	//	//printfDx("TankEnemy_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	//	//printfDx("TankEnemy_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
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

const bool TankEnemy::GetIsAttack()const
{
	return false;
}