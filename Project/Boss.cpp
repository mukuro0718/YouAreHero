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
#include "HitStop.h"
#include "LoadingAsset.h"
#include "PlayerManager.h"
#include "CameraManager.h"
#include "EffectManager.h"
#include "Debug.h"
#include "Shadow.h"
#include "MapManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Boss::Boss()
	: Enemy()
	, prevAttack(AttackType::NONE)
	, nowAction				(0)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json  = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*メンバクラスのインスタンスの作成*/
	this->modelHandle	= MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::ENEMY));
	this->normalTexture = asset.GetImage(LoadingAsset::ImageType::BOSS_NORMAL_TEXTURE);
	this->angryTexture  = asset.GetImage(LoadingAsset::ImageType::BOSS_ANGRY_TEXTURE);
	this->tiredTexture  = asset.GetImage(LoadingAsset::ImageType::BOSS_TIRED_TEXTURE);
	/*アニメーションの設定*/
	vector<int>	animationHandle	  = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_HANDLE"];
	vector<int>		animationIndex	  = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_INDEX"];
			  this->nowAnimation	  = static_cast<int>(AnimationType::IDLE);
			  this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	//アニメーションの追加
	for (int i = 0; i < animationHandle.size(); i++)
	{
		this->animation->Add(animationHandle[i], animationIndex[i]);
	}
	//アニメーションのアタッチ
	this->animation->Attach(&this->modelHandle);

	/*アクションマップの作成*/
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
	this->actionTypeMap.emplace(static_cast<int>(ActionType::SLASH_COMBO_2) ,this->SLASH_COMBO_2);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::JUMP_ATTACK)	,this->JUMP_ATTACK);

	/*コライダーデータの作成*/
	this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BOSS, new CharacterData());
	this->collider->type = CharacterColliderData::CharaType::GORG;
	/*アクションパラメーターの作成*/
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
/// デストラクタ
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
/// 初期化
/// </summary>
void Boss::Initialize()
{
	/*変数の初期化*/
	//Characterクラス
	this->nextRotation = Gori::ORIGIN;
	this->isAlive		= true;
	this->speed			= 0.0f;
	this->entryInterval	= 0;
	this->isDraw		= true;
	//Enemyクラス
	this->moveTarget		= Gori::ORIGIN;
	this->animationPlayTime	= 0.0f;
	this->nowAnimation		= static_cast<int>(AnimationType::ROAR);
	//Bossクラス
	for (int i = 0; i < this->parameters.size(); i++)
	{
		this->parameters[i]->Initialize();
	}
	this->prevAttack		= AttackType::NONE;
	this->angryValue		= 0;
	this->nowAction			= static_cast<int>(ActionType::ROAR);
	this->bossState			= static_cast<int>(BossState::NORMAL);
	this->tiredValue		= 0;
	this->tiredDuration		= 0;
	this->attackCount		= 0;

	/*コンボの設定*/
	SetAttackCount();

	/*コライダーの初期化*/
	auto& json							 = Singleton<JsonManager>::GetInstance();
	float height						 = json.GetJson(JsonManager::FileType::ENEMY)["HIT_HEIGHT"];
	this->collider->topPositon			 = VGet(0.0f, height, 0.0f);
	this->collider->radius				 = json.GetJson(JsonManager::FileType::ENEMY)["HIT_RADIUS"];
	this->collider->isUseCollWithGround	 = true;
	this->collider->isUseCollWithChara	 = true;
	this->collider->data->defensivePower = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DEFENSIVE_POWER"];
	this->collider->data->hp			 = json.GetJson(JsonManager::FileType::ENEMY)["HP"];
	this->collider->data->isHit			 = false;

	/*コライダー内のリジッドボディの初期化*/
	const VECTOR POSITION = Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_POSITION"]);//座標
	const VECTOR ROTATION = Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_ROTATION"]);//回転率
	const VECTOR SCALE = Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_SCALE"]);	 //拡大率
	this->collider->rigidbody.Initialize(true);
	this->collider->rigidbody.SetPosition(POSITION);
	this->collider->rigidbody.SetRotation(ROTATION);
	this->collider->rigidbody.SetScale(SCALE);
	MV1SetPosition	 (this->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->modelHandle, this->collider->rigidbody.GetScale());

	/*状態の初期化*/
	this->state->ClearFlag(this->MASK_ALL);
	this->state->SetFlag(this->ROAR);

	/*アニメーションのアタッチ*/
	this->animation->Attach(&this->modelHandle);

	/*使用するテクスチャを通常に戻す*/
	MV1SetTextureGraphHandle(this->modelHandle, 0, this->normalTexture, FALSE);
}

