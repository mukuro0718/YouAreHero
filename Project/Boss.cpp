#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "ReactionType.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "BossData.h"
#include "ColliderData.h"
#include "CharacterColliderData.h"
#include "BitFlag.h"
#include "Animation.h"
#include "Character.h"
#include "BossActionHeader.h"
#include "Boss.h"
#include "LoadingAsset.h"
#include "PlayerManager.h"
#include "CameraManager.h"
#include "BossAttackManager.h"
#include "EffectManager.h"
#include "Debug.h"
#include "HitStop.h"

/// <summary>
/// コンストラクタ
/// </summary>
Boss::Boss()
	: animationPlayTime(0.0f)
	, moveTarget			{ 0.0f, 0.0f, 0.0f }
	, nowAnimation			(0)
	, nowPhase				(0)
	, prevPhase				(-1)
	, actionType(0)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json  = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*メンバクラスのインスタンスの作成*/
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::ENEMY));

	/*アニメーションの設定*/
	vector<string>	animationHandle	  = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_HANDLE"];
	vector<int>		animationIndex	  = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_INDEX"];
			  this->nowAnimation	  = static_cast<int>(AnimationType::IDLE);
			  this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	//アニメーションの追加
	for (int i = 0; i < animationHandle.size(); i++)
	{
		this->animation->Add(MV1LoadModel(animationHandle[i].c_str()), animationIndex[i]);
	}
	//アニメーションのアタッチ
	this->animation->Attach(&this->modelHandle);

	/*アニメーションマップの設定*/
	this->stateAnimationMap.emplace(this->DYING, static_cast<int>(AnimationType::DYING));
	this->stateAnimationMap.emplace(this->IDLE, static_cast<int>(AnimationType::IDLE));
	this->stateAnimationMap.emplace(this->ROAR, static_cast<int>(AnimationType::ROAR));
	this->stateAnimationMap.emplace(this->WALK, static_cast<int>(AnimationType::WALK));
	this->stateAnimationMap.emplace(this->REST, static_cast<int>(AnimationType::IDLE));
	this->stateAnimationMap.emplace(this->SLASH, static_cast<int>(AnimationType::SLASH));
	this->stateAnimationMap.emplace(this->FLY_ATTACK, static_cast<int>(AnimationType::FLY_ATTACK));
	this->stateAnimationMap.emplace(this->HURRICANE_KICK, static_cast<int>(AnimationType::HURRICANE_KICK));
	this->stateAnimationMap.emplace(this->JUMP_ATTACK, static_cast<int>(AnimationType::JUMP_ATTACK));
	this->stateAnimationMap.emplace(this->ROTATE_PUNCH, static_cast<int>(AnimationType::ROTATE_PUNCH));
	/*アクションマップの作成*/
	this->actionTypeMap.emplace(static_cast<int>(ActionType::DYING),this->DYING);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::IDLE),this->IDLE);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::ROAR),this->ROAR);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::WALK),this->WALK);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::REST),this->REST);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::SLASH),this->SLASH);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::FLY_ATTACK),this->FLY_ATTACK);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::HURRICANE_KICK),this->HURRICANE_KICK);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::JUMP_ATTACK),this->JUMP_ATTACK);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::ROTATE_PUNCH),this->ROTATE_PUNCH);

	/*コライダーデータの作成*/
	CharacterData* data = new BossData();
	this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BOSS, data);

	/*アクションパラメーターの作成*/
	this->parameters.emplace_back(new BossDeathAction());
	this->parameters.emplace_back(new BossIdleAction());
	this->parameters.emplace_back(new BossRoarAction());
	this->parameters.emplace_back(new BossChaseAction());
	this->parameters.emplace_back(new BossRestAction());
	/*this->parameters.emplace_back(new BossSlashAction());
	this->parameters.emplace_back(new BossFlyAction());
	this->parameters.emplace_back(new BossHurricaneKickAction());
	this->parameters.emplace_back(new BossStabAction());
	this->parameters.emplace_back(new BossRotatePunchAction());*/
}

/// <summary>
/// デストラクタ
/// </summary>
Boss::~Boss()
{
	this->stateAnimationMap.clear();
}

