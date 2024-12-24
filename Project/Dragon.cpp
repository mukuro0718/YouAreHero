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
//#include "DragonActionHeader.h"
#include "Player.h"
#include "Dragon.h"
#include "LoadingAsset.h"
#include "PlayerManager.h"
#include "CameraManager.h"
#include "EffectManager.h"
#include "Debug.h"
#include "HitStopManager.h"
#include "Shadow.h"
#include "MapManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Dragon::Dragon()
	: prevAttack(AttackType::NONE)
	, moveTarget			(Gori::ORIGIN)
	, animationPlayTime		(0.0f)
	, angryValue			(0.0f)
	, nowAnimation			(0)
	, nowAction				(0)
	, angryState			(0)
	, tiredInterval			(0)
	, attackComboCount		(0)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json  = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*メンバクラスのインスタンスの作成*/
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::DRAGON));

	/*アニメーションの設定*/
	vector<int>	animationHandle		  = json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_HANDLE"];
	vector<int>		animationIndex	  = json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_INDEX"];
			  this->nowAnimation	  = static_cast<int>(AnimationType::IDLE);
			  this->animationPlayTime = json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	//アニメーションの追加
	for (int i = 0; i < animationHandle.size(); i++)
	{
		this->animation->Add(animationHandle[i], animationIndex[i]);
	}
	//アニメーションのアタッチ
	this->animation->Attach(&this->modelHandle);

	/*コライダーデータの作成*/
	this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BOSS, new CharacterData());
}

/// <summary>
/// デストラクタ
/// </summary>
Dragon::~Dragon()
{
	Finalize();
}

void Dragon::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*変数の初期化*/
	this->isAlive			= true;
	this->isGround			= true;
	this->isDraw			= true;
	this->speed				= 0.0f;
	this->animationPlayTime	= 0.0f;
	this->entryInterval		= 0;
	this->moveTarget		= Gori::ORIGIN;
	this->nowAnimation		= static_cast<int>(AnimationType::ROAR);
	this->nowAction			= static_cast<int>(ActionType::ROAR);
	this->attackComboCount	= 0;
	this->angryState		= static_cast<int>(AngryStateType::NORMAL);
	float height			= json.GetJson(JsonManager::FileType::DRAGON)["HIT_HEIGHT"];
	this->collider->topPositon		= VGet(0.0f, height, 0.0f);
	this->collider->radius			= json.GetJson(JsonManager::FileType::DRAGON)["HIT_RADIUS"];
	this->collider->isUseCollWithGround = true;
	this->collider->data->hp					= json.GetJson(JsonManager::FileType::DRAGON)["HP"];
	this->collider->data->isHit				= false;
	

	/*物理挙動の初期化*/
	//jsonデータを定数に代入
	const VECTOR POSITION = Gori::Convert(json.GetJson(JsonManager::FileType::DRAGON)["INIT_POSITION"]);//座標
	const VECTOR ROTATION = Gori::Convert(json.GetJson(JsonManager::FileType::DRAGON)["INIT_ROTATION"]);//回転率
	const VECTOR SCALE = Gori::Convert(json.GetJson(JsonManager::FileType::DRAGON)["INIT_SCALE"]);	 //拡大率
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
	const COLOR_F BASE = { 1.0f,1.0f,1.0f,1.0f };
	MV1SetDifColorScale(this->modelHandle, BASE);
	MV1SetSpcColorScale(this->modelHandle, BASE);
	MV1SetEmiColorScale(this->modelHandle, BASE);
	MV1SetAmbColorScale(this->modelHandle, BASE);
}

/// <summary>
/// 後処理
/// </summary>
void Dragon::Finalize()
{
}

/// <summary>
/// 更新
/// </summary>
void Dragon::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	this->nowAction = static_cast<int>(AnimationType::ROAR);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	PlayAnimation();
}

/// <summary>
/// アニメーションの再生
/// </summary>
void Dragon::PlayAnimation()
{
	//アニメーションの再生
	if (this->isAlive)
	{
		VECTOR position = this->collider->rigidbody.GetPosition();
		this->animation->Play(&this->modelHandle, this->nowAnimation, this->animationPlayTime);
		this->collider->rigidbody.SetPosition(position);
	}
}

