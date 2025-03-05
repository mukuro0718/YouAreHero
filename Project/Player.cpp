#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include <iostream>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "ReactionType.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "CharacterData.h"
#include "CharacterColliderData.h"
#include "Character.h"
#include "Animation.h"
#include "BitFlag.h"
#include "LoadingAsset.h"
#include "PlayerActionHeader.h"
#include "Player.h"
#include "PlayerController.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "EnemyManager.h"
#include "PlayerAttackManager.h"
#include "Debug.h"
#include "EffectManager.h"
#include "HitStop.h"
#include "Shadow.h"
#include "MapManager.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player()
	: controller	(nullptr)
	, healCount		(0)
	, isDrawSword	(true)
	, frameTime		(0)
	, isLock		(false)
	, isPrevPushLS	(false)
{
	/*�R���g���[���[�̍쐬*/
	this->controller = new PlayerController();

	/*���f���̕���*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->modelHandle = asset.GetModel(LoadingAsset::ModelType::PLAYER);

	/*�A�N�V�����N���X�̍쐬*/
	this->action.emplace_back(new PlayerIdle());
	this->action.emplace_back(new PlayerAvoid());
	this->action.emplace_back(new PlayerDeath());
	this->action.emplace_back(new PlayerBlock());
	this->action.emplace_back(new PlayerStagger());
	this->action.emplace_back(new PlayerBlockStagger());
	this->action.emplace_back(new PlayerKnockdown());
	this->action.emplace_back(new PlayerKnockup());
	this->action.emplace_back(new PlayerHeal());
	this->action.emplace_back(new PlayerRun());
	this->action.emplace_back(new PlayerRunOutOfStamina());
	this->action.emplace_back(new PlayerWalk());
	this->action.emplace_back(new PlayerCombo1());
	this->action.emplace_back(new PlayerCombo2());
	this->action.emplace_back(new PlayerCombo3());
	this->action.emplace_back(new PlayerStrongAttack());
	this->action.emplace_back(new PlayerDrawSword1());
	this->action.emplace_back(new PlayerDrawSword2());

	/*�A�j���[�V�����̐ݒ�*/
	auto& json = Singleton<JsonManager>::GetInstance();
	vector<int> animationHandle = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_HANDLE"];
	vector<int> animationIndex = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_INDEX"];
	//�A�j���[�V�����̒ǉ�
	for (int i = 0; i < animationHandle.size(); i++)
	{
		this->animation->Add(animationHandle[i], animationIndex[i]);
	}
	//�A�j���[�V�����̃A�^�b�`
	this->animation->Attach(&this->modelHandle);

	/*�R���C�_�[�f�[�^�̍쐬*/
	this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::PLAYER, new CharacterData());
	this->collider->type = CharacterColliderData::CharaType::PLAYER;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	DeleteMemberInstance(this->hitStop);
}

/// <summary>
/// ������
/// </summary>
void Player::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json		= Singleton<JsonManager>::GetInstance();
	auto& asset		= Singleton<LoadingAsset>::GetInstance();

	/*�ϐ��̏�����*/
	this->isAlive			= true;
	this->isDraw			= true;
	this->speed				= 0.0f;
	this->entryInterval		= 0;
	this->nextRotation		= Gori::ORIGIN;
	this->healCount			= json.GetJson(JsonManager::FileType::PLAYER)["MAX_HEAL_ORB_NUM"];
	this->frameTime			= 0;
	this->isDrawSword		= true;
	this->isPrevPushLS		= false;
	this->isLock			= false;
	/*�R���C�_�[�̏�����*/
	const VECTOR POSITION = Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_POSITION"]);//���W
	const VECTOR SCALE	  = Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_SCALE"]);	//�g�嗦
		  VECTOR rotation = Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_ROTATION"]);//��]��
		  rotation.y	  = rotation.y * (DX_PI_F / 180.0f);
	this->collider->rigidbody.Initialize(true);
	this->collider->rigidbody.SetPosition(POSITION);
	this->collider->rigidbody.SetRotation(rotation);
	this->collider->rigidbody.SetScale(SCALE);
	MV1SetPosition	 (this->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->modelHandle, this->collider->rigidbody.GetScale());

	float height				 = json.GetJson(JsonManager::FileType::PLAYER)["HIT_HEIGHT"];					//�J�v�Z���̍���
	this->collider->topPositon			 = /*VAdd(this->collider->rigidbody.GetPosition(), */VGet(0.0f, height, 0.0f)/*)*/;	//�J�v�Z���̏���W
	this->collider->radius				 = json.GetJson(JsonManager::FileType::PLAYER)["RADIUS"];						//�J�v�Z���̔��a
	this->collider->isUseCollWithGround  = true;																			//�n�ʂƂ̓����蔻����Ƃ邩
	this->collider->isUseCollWithChara	 = true;																			//�n�ʂƂ̓����蔻����Ƃ邩
	this->collider->data->hp			 = json.GetJson(JsonManager::FileType::PLAYER)["HP"];							//HP
	this->collider->data->stamina		 = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];						//�X�^�~�i
	this->collider->data->isInvinvible	 = false;																			//�_���[�W���J�b�g���邩
	this->collider->data->isGuard		 = false;																			//�_���[�W���J�b�g���邩
	this->collider->data->isHit		 = false;																			//�U�����q�b�g������

	/*�A�j���[�V�����̃A�^�b�`*/
	this->animation->Attach(&this->modelHandle);
}