void Boss::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<BossData&>(*collider.data);

	/*変数の初期化*/
	this->isAlive				 = false;
	this->isGround				 = true;
	this->isDraw = false;
	this->speed					 = 0.0f;
	this->animationPlayTime		 = 0.0f;
	this->entryInterval			 = 0;
	this->nowPhase				 = 0;
	this->prevPhase				 = -1;
	this->moveTarget			 = Gori::ORIGIN;
	this->nowAnimation			 = static_cast<int>(AnimationType::ROAR);
	this->actionType			 = static_cast<int>(ActionType::ROAR);
	for (int i = 0; i < this->parameters.size(); i++)
	{
		this->parameters[i]->Initialize();
	}
	float height		= json.GetJson(JsonManager::FileType::ENEMY)["HIT_HEIGHT"];
	collider.topPositon = /*VAdd(collider.rigidbody.GetPosition(),*/ VGet(0.0f, height, 0.0f)/*)*/;
	collider.radius		= json.GetJson(JsonManager::FileType::ENEMY)["HIT_RADIUS"];
	data.hp				= json.GetJson(JsonManager::FileType::ENEMY)["HP"];
	data.isHit			= false;

	/*フェーズの設定*/
	SetPhase();

	/*物理挙動の初期化*/
	//jsonデータを定数に代入
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_POSITION"]);//座標
	const VECTOR ROTATION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_ROTATION"]);//回転率
	const VECTOR SCALE = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_SCALE"]);	 //拡大率
	//初期化
	this->collider->rigidbody.Initialize(true);
	this->collider->rigidbody.SetPosition(POSITION);
	this->collider->rigidbody.SetRotation(ROTATION);
	this->collider->rigidbody.SetScale(SCALE);
	MV1SetPosition	 (this->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->modelHandle, this->collider->rigidbody.GetScale());

	this->state->ClearFlag(this->MASK_ALL);
	this->state->SetFlag(this->ROAR);

	/*アニメーションのアタッチ*/
	this->animation->Attach(&this->modelHandle);
}

/// <summary>
/// 後処理
/// </summary>
void Boss::Finalize()
{
}

/// <summary>
/// 更新
/// </summary>
void Boss::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& hitStop = Singleton<HitStop>			::GetInstance();

	if (!this->isAlive)
	{
		if (player.GetIsAlive())
		{
			if (this->entryInterval == json.GetJson(JsonManager::FileType::ENEMY)["ON_ENTRY_EFFECT_INTERVAL"])
			{
				auto& effect = Singleton<EffectManager>::GetInstance();
				effect.OnIsEffect(EffectManager::EffectType::BOSS_ENTRY);
			}
			this->entryInterval++;
			if (this->entryInterval >= json.GetJson(JsonManager::FileType::ENEMY)["ENTRY_INTERVAL"])
			{
				this->entryInterval = 0;
				this->isAlive = true;
				this->isDraw = true;
			}
		}
	}
	else
	{
		/*フェーズの初期化*/
		SetPhase();

		/*状態の切り替え*/
		ChangeState();

		if (!hitStop.IsHitStop())
		{

			/*ここですべてのパラメータの計算を行う*/
			for (auto& item : this->parameters)
			{
				item->CalcParameter(*this);
			}

			/*ここに各アクションごとの更新処理を入れたい*/
			this->parameters[this->actionType]->Update(*this);

			/*アニメーションの更新*/
			unsigned int nowState = this->state->GetFlag();
			this->nowAnimation = this->stateAnimationMap[nowState];

			//アニメーションの再生
			VECTOR position = this->collider->rigidbody.GetPosition();
			this->animation->Play(&this->modelHandle, position, this->nowAnimation, this->animationPlayTime);
			this->collider->rigidbody.SetPosition(position);
		}
	}
}