/// <summary>
/// 攻撃コンボ回数のセット
/// </summary>
void Dragon::SetAttackComboCount()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*コンボ数の設定*/
	this->attackComboCount = json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_COMBO_COUNT"][this->angryState];
}

/// <summary>
/// 状態の変更
/// </summary>
void Dragon::ChangeState()
{
	///*シングルトンクラスのインスタンスを取得*/
	//auto& debug = Singleton<Debug>::GetInstance();
	//auto& json = Singleton<JsonManager>::GetInstance();

	//int debugActionType = json.GetJson(JsonManager::FileType::DEBUG)["Dragon_ACTION"];
	//if (debug.IsShowDebugInfo(Debug::ItemType::ENEMY) && debugActionType != static_cast<int>(Dragon::ActionType::NONE))
	//{
	//	/*今立っているフラグを下す*/
	//	unsigned int clearFlag = this->actionTypeMap[this->nowAction];
	//	this->state->ClearFlag(clearFlag);
	//	this->nowAction = debugActionType;
	//	this->parameters[this->nowAction]->OnIsSelect();
	//}
	//else
	//{
	//	/*選択されているか調べる*/
	//	bool isSelect = false;//選択されているか
	//	int  sumDesireValue = 0;	//欲求値の合計
	//	for (int i = 0; i < this->parameters.size(); i++)
	//	{
	//		isSelect = this->parameters[i]->GetIsSelect();
	//		sumDesireValue += parameters[i]->GetDesireValue();
	//		if (isSelect)return;
	//	}
	//	
	//	/*ここですべてのパラメータの計算を行う*/
	//	bool isPriority = false;//選択されているか
	//	for (int i = 0; i < this->parameters.size(); i++)
	//	{
	//		this->parameters[i]->CalcParameter(*this);
	//		//優先フラグが立っているかも調べる
	//		if (this->parameters[i]->GetIsPriority())
	//		{
	//			isPriority = true;
	//		}
	//	}

	//	/*今立っているフラグを下す*/
	//	unsigned int clearFlag = this->actionTypeMap[this->nowAction];
	//	this->state->ClearFlag(clearFlag);

	//	/*選択されていなかったら*/
	//	if (!isSelect)
	//	{
	//		//各行動の期待値を求める
	//		std::vector<int> actionWeight;//重み
	//		int count = 0;
	//		for (int i = 0; i < this->parameters.size(); i++)
	//		{
	//			actionWeight.emplace_back(this->parameters[i]->GetWeight(sumDesireValue));
	//			//もしリストの中で選択フラグが一つでも立っていたら
	//			if (isPriority)
	//			{
	//				//現在のitemが選択フラグが立っていなかったら重みを0にする
	//				if (!this->parameters[i]->GetIsPriority())
	//				{
	//					actionWeight[count] = 0;
	//				}
	//			}
	//			count++;
	//		}
	//		//重みをランダムで出す
	//		int randomWeight = GetRand(this->parameters[0]->GetBaseWeight() - 1);
	//		//forでvectorを回し、重みが０以下になったところのアクションを行う
	//		for (int i = 0; i < actionWeight.size(); i++)
	//		{
	//			randomWeight -= actionWeight[i];
	//			if (randomWeight < 0 || this->parameters[i]->GetIsPriority())
	//			{
	//				this->nowAction = i;
	//				this->parameters[i]->OnIsSelect();
	//				isSelect = true;
	//				break;
	//			}
	//		}
	//	}
	//	if (!isSelect)
	//	{
	//		this->nowAction = static_cast<int>(ActionType::IDLE);
	//	}
	//}
	//unsigned int setFlag = this->actionTypeMap[this->nowAction];
	//this->state->SetFlag(setFlag);
}