/// <summary>
/// 後処理
/// </summary>
void Boss::Finalize()
{
	this->collider->isUseCollWithGround = false;
}

/// <summary>
/// 更新
/// </summary>
void Boss::Update()
{
	//int startTime = GetNowCount();
	
	/*ステージ外に出たらデス*/
	if (this->collider->rigidbody.GetPosition().y < -30.0f)
	{
		RespawnIfOutOfStage();
	}

	/*怒り状態の設定*/
	UpdateBossState();

	/*ヒットストップ*/
	if (this->collider->data->isHit)
	{
		this->hitStop->SetHitStop(this->collider->data->hitStopTime, this->collider->data->hitStopType, this->collider->data->hitStopDelay, this->collider->data->slowFactor);
		this->collider->data->isHit = false;
	}
	if (this->hitStop->IsHitStop()) return;

	/*状態の切り替え*/
	ChangeState();

	/*ここに各アクションごとの更新処理を入れたい*/
	this->parameters[this->nowAction]->Update(*this);

	this->positionForLockon = this->collider->rigidbody.GetPosition();
	this->positionForLockon.y += this->LOCKON_OFFSET;
	//int endTime = GetNowCount();
	//this->frameTime = endTime - startTime;
}

/// <summary>
/// アニメーションの再生
/// </summary>
void Boss::PlayAnimation()
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
void Boss::SetAttackCount()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*コンボ数の設定*/
	if (this->bossState == static_cast<int>(BossState::NORMAL))
	{
		this->attackCount = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_COMBO_COUNT"][this->bossState];
	}
}

