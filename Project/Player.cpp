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
#include "PlayerData.h"
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

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
	: healCount	(0)
	, hitStop	(nullptr)
	, controller(nullptr)
{
	/*コントローラーの作成*/
	this->controller = new PlayerController();

	/*モデルの複製*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->modelHandle = asset.GetModel(LoadingAsset::ModelType::PLAYER);

	/*アクションクラスの作成*/
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

	/*ヒットストップクラスの作成*/
	this->hitStop = new HitStop();

	/*アニメーションの設定*/
	auto& json = Singleton<JsonManager>::GetInstance();
	vector<int> animationHandle = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_HANDLE"];
	vector<int> animationIndex = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_INDEX"];
	//アニメーションの追加
	for (int i = 0; i < animationHandle.size(); i++)
	{
		this->animation->Add(animationHandle[i], animationIndex[i]);
	}
	//アニメーションのアタッチ
	this->animation->Attach(&this->modelHandle);

	/*コライダーデータの作成*/
	CharacterData* data = new PlayerData;
	this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::PLAYER, data);
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	DeleteMemberInstance(this->hitStop);
}

/// <summary>
/// 初期化
/// </summary>
void Player::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json		= Singleton<JsonManager>::GetInstance();
	auto& asset		= Singleton<LoadingAsset>::GetInstance();
	auto& collider	= dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data		= dynamic_cast<PlayerData&>(*collider.data);

	/*変数の初期化*/
	this->isAlive			 = true;
	this->isDraw			 = true;
	this->isGround			 = false;
	this->isInitialize		 = true;
	this->speed				 = 0.0f;
	this->entryInterval		 = 0;
	this->nextRotation		 = Gori::ORIGIN;
	this->healCount			 = json.GetJson(JsonManager::FileType::PLAYER)["MAX_HEAL_ORB_NUM"];	//最大回復オーブ数
	
	/*コライダーの初期化*/
	const VECTOR POSITION = Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_POSITION"]);//座標
	const VECTOR SCALE	  = Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_SCALE"]);	//拡大率
		  VECTOR rotation = Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_ROTATION"]);//回転率
		  rotation.y	  = rotation.y * (DX_PI_F / 180.0f);
	this->collider->rigidbody.Initialize(true);
	this->collider->rigidbody.SetPosition(POSITION);
	this->collider->rigidbody.SetRotation(rotation);
	this->collider->rigidbody.SetScale(SCALE);
	MV1SetPosition	 (this->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->modelHandle, this->collider->rigidbody.GetScale());

	float height				 = json.GetJson(JsonManager::FileType::PLAYER)["HIT_HEIGHT"];		//カプセルの高さ
	collider.topPositon			 = /*VAdd(collider.rigidbody.GetPosition(), */VGet(0.0f, height, 0.0f)/*)*/;	//カプセルの上座標
	collider.radius				 = json.GetJson(JsonManager::FileType::PLAYER)["RADIUS"];			//カプセルの半径
	collider.isUseCollWithGround = true;
	data.hp						 = json.GetJson(JsonManager::FileType::PLAYER)["HP"];				//HP
	data.stamina				 = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];			//スタミナ
	data.isInvinvible			 = false;															//ダメージをカットするか
	data.isGuard				 = false;															//ダメージをカットするか
	data.isHit					 = false;															//攻撃がヒットしたか

	/*アニメーションのアタッチ*/
	this->animation->Attach(&this->modelHandle);
}

/// <summary>
/// 後処理
/// </summary>
void Player::Finalize()
{
}

/// <summary>
/// 更新
/// </summary>
void Player::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	if (this->hitStop->IsHitStop()) return;


	/*状態の変更*/
	int nowState = this->controller->GetNowState();
	bool isChancel = this->action[nowState]->GetIsChangeAction();
	bool isEndAction = this->action[nowState]->GetIsEndAction();
	bool isInitialize = this->controller->StateChanger(isChancel, isEndAction, GetCharacterData());
	nowState = this->controller->GetNowState();

	/*状態が異なっていたらアクションを初期化する*/
	if (isInitialize)
	{
		this->action[nowState]->Initialize();
		this->controller->SynchroState();
	}

	/*アクションの更新*/
	this->action[nowState]->Update(*this);
	//状態がガードじゃなければガードフラグを下す
	if (nowState != static_cast<int>(PlayerController::PlayerState::BLOCK))
	{
		GetPlayerData().isGuard = false;
	}

	/*無敵フラグが立っていたら最大HPから変えない*/
	auto& debug = Singleton<Debug>			::GetInstance();
	if (debug.IsShowDebugInfo(Debug::ItemType::PLAYER) && json.GetJson(JsonManager::FileType::DEBUG)["PLAYER_INVINCIBLE"])
	{
		GetPlayerData().hp = json.GetJson(JsonManager::FileType::PLAYER)["HP"];
	}
}

