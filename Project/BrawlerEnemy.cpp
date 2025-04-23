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
#include "Boid.h"
#include "Enemy.h"
#include "BrawlerEnemyBehaviorTreeHeader.h"
#include "HitStop.h"
#include "BrawlerEnemy.h"
#include "LoadingAsset.h"
#include "HitStopManager.h"
#include "Shadow.h"
#include "MapManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BrawlerEnemy::BrawlerEnemy()
	: indentNum	(0)
	, bossType	(0)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�����o�N���X�̃C���X�^���X�̍쐬*/
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::BRAWLER_ENEMY));


	/*�A�j���[�V�����̐ݒ�*/
	vector<string>	animationHandle = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ANIMATION_HANDLE"];
	int				animationIndex	= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ANIMATION_INDEX"];
	this->nowAnimation		= static_cast<int>(AnimationType::IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	//�A�j���[�V�����̒ǉ�
	for (int i = 0; i < animationHandle.size(); i++)
	{
		int handle = MV1LoadModel(animationHandle[i].c_str());
		this->animation->Add(handle, animationIndex);
	}
	//�A�j���[�V�����̃A�^�b�`
	this->animation->Attach(&this->modelHandle);

	/*�R���C�_�[�f�[�^�̍쐬*/
	this->collider = new CharacterColliderData(ColliderData::Priority::LOW, GameObjectTag::WEAK_ENEMY, new CharacterData());

	/*�r�w�C�r�A�c���[�̍쐬*/
	this->tree = new BrawlerEnemyBehaviorTree();

	/*boid�̍쐬*/
	this->boid = new Boid();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BrawlerEnemy::~BrawlerEnemy()
{
	Finalize();
}

void BrawlerEnemy::Initialize()
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
	float height						 = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["HIT_HEIGHT"];
	this->collider->topPositon			 = VGet(0.0f, height, 0.0f);
	this->collider->radius				 = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["HIT_RADIUS"];
	this->collider->isUseCollWithChara	 = true;
	this->collider->isUseCollWithGround  = true;
	this->collider->data->defensivePower = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["DEFENSIVE_POWER"];
	this->collider->data->hp			 = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["HP"];
	this->collider->data->isHit			 = false;
	this->collider->type				 = CharacterColliderData::CharaType::BRAWLER;

	/*���������̏�����*/
	//json�f�[�^��萔�ɑ��
	const VECTOR ROTATION	= Gori::Convert(json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["INIT_ROTATION"]);//��]��
	const VECTOR DIRECTION	= Gori::Convert(json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["INIT_DIRECTION"]);//��]��
	const VECTOR SCALE		= Gori::Convert(json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["INIT_SCALE"]);	 //�g�嗦

	 //������
	this->collider->rigidbody.Initialize(true);
	this->collider->rigidbody.SetVelocity(DIRECTION);
	this->collider->rigidbody.SetRotation(ROTATION);
	this->collider->rigidbody.SetScale(SCALE);
	MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale(this->modelHandle, this->collider->rigidbody.GetScale());

	/*�A�j���[�V�����̃A�^�b�`*/
	this->animation->Attach(&this->modelHandle);
}

void BrawlerEnemy::SetSpownPosition(const int _indentNum, const int _bossType)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	const VECTOR POSITION = Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["DUNGEON_SPOWN_POSITION"][_bossType][_indentNum]);
	this->spownPosition = POSITION;
	this->collider->rigidbody.SetPosition(POSITION);
	MV1SetPosition(this->modelHandle, this->collider->rigidbody.GetPosition());

	float neighborRadius	 = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["NEIGHBOR_RADIUS"];
	float separationDistance = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["SEPARATION_DISTANCE"];
	float cohesionWeight	 = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["COHESION_WEIGHT"];
	float alignmentWeight	 = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ALIGNMENT_WEIGHT"];
	float separationWeight	 = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["SEPARATION_WEIGHT"];
	float targetRadius		 = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["TARGET_RADIUS"];
	float stageRadius		 = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["STAGE_RADIUS"];
	VECTOR stageCenter		 = Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["STAGE_CENTER_POSITION"][_bossType][_indentNum]);
	this->boid->Initialize(neighborRadius, cohesionWeight, alignmentWeight, separationDistance, separationWeight, targetRadius, stageRadius, stageCenter);
}

/// <summary>
/// �㏈��
/// </summary>
void BrawlerEnemy::Finalize()
{
	this->collider->isUseCollWithChara = false;
}

/// <summary>
/// �X�V
/// </summary>
void BrawlerEnemy::Update()
{
	VECTOR pos = this->collider->rigidbody.GetPosition();
	printfDx("Indent%d Braw X:%f,Y:%f,Z:%f\n", this->indentNum, pos.x, pos.y, pos.z);
	/*�X�e�[�W�O�ɏo���璆���ɖ߂�*/
	if (this->collider->rigidbody.GetPosition().y < -30.0f)
	{
		RespawnIfOutOfStage();
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
void BrawlerEnemy::PlayAnimation()
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
const void BrawlerEnemy::DrawCharacterInfo()const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& shadow = Singleton<Shadow>::GetInstance();
	auto& map = Singleton<MapManager>::GetInstance();

#ifdef _DEBUG
	//auto& debug = Singleton<Debug>::GetInstance();
	//auto& tree = Singleton<BrawlerEnemyBehaviorTree>::GetInstance();
	//tree.Draw();
	//if (debug.IsShowDebugInfo(Debug::ItemType::ENEMY))
	//{
	//	//VECTOR position = this->collider->rigidbody.GetPosition();
	//	//VECTOR rotation = this->collider->rigidbody.GetRotation();
	//	//printfDx("BrawlerEnemy_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	//	//printfDx("BrawlerEnemy_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
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

const bool BrawlerEnemy::GetIsAttack()const
{
	return false;
}