void Boss::ChangeState()
{
	/*選択されているか調べる*/
	bool isSelect		= false;//選択されているか
	int  sumDesireValue = 0;	//欲求値の合計
	for (auto& item : this->parameters)
	{
		isSelect		= item->GetIsSelect();
		sumDesireValue += item->GetDesireValue();
		if (isSelect)return;
	}
	
	/*今立っているフラグを下す*/
	unsigned int clearFlag = this->actionTypeMap[this->actionType];
	this->state->ClearFlag(clearFlag);

	/*選択されていなかったら*/
	if (!isSelect)
	{
		//各行動の期待値を求める
		std::vector<int> actionWeight;//重み
		for (auto& item : this->parameters)
		{
			actionWeight.emplace_back(item->GetWeight(sumDesireValue));
		}
		//重みをランダムで出す
		int randomWeight = GetRand(this->parameters[0]->GetBaseWeight() - 1);
		//forでvectorを回し、重みが０以下になったところのアクションを行う
		for (int i = 0; i < actionWeight.size(); i++)
		{
			randomWeight -= actionWeight[i];
			if (randomWeight < 0)
			{
				this->actionType = i;
				this->parameters[i]->OnIsSelect();
				break;
			}
		}
	}

	unsigned int setFlag = this->actionTypeMap[this->actionType];
	this->state->SetFlag(setFlag);
}

/// <summary>
/// 描画
/// </summary>
const void Boss::DrawCharacterInfo()const
{
	auto& debug = Singleton<Debug>::GetInstance();
	if (debug.CheckEnemyFlag())
	{
		VECTOR position = this->collider->rigidbody.GetPosition();
		VECTOR rotation = this->collider->rigidbody.GetRotation();
		printfDx("Boss_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
		printfDx("Boss_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
		printfDx("%d:DYING					\n", this->state->CheckFlag(this->DYING));
		printfDx("%d:IDLE						\n", this->state->CheckFlag(this->IDLE));
		printfDx("%d:ROAR						\n", this->state->CheckFlag(this->ROAR));
		printfDx("%d:WALK						\n", this->state->CheckFlag(this->WALK));
		printfDx("%d:REST						\n", this->state->CheckFlag(this->REST));
		printfDx("%d:SLASH					\n", this->state->CheckFlag(this->SLASH));
		printfDx("%d:FLY_ATTACK				\n", this->state->CheckFlag(this->FLY_ATTACK));
		printfDx("%d:HURRICANE_KICK			\n", this->state->CheckFlag(this->HURRICANE_KICK));
		printfDx("%d:JUMP_ATTACK				\n", this->state->CheckFlag(this->JUMP_ATTACK));
		printfDx("%d:ROTATE_PUNCH				\n", this->state->CheckFlag(this->ROTATE_PUNCH));
	}
}





const bool Boss::GetIsAttack()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))return true;
	return false;
}
const VECTOR Boss::GetHeadPosition()const
{
	return MV1GetFramePosition(this->modelHandle, 7);
}

/// <summary>
/// アニメーション再生時間の取得
/// </summary>
const float Boss::GetAnimationPlayTime()const
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	return json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nowAnimation];
}

/// <summary>
/// フェーズの設定
/// </summary>
void Boss::SetPhase()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<BossData&>(*collider.data);
	const float HP = data.hp;
	const float MAX_HP = json.GetJson(JsonManager::FileType::ENEMY)["HP"];

	/*HPが最大値の1/3未満だったらフェーズ３*/
	if (HP < (MAX_HP / 3))
	{
		this->nowPhase = static_cast<int>(Phase::PHASE_3);
	}
	/*HPが最大値の2/3未満だったらフェーズ2*/
	else if (HP < (MAX_HP / 3) * 2)
	{
		this->nowPhase = static_cast<int>(Phase::PHASE_2);
	}
	/*フェーズ１*/
	else
	{
		this->nowPhase = static_cast<int>(Phase::PHASE_1);
	}
}


/// <summary>
/// 回転率の設定
/// </summary>
void Boss::SetRotation(const VECTOR _rotation)
{
	this->collider->rigidbody.SetRotation(_rotation);
}

/// <summary>
/// 移動ベクトルの設定
/// </summary>
void Boss::SetVelocity(const VECTOR _velocity)
{
	this->collider->rigidbody.SetVelocity(_velocity);
}