/// <summary>
/// 描画
/// </summary>
const void Player::DrawCharacterInfo()const
{
	/*デバック表示フラグが立っていたら表示する*/
	//auto& debug = Singleton<Debug>::GetInstance();
	//if (debug.IsShowDebugInfo(Debug::ItemType::PLAYER))
	//{
		VECTOR position = this->collider->rigidbody.GetPosition();
		VECTOR direction = this->collider->rigidbody.GetDirection();
		VECTOR rotation = this->collider->rigidbody.GetRotation();
		printfDx("PLAYER_POSITION X:%f,Y:%f,Z:%f	\n", position.x, position.y, position.z);
		printfDx("PLAYER_DIRECTION X:%f,Y:%f,Z:%f	\n", direction.x, direction.y, direction.z);
		printfDx("PLAYER_ROTATION X:%f,Y:%f,Z:%f	\n", rotation.x, rotation.y, rotation.z);
		printfDx("PLAYER_SPEED:%f					\n", this->speed);
		auto& characterCollider = dynamic_cast<CharacterColliderData&> (*this->collider);
		printfDx("%d:REACTION_TYPE				\n", characterCollider.data->playerReaction);
		auto& json = Singleton<JsonManager>::GetInstance();
		string stateInfo = json.GetJson(JsonManager::FileType::PLAYER)["STATE_INFO"][this->controller->GetNowState()];
		printfDx(stateInfo.c_str());
	//}
	
	/*かげの描画*/
	if (this->isDraw)
	{
		auto& shadow = Singleton<Shadow>::GetInstance();
		auto& map = Singleton<MapManager>::GetInstance();
		shadow.Draw(map.GetStageModelHandle(), this->collider->rigidbody.GetPosition(), this->SHADOW_HEIGHT, this->SHADOW_SIZE);
	}
}



/// <summary>
/// アニメーションの再生
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
/// 必要な消費スタミナと現在のスタミナの残量を比べてアクションが可能ならtrueを返す
/// </summary>
const bool Player::CanAction(const float _staminaConsumed)const
{
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<PlayerData&>(*collider.data);
	float staminaConsumed = _staminaConsumed * -1.0f;
	/*スタミナの消費量が現在のスタミナの総量よりも多ければfalseを返す*/
	if (staminaConsumed > data.stamina)return false;
	return true;
}

/// <summary>
/// スタミナの回復処理
/// </summary>
void Player::CalcStamina(const float _staminaConsumed)
{
	auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<PlayerData&>(*collider.data);

	/*それ以外の状態だったら状態に応じてスタミナを消費する*/
	//走り
	data.stamina += _staminaConsumed;
	/*上限値、下限値を超えないように調整*/
	if (data.stamina >= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"])
	{
		data.stamina = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	}
	else if (data.stamina < 0)
	{
		data.stamina = 0;
	}
}

/// <summary>
/// スタミナの取得
/// </summary>
const int Player::GetStamina()const
{
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<PlayerData&>(*collider.data);
	return data.stamina;
}

/// <summary>
/// キャラクターデータの取得
/// これを持ってくるとデータのすべてを変更することができるので、
/// あくまで下にぶら下がっているクラスのみで使用するようにする
/// </summary>
CharacterData& Player::GetPlayerData()
{
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	return *collider.data;
}

/// <summary>
/// リジッドボディの取得
/// これを持ってくるとデータのすべてを変更することができるので、
/// あくまで下にぶら下がっているクラスのみで使用するようにする
/// </summary>
Rigidbody& Player::GetPlayerRigidbody()
{
	return this->collider->rigidbody;
}

void Player::SetHitStop(const float _time, const int _type, const float _delay, const float _factor)
{
	this->hitStop->SetHitStop(_time, _type, _delay, _factor);
}