/// <summary>
/// 描画
/// </summary>
const void Dragon::DrawCharacterInfo()const
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& shadow = Singleton<Shadow>::GetInstance();
	auto& map = Singleton<MapManager>::GetInstance();
	auto& debug = Singleton<Debug>::GetInstance();
	
	if (debug.IsShowDebugInfo(Debug::ItemType::ENEMY))
	{
		//VECTOR position = this->collider->rigidbody.GetPosition();
		//VECTOR rotation = this->collider->rigidbody.GetRotation();
		//printfDx("Dragon_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
		//printfDx("Dragon_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
		//printfDx("%d:DYING					\n", this->state->CheckFlag(this->DYING));
		//printfDx("%d:IDLE						\n", this->state->CheckFlag(this->IDLE));
		//printfDx("%d:ROAR						\n", this->state->CheckFlag(this->ROAR));
		//printfDx("%d:WALK						\n", this->state->CheckFlag(this->WALK));
		//printfDx("%d:REST						\n", this->state->CheckFlag(this->REST));
		//printfDx("%d:SLASH_1					\n", this->state->CheckFlag(this->SLASH_1));
		//printfDx("%d:SLASH_2					\n", this->state->CheckFlag(this->SLASH_2));
		//printfDx("%d:STAB						\n", this->state->CheckFlag(this->STAB));
		//printfDx("%d:ROTATE_SLASH				\n", this->state->CheckFlag(this->ROTATE_SLASH));
		//printfDx("%d:PUNCH					\n", this->state->CheckFlag(this->PUNCH));
		//printfDx("%d:SLASH_COMBO_1			\n", this->state->CheckFlag(this->SLASH_COMBO_1));
		//printfDx("%d:SLASH_COMBO_2			\n", this->state->CheckFlag(this->SLASH_COMBO_2));
		//printfDx("%d:JUMP_ATTACK				\n", this->state->CheckFlag(this->JUMP_ATTACK));
		//printfDx("%d:STATE					\n", this->angryState);
		/*各アクションの当たり判定図形の描画*/
		//this->parameters[this->nowAction]->Draw();
	}

	if (this->isDraw)
	{
		/*かげの描画*/
		shadow.Draw(map.GetStageModelHandle(), this->collider->rigidbody.GetPosition(), this->SHADOW_HEIGHT, this->SHADOW_SIZE);
	}
}

const bool Dragon::GetIsAttack()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))return true;
	return false;
}

/// <summary>
/// アニメーション再生時間の取得
/// </summary>
const float Dragon::GetAnimationPlayTime()const
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	return json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_PLAY_TIME"][this->nowAnimation];
}

/// <summary>
/// 怒り状態の設定
/// </summary>
void Dragon::SetAngryState()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	
	/*怒り状態*/
	switch (this->angryState)
	{
		//怒り
	case static_cast<int>(AngryStateType::ANGRY):
		this->angryValue--;
		if (this->angryValue < 0)
		{
			this->angryState = static_cast<int>(AngryStateType::TIRED);
			this->angryValue = 0;
		}
		break;
		//通常
	case static_cast<int>(AngryStateType::NORMAL):
		//怒り値を増加
		this->angryValue++;
		//攻撃が当たっていたら怒り値をさらに増加
		if (this->collider->data->isHit)
		{
			this->angryValue++;
		}
		//怒り値が最大以上だったら状態をANGRYにする
		if (this->angryValue >= json.GetJson(JsonManager::FileType::DRAGON)["MAX_ANGRY_VALUE"])
		{
			this->angryState = static_cast<int>(AngryStateType::ANGRY);
		}
		break;
	//疲れ
	case static_cast<int>(AngryStateType::TIRED):
		//疲れ時間を増加
		this->tiredInterval++;
		//最大値を超えたら状態を通常に変更
		if (this->tiredInterval >= json.GetJson(JsonManager::FileType::DRAGON)["MAX_TIRED_INTERVAL"])
		{
			this->angryState = static_cast<int>(AngryStateType::NORMAL);
			this->tiredInterval = 0;
		}
		break;
	}
}

/// <summary>
/// 回転率の設定
/// </summary>
void Dragon::SetRotation(const VECTOR _rotation)
{
	this->collider->rigidbody.SetRotation(_rotation);
}

/// <summary>
/// 移動ベクトルの設定
/// </summary>
void Dragon::SetVelocity(const VECTOR _velocity)
{
	this->collider->rigidbody.SetVelocity(_velocity);
}