/// <summary>
/// �㏈��
/// </summary>
void Player::Finalize()
{
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update()
{
	//int startTime = GetNowCount();
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/

	if (this->collider->rigidbody.GetPosition().y < -30.0f)
	{
		RespawnIfOutOfStage();
	}

	if (this->hitStop->IsHitStop()) return;

	auto& input = Singleton<InputManager>::GetInstance();
	if (input.GetNowPad(InputManager::PAD_RS))
	{
		if (!this->isPrevPushLS)
		{
			if (!this->isLock)
			{
				this->isLock = true;
			}
			else
			{
				this->isLock = false;
			}
			this->isPrevPushLS = true;
		}
	}
	else
	{
		this->isPrevPushLS = false;
	}

	/*��Ԃ̕ύX(0~1m/s)*/
	int  nowState = this->controller->GetNowState();
	bool isChancel = this->action[nowState]->GetIsChangeAction();
	bool isEndAction = this->action[nowState]->GetIsEndAction();
	bool isInitialize = this->controller->StateChanger(isChancel, isEndAction, this->isDrawSword, GetCharacterData(), this->healCount);
	nowState = this->controller->GetNowState();

	/*��Ԃ��قȂ��Ă�����A�N�V����������������*/
	if (isInitialize)
	{
		this->action[nowState]->Initialize();
		this->controller->SynchroState();
	}

	/*�A�N�V�����̍X�V*/
	this->action[nowState]->Update(*this);
	//��Ԃ��K�[�h����Ȃ���΃K�[�h�t���O������
	if (nowState != static_cast<int>(PlayerController::PlayerState::BLOCK) && nowState != static_cast<int>(PlayerController::PlayerState::BLOCK_STAGGER))
	{
		GetPlayerData().isGuard = false;
	}
	auto& sound = Singleton<SoundManager>::GetInstance();
	//��Ԃ����肶��Ȃ���Ε����T�E���h������
	if (sound.GetCheckEffectSoundState(SoundManager::EffectType::PLAYER_RUN) && nowState != static_cast<int>(PlayerController::PlayerState::RUN))
	{
		sound.OffIsPlayEffect(SoundManager::EffectType::PLAYER_RUN);
	}
	//��Ԃ���������Ȃ���Ε����T�E���h������
	if (sound.GetCheckEffectSoundState(SoundManager::EffectType::PLAYER_WALK) && nowState != static_cast<int>(PlayerController::PlayerState::WALK))
	{
		sound.OffIsPlayEffect(SoundManager::EffectType::PLAYER_WALK);
	}
	//int endTime = GetNowCount();
	//this->frameTime = endTime - startTime;
	/*���G�t���O�������Ă�����ő�HP����ς��Ȃ�*/
#ifdef _DEBUG
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& debug = Singleton<Debug>			::GetInstance();
	if (debug.IsShowDebugInfo(Debug::ItemType::PLAYER) && json.GetJson(JsonManager::FileType::DEBUG)["PLAYER_INVINCIBLE"])
	{
		GetPlayerData().hp = json.GetJson(JsonManager::FileType::PLAYER)["HP"];
	}
	//printfDx("PLAYER_FRAMETIME:%d\n", this->frameTime);
#endif // _DEBUG
}

/// <summary>
/// �`��
/// </summary>
const void Player::DrawCharacterInfo()const
{
#ifdef _DEBUG
	/*�f�o�b�N�\���t���O�������Ă�����\������*/
	//auto& debug = Singleton<Debug>::GetInstance();
	//if (debug.IsShowDebugInfo(Debug::ItemType::PLAYER))
	//{
		//VECTOR position = this->collider->rigidbody.GetPosition();
		//VECTOR direction = this->collider->rigidbody.GetDirection();
		//VECTOR rotation = this->collider->rigidbody.GetRotation();
		//printfDx("PLAYER_POSITION X:%f,Y:%f,Z:%f	\n", position.x, position.y, position.z);
		//printfDx("PLAYER_DIRECTION X:%f,Y:%f,Z:%f	\n", direction.x, direction.y, direction.z);
		//printfDx("PLAYER_ROTATION X:%f,Y:%f,Z:%f	\n", rotation.x, rotation.y, rotation.z);
		//printfDx("PLAYER_SPEED:%f					\n", this->speed);
		//auto& characterCollider = dynamic_cast<CharacterColliderData&> (*this->collider);
		//printfDx("%d:REACTION_TYPE				\n", characterthis->collider->data->reactionType);
		//printfDx("%d:PLAYER_ISHIT				\n", this->collider->data->isHit);
		//auto& json = Singleton<JsonManager>::GetInstance();
		//string stateInfo = json.GetJson(JsonManager::FileType::PLAYER)["STATE_INFO"][this->controller->GetNowState()];
		//printfDx(stateInfo.c_str());
	//}
#endif // _DEBUG
	
	/*�����̕`��*/
	if (this->isDraw)
	{
		auto& shadow = Singleton<Shadow>::GetInstance();
		auto& map = Singleton<MapManager>::GetInstance();
		shadow.Draw(map.GetStageModelHandle(), this->collider->rigidbody.GetPosition(), this->SHADOW_HEIGHT, this->SHADOW_SIZE);
	}
}



/// <summary>
/// �A�j���[�V�����̍Đ�
/// </summary>
void Player::PlayAnimation(const int _nextAnimation, const float _playTime)
{
	this->animation->Play(&this->modelHandle, _nextAnimation, _playTime);
}

void Player::DeathProcess()
{
	this->isAlive = false;
	this->isDraw = false;
}

/// <summary>
/// �K�v�ȏ���X�^�~�i�ƌ��݂̃X�^�~�i�̎c�ʂ��ׂăA�N�V�������\�Ȃ�true��Ԃ�
/// </summary>
const bool Player::CanAction(const float _staminaConsumed)const
{
	float staminaConsumed = _staminaConsumed * -1.0f;
	/*�X�^�~�i�̏���ʂ����݂̃X�^�~�i�̑��ʂ����������false��Ԃ�*/
	if (staminaConsumed > this->collider->data->stamina)return false;
	return true;
}

/// <summary>
/// �X�^�~�i�̌v�Z
/// </summary>
void Player::CalcStamina(const float _staminaConsumed, const float _maxStamina)
{
	this->collider->data->stamina += _staminaConsumed;

	/*����l�A�����l�𒴂��Ȃ��悤�ɒ���*/
	if (this->collider->data->stamina >= _maxStamina)
	{
		this->collider->data->stamina = _maxStamina;
	}
	else if (this->collider->data->stamina < 0)
	{
		this->collider->data->stamina = 0;
	}
}

/// <summary>
/// ���݂̏�Ԃ��擾
/// </summary>
const int Player::GetNowState()const
{
	return this->controller->GetNowState();
}


/// <summary>
/// �X�^�~�i�̎擾
/// </summary>
const float Player::GetStamina()const
{
	return this->collider->data->stamina;
}

/// <summary>
/// �L�����N�^�[�f�[�^�̎擾
/// ����������Ă���ƃf�[�^�̂��ׂĂ�ύX���邱�Ƃ��ł���̂ŁA
/// �����܂ŉ��ɂԂ牺�����Ă���N���X�݂̂Ŏg�p����悤�ɂ���
/// </summary>
CharacterData& Player::GetPlayerData()
{
	return *this->collider->data;
}

/// <summary>
/// ���W�b�h�{�f�B�̎擾
/// ����������Ă���ƃf�[�^�̂��ׂĂ�ύX���邱�Ƃ��ł���̂ŁA
/// �����܂ŉ��ɂԂ牺�����Ă���N���X�݂̂Ŏg�p����悤�ɂ���
/// </summary>
Rigidbody& Player::GetPlayerRigidbody()
{
	return this->collider->rigidbody;
}