/// <summary>
/// 状態の変更
/// </summary>
void Boss::ChangeState()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& debug = Singleton<Debug>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	int debugActionType = json.GetJson(JsonManager::FileType::DEBUG)["BOSS_ACTION"];
	if (debug.IsShowDebugInfo(Debug::ItemType::ENEMY) && debugActionType != static_cast<int>(Boss::ActionType::NONE))
	{
		/*今立っているフラグを下す*/
		unsigned int clearFlag = this->actionTypeMap[this->nowAction];
		this->state->ClearFlag(clearFlag);
		this->nowAction = debugActionType;
		this->parameters[this->nowAction]->OnIsSelect();
	}
	else
	{
		/*選択されているか調べる*/
		bool isSelect = false;//選択されているか
		int  sumDesireValue = 0;	//欲求値の合計
		for (int i = 0; i < this->parameters.size(); i++)
		{
			if (i == this->nowAction)
			{
				isSelect = this->parameters[i]->GetIsSelect();
				if (isSelect)return;
			}
			sumDesireValue += parameters[i]->GetDesireValue();
		}
		
		/*ここですべてのパラメータの計算を行う*/
		bool isPriority = false;//選択されているか
		for (int i = 0; i < this->parameters.size(); i++)
		{
			this->parameters[i]->CalcParameter(*this);
			//優先フラグが立っているかも調べる
			if (this->parameters[i]->GetIsPriority())
			{
				isPriority = true;
			}
		}

		/*今立っているフラグを下す*/
		unsigned int clearFlag = this->actionTypeMap[this->nowAction];
		this->state->ClearFlag(clearFlag);

		/*選択されていなかったら*/
		if (!isSelect)
		{
			//各行動の期待値を求める
			std::vector<int> actionWeight;//重み
			int count = 0;
			for (int i = 0; i < this->parameters.size(); i++)
			{
				actionWeight.emplace_back(this->parameters[i]->GetWeight(sumDesireValue));
				//もしリストの中で選択フラグが一つでも立っていたら
				if (isPriority)
				{
					//現在のitemが選択フラグが立っていなかったら重みを0にする
					if (!this->parameters[i]->GetIsPriority())
					{
						actionWeight[count] = 0;
					}
				}
				count++;
			}
			//重みをランダムで出す
			int randomWeight = GetRand(this->parameters[0]->GetBaseWeight() - 1);
			//forでvectorを回し、重みが０以下になったところのアクションを行う
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
/// 描画
/// </summary>
const void Boss::DrawCharacterInfo()const
{
	/*シングルトンクラスのインスタンスを取得*/
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
	//	/*各アクションの当たり判定図形の描画*/
	//	this->parameters[this->nowAction]->Draw();
	//}

#endif // _DEBUG
	if (this->isDraw)
	{
		/*かげの描画*/
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
/// 怒り状態の設定
/// </summary>
void Boss::UpdateBossState()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	
	/*状態の切り替え*/
	switch (this->bossState)
	{
		//怒り
	case static_cast<int>(BossState::ANGRY):
		//攻撃回数が０だったら状態を通常に戻す
		if (this->attackCount <= 0)
		{
			this->bossState = static_cast<int>(BossState::NORMAL);
			this->angryValue = 0;
			MV1SetTextureGraphHandle(this->modelHandle, 0, this->normalTexture, FALSE);
			this->collider->data->defensivePower = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DEFENSIVE_POWER"];
		}
		break;
		//通常
	case static_cast<int>(BossState::NORMAL):
		//攻撃が当たっていたら各状態変化用の値を増加させる
		if (this->collider->data->isHit)
		{
			this->angryValue += this->collider->data->damage;
			this->tiredValue += this->collider->data->damage;
		}
		//疲れゲージが最大以上だったら状態をTIREDにする
		if (this->tiredValue >= json.GetJson(JsonManager::FileType::ENEMY)["MAX_TIRED_VALUE"])
		{
			this->bossState = static_cast<int>(BossState::TIRED);
			this->attackCount = 0;
			this->tiredValue = 0;
			this->collider->data->defensivePower = json.GetJson(JsonManager::FileType::ENEMY)["TIRED_DEFENSIVE_POWER"];
			MV1SetTextureGraphHandle(this->modelHandle, 0, this->tiredTexture, FALSE);
		}
		//怒り値が最大以上だったら状態をANGRYにする
		if (this->angryValue >= json.GetJson(JsonManager::FileType::ENEMY)["MAX_ANGRY_VALUE"])
		{
			this->bossState = static_cast<int>(BossState::ANGRY);
			this->attackCount = json.GetJson(JsonManager::FileType::ENEMY)["ANGRY_ATTACK_COMBO_COUNT"];
			this->collider->data->defensivePower = json.GetJson(JsonManager::FileType::ENEMY)["ANGRY_DEFENSIVE_POWER"];
			MV1SetTextureGraphHandle(this->modelHandle, 0, this->angryTexture, FALSE);
		}
		break;
	//疲れ
	case static_cast<int>(BossState::TIRED):
		//疲れ時間を増加
		this->tiredDuration++;
		//最大値を超えたら状態を通常に変更
		if (this->tiredDuration >= json.GetJson(JsonManager::FileType::ENEMY)["TIRED_DURATION"])
		{
			this->bossState = static_cast<int>(BossState::NORMAL);
			MV1SetTextureGraphHandle(this->modelHandle, 0, this->normalTexture, FALSE);
			this->tiredDuration = 0;
			this->collider->data->defensivePower = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DEFENSIVE_POWER"];
		}
		break;
	}
}

