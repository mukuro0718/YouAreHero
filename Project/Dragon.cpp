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
#include "AttackCapsuleColliderData.h"
#include "AttackData.h"
#include "CharacterColliderData.h"
#include "Animation.h"
#include "Character.h"
#include "Enemy.h"
#include "Player.h"
#include "DragonBehaviorTreeHeader.h"
#include "Dragon.h"
#include "LoadingAsset.h"
#include "PlayerManager.h"
#include "CameraManager.h"
#include "EffectManager.h"
#include "Debug.h"
#include "HitStopManager.h"
#include "Shadow.h"
#include "MapManager.h"
#include "ReactionType.h"
#include "HitStop.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Dragon::Dragon()

{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json  = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*メンバクラスのインスタンスの作成*/
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::DRAGON));

	/*ツリーの作成*/
	this->tree = new DragonBehaviorTree();

	/*アニメーションの設定*/
	int			animationHandle	= json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_HANDLE"];
	vector<int>	animationIndex	= json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_INDEX"];
	//アニメーションの追加
	for (int i = 0; i < animationIndex.size(); i++)
	{
		this->animation->Add(animationHandle, animationIndex[i]);
	}
	//アニメーションのアタッチ
	this->animation->Attach(&this->modelHandle);

	/*コライダーデータの作成*/
	this->maxHp						= json.GetJson(JsonManager::FileType::DRAGON)["HP"];
	this->collider					= new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::DRAGON, new CharacterData());
	this->maxPartsColliderNum		= json.GetJson(JsonManager::FileType::DRAGON)["COLLIDER_NUM"];
	this->frameIndexUsePartsColider = json.GetJson(JsonManager::FileType::DRAGON)["FRAME_INDEX_USE_PARTS_COLLIDER"];
	for (int i = 0; i < this->maxPartsColliderNum; i++)
	{
		this->partsCollider.emplace_back(new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::DRAGON, new CharacterData()));
		this->partsCollider[i]->data->hp			= this->maxHp;
		this->partsCollider[i]->radius				= json.GetJson(JsonManager::FileType::DRAGON)["PARTS_COLL_RADIUS"][i];
		this->partsCollider[i]->isUseCollWithChara	= false;
		this->partsCollider[i]->isUseCollWithGround = false;
		this->partsCollider[i]->isSetTopPosition	= true;
		this->partsCollider[i]->rigidbody.Initialize(false);
		this->partsCollider[i]->type = CharacterColliderData::CharaType::MOL;
		this->prevPartsHp.emplace_back(this->maxHp);
	}
	//攻撃コライダー
	this->breathLength = json.GetJson(JsonManager::FileType::DRAGON)["BREATH_LENGTH"];
	vector<AttackCapsuleColliderData*> smashColider;
	vector<AttackCapsuleColliderData*> sweepColider;
	vector<AttackCapsuleColliderData*> rotateColider;
	vector<AttackCapsuleColliderData*> breathColider;
	smashColider.emplace_back (new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::DRAGON, new AttackData()));
	sweepColider.emplace_back (new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::DRAGON, new AttackData()));
	rotateColider.emplace_back(new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::DRAGON, new AttackData()));
	rotateColider.emplace_back(new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::DRAGON, new AttackData()));
	rotateColider.emplace_back(new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::DRAGON, new AttackData()));
	rotateColider.emplace_back(new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::DRAGON, new AttackData()));
	breathColider.emplace_back(new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::DRAGON, new AttackData()));
	this->attackCollider.emplace(static_cast<int>(AttackCollider::SMASH), smashColider);
	this->attackCollider.emplace(static_cast<int>(AttackCollider::SWEEP), sweepColider);
	this->attackCollider.emplace(static_cast<int>(AttackCollider::ROTATE), rotateColider);
	this->attackCollider.emplace(static_cast<int>(AttackCollider::BREATH), breathColider);
	for (int i = 0; i < this->attackCollider.size(); i++)
	{
		for (int j = 0; j < this->attackCollider[i].size(); j++)
		{
			this->attackCollider[i][j]->radius				= json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_HIT_RADIUS"][i][j];
			this->attackCollider[i][j]->data->damage		= json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_DAMAGE"][i][j];
			this->attackCollider[i][j]->data->reactionType	= static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
			this->attackCollider[i][j]->data->hitStopTime	= json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_HIT_STOP_TIME"][i][j];
			this->attackCollider[i][j]->data->hitStopType	= static_cast<int>(HitStop::Type::STOP);
			this->attackCollider[i][j]->data->hitStopDelay	= json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_HIT_STOP_DELAY"][i][j];
			this->attackCollider[i][j]->data->slowFactor	= json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_HIT_STOP_SLOW_FACTOR"][i][j];
			this->attackCollider[i][j]->data->isHitAttack	= false;
			this->attackCollider[i][j]->data->isDoHitCheck	= false;
		}
		this->isStartHitCheck[i]		= false;
		this->startHitCheckPlayTime[i]	= json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_START_HIT_CHECK_PLAY_TIME"][i];
		this->endHitCheckPlayTime[i]	= json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_END_HIT_CHECK_PLAY_TIME"][i];
		this->frameIndexUseAttackColider.emplace(i, json.GetJson(JsonManager::FileType::DRAGON)["FRAME_INDEX_USE_ATTACK_COLLIDER"][i]);
	}
	/*通常時のカラースケールを取得*/
	this->normalColor = MV1GetDifColorScale(this->modelHandle);
	this->tiredColor = this->angryColor = this->normalColor;
	this->tiredColor.b += 20;
	this->angryColor.r += 20;

}

/// <summary>
/// デストラクタ
/// </summary>
Dragon::~Dragon()
{
	Finalize();
}

/// <summary>
/// 初期化
/// </summary>
void Dragon::Initialize()
{
	/*変数の初期化*/
	//Characterクラス
	this->nextRotation	= Gori::ORIGIN;
	this->isAlive		= true;
	this->speed			= 0.0f;
	this->entryInterval = 0;
	this->isDraw		= true;
	//Enemyクラス
	this->moveTarget		= Gori::ORIGIN;
	this->animationPlayTime = 0.0f;
	this->nowAnimation		= static_cast<int>(AnimationType::ROAR);	
	this->bossState			= static_cast<int>(BossState::NORMAL);
	this->angryValue		= 0;
	this->tiredValue		= 0;
	this->attackCount		= 0;
	this->tiredDuration		= 0;
	//ビヘイビアツリーを初期化
	this->tree->Initialize();

	/*コライダーの初期化*/
	auto& json							= Singleton<JsonManager>::GetInstance();
	float height						= json.GetJson(JsonManager::FileType::DRAGON)["HIT_HEIGHT"];
	this->collider->topPositon			= VGet(0.0f, height, 0.0f);
	this->collider->radius				= json.GetJson(JsonManager::FileType::DRAGON)["HIT_RADIUS"];
	this->collider->isUseCollWithGround = true;
	this->collider->isUseCollWithChara  = false;
	this->collider->data->hp			= json.GetJson(JsonManager::FileType::DRAGON)["HP"];
	this->collider->data->isHit			= false;
	this->collider->data->damage		= 0;
	for (int i = 0; i < this->attackCollider.size(); i++)
	{
		for (int j = 0; j < this->attackCollider[i].size(); j++)
		{
			this->attackCollider[i][j]->radius = json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_HIT_RADIUS"][i][j];
			this->attackCollider[i][j]->data->damage = json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_DAMAGE"][i][j];
			this->attackCollider[i][j]->data->reactionType = static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
			this->attackCollider[i][j]->data->hitStopTime = json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_HIT_STOP_TIME"][i][j];
			this->attackCollider[i][j]->data->hitStopType = static_cast<int>(HitStop::Type::STOP);
			this->attackCollider[i][j]->data->hitStopDelay = json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_HIT_STOP_DELAY"][i][j];
			this->attackCollider[i][j]->data->slowFactor = json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_HIT_STOP_SLOW_FACTOR"][i][j];
			this->attackCollider[i][j]->data->isHitAttack = false;
			this->attackCollider[i][j]->data->isDoHitCheck = false;
		}
	}
	for (int i = 0; i < this->isStartHitCheck.size(); i++)
	{
		this->isStartHitCheck[i]		= false;
		this->startHitCheckPlayTime[i]	= json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_START_HIT_CHECK_PLAY_TIME"][i];
		this->endHitCheckPlayTime[i]	= json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_END_HIT_CHECK_PLAY_TIME"][i];
	}
	for (int i = 0; i < this->maxPartsColliderNum; i++)
	{
		this->partsCollider[i]->data->hp = this->maxHp;
		this->partsCollider[i]->radius = json.GetJson(JsonManager::FileType::DRAGON)["PARTS_COLL_RADIUS"][i];
		this->partsCollider[i]->isUseCollWithChara = true;
		this->partsCollider[i]->isUseCollWithGround = false;
		this->partsCollider[i]->isSetTopPosition = true;
		this->partsCollider[i]->rigidbody.Initialize(false);
		this->partsCollider[i]->type = CharacterColliderData::CharaType::MOL;
		this->prevPartsHp[i] = this->maxHp;
	}

	/*リジッドボディの初期化*/
	const VECTOR POSITION = Gori::Convert(json.GetJson(JsonManager::FileType::DRAGON)["INIT_POSITION"]);//座標
	const VECTOR ROTATION = Gori::Convert(json.GetJson(JsonManager::FileType::DRAGON)["INIT_ROTATION"]);//回転率
	const VECTOR SCALE = Gori::Convert(json.GetJson(JsonManager::FileType::DRAGON)["INIT_SCALE"]);	 //拡大率
	this->collider->rigidbody.Initialize(true);
	this->collider->rigidbody.SetPosition(POSITION);
	this->collider->rigidbody.SetRotation(ROTATION);
	this->collider->rigidbody.SetScale(SCALE);

	/*モデルの初期化*/
	MV1SetPosition	 (this->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->modelHandle, this->collider->rigidbody.GetScale());

	/*アニメーションの初期化*/
	this->animation->Attach(&this->modelHandle);

	ChangeNormalColor();
	SetAttackCount();
}

/// <summary>
/// 後処理
/// </summary>
void Dragon::Finalize()
{
	for (int i = 0; i < this->maxPartsColliderNum; i++)
	{
		this->partsCollider[i]->isUseCollWithChara = false;
	}
}

/// <summary>
/// 更新
/// </summary>
void Dragon::Update()
{
	/*コライダーの更新*/
	for (int i = 0; i < this->maxPartsColliderNum; i++)
	{
		//座標の設定
		VECTOR pos1 = MV1GetFramePosition(this->modelHandle, this->frameIndexUsePartsColider[i][0]);
		VECTOR pos2 = MV1GetFramePosition(this->modelHandle, this->frameIndexUsePartsColider[i][1]);
		this->partsCollider[i]->rigidbody.SetPosition(pos1);
		this->partsCollider[i]->topPositon = pos2;
		//ダメージの計算
		if (this->partsCollider[i]->data->isHit)
		{
			this->partsCollider[i]->data->isHit = false;
			int damage = this->prevPartsHp[i] - this->partsCollider[i]->data->hp;
			this->prevPartsHp[i] = this->partsCollider[i]->data->hp;
			this->collider->data->damage = damage;
			this->collider->data->hp -= damage;
			this->collider->data->isHit = true;
			this->collider->data->hitStopTime = this->partsCollider[i]->data->hitStopTime;
			this->collider->data->hitStopType = this->partsCollider[i]->data->hitStopType;
			this->collider->data->hitStopDelay = this->partsCollider[i]->data->hitStopDelay;
			this->collider->data->slowFactor = this->partsCollider[i]->data->slowFactor;
		}
	}

	UpdateBossState();

	/*ヒットストップ*/
	if (this->collider->data->isHit)
	{
		this->hitStop->SetHitStop(this->collider->data->hitStopTime, this->collider->data->hitStopType, this->collider->data->hitStopDelay, this->collider->data->slowFactor);
		this->collider->data->isHit = false;
	}
	if (this->hitStop->IsHitStop()) return;

	/*ステージ外に出たらデス*/
	if (this->collider->rigidbody.GetPosition().y < -30.0f)
	{
		RespawnIfOutOfStage();
	}

	/*ビヘイビアツリーの更新*/
	this->tree->Update(*this);

	this->positionForLockon = MV1GetFramePosition(this->modelHandle, 6);
	this->positionForLockon.y = this->collider->rigidbody.GetPosition().y;
	this->positionForLockon.y += this->LOCKON_OFFSET;

	/*HPが０以下ならほかキャラクターとの当たり判定を行わないようにする*/
}

/// <summary>
/// アニメーションの再生
/// </summary>
void Dragon::PlayAnimation(const int _playAnimation, const float _playTime)
{
	//アニメーションの再生
	if (this->isAlive)
	{
		this->animation->Play(&this->modelHandle, _playAnimation, _playTime);
	}
}

/// <summary>
/// 描画
/// </summary>
const void Dragon::DrawCharacterInfo()const
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& shadow = Singleton<Shadow>::GetInstance();
	auto& map = Singleton<MapManager>::GetInstance();

#ifdef _DEBUG
	//auto& debug = Singleton<Debug>::GetInstance();
	//auto& tree = Singleton<DragonBehaviorTree>::GetInstance();
	////tree.Draw();
	////for (int i = 0; i < this->maxPartsColliderNum; i++)
	////{
	////	VECTOR underPos = this->partsCollider[i]->rigidbody.GetPosition();
	////	VECTOR topPos = this->partsCollider[i]->topPositon;
	////	DrawCapsule3D(underPos, topPos, this->partsCollider[i]->radius, 16, GetColor(0, 255, 0), GetColor(0, 255, 0), FALSE);
	////}
	//for (const auto& pair : this->attackCollider)
	//{
	//	const std::vector<AttackCapsuleColliderData*>& colliderSet = pair.second;
	//	for (const auto& collider : colliderSet)
	//	{
	//		VECTOR underPos = collider->rigidbody.GetPosition();
	//		VECTOR topPos = collider->topPositon;
	//		DrawCapsule3D(underPos, topPos, collider->radius, 16, GetColor(0, 255, 0), GetColor(250, 0, 0), FALSE);
	//	}
	//}
#endif // _DEBUG

	if (this->isDraw)
	{
		/*かげの描画*/
		shadow.Draw(map.GetStageModelHandle(), this->collider->rigidbody.GetPosition(), this->SHADOW_HEIGHT, this->SHADOW_SIZE);
	}
}

const bool Dragon::GetIsAttack()const
{
	return false;
}

/// <summary>
/// //指定の攻撃コライダーのフラグを立てる
/// </summary>
void Dragon::OnAttackCollider(const short _index)
{
	auto& sound = Singleton<SoundManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();
	for (auto& collider:this->attackCollider[_index])
	{
		switch (_index)
		{
		case static_cast<short>(AttackCollider::SMASH):
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_SWING_1);
			collider->data->blockStaminaConsumption = json.GetJson(JsonManager::FileType::DRAGON)["SMASH_BLOCK_STAMINA_CONSUMPTION"][this->bossState];
			break;
		case static_cast<short>(AttackCollider::SWEEP):
			collider->data->blockStaminaConsumption = json.GetJson(JsonManager::FileType::DRAGON)["SWEEP_BLOCK_STAMINA_CONSUMPTION"][this->bossState];
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_SWING_2);
			break;
		case static_cast<short>(AttackCollider::ROTATE):
			collider->data->blockStaminaConsumption = json.GetJson(JsonManager::FileType::DRAGON)["ROTATE_BLOCK_STAMINA_CONSUMPTION"][this->bossState];
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_SWING_3);
			break;
		case static_cast<short>(AttackCollider::BREATH):
			collider->data->blockStaminaConsumption = json.GetJson(JsonManager::FileType::DRAGON)["BREATH_BLOCK_STAMINA_CONSUMPTION"][this->bossState];
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_BREATH);
			effect.OnIsEffect(EffectManager::EffectType::DRAGON_BREATH);
			break;
		default:
			break;
		}
		collider->data->isDoHitCheck = true;
	}
}
/// <summary>
/// 指定の攻撃コライダーを更新
/// </summary>
void Dragon::UpdateAttackCollider(const short _colliderIndex, const float _nowTotalPlayTime)
{
	/*当たり判定開始フラグが立っていないかつ、再生時間が開始の再生時間を超えていたら*/
	if (!this->isStartHitCheck[_colliderIndex] && _nowTotalPlayTime >= this->startHitCheckPlayTime[_colliderIndex])
	{
		if (_nowTotalPlayTime < this->endHitCheckPlayTime[_colliderIndex])
		{
			OnAttackCollider(_colliderIndex);
			this->isStartHitCheck[_colliderIndex] = true;
		}
	}

	/*当たり判定フラグが立っていなければ早期リターン*/
	bool isHitAttack = false;
	for (int i = 0; i < this->attackCollider[_colliderIndex].size(); i++)
	{
		auto& collider = this->attackCollider[_colliderIndex][i];
		/*攻撃が当たっていたらヒットフラグを下す*/
		if (collider->data->isHitAttack)
		{
			auto& sound = Singleton<SoundManager>::GetInstance();
			switch (_colliderIndex)
			{
			case static_cast<short>(AttackCollider::SMASH):
				sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_LIGHT_ATTACK);
				break;
			case static_cast<short>(AttackCollider::SWEEP):
				sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_LIGHT_ATTACK);
				break;
			case static_cast<short>(AttackCollider::ROTATE):
				sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_HEAVY_ATTACK);
				break;
			case static_cast<short>(AttackCollider::BREATH):
				sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_HEAVY_ATTACK);
				break;
			default:
				break;
			}
			isHitAttack = true;
		}
		if (isHitAttack)
		{
			OffAttackCollider(_colliderIndex);
			continue;
		}
		if (!this->attackCollider[_colliderIndex][i]->data->isDoHitCheck) continue;
		/*座標の更新*/
		VECTOR capsuleTopPosition = MV1GetFramePosition(this->modelHandle, this->frameIndexUseAttackColider[_colliderIndex][i][0]);
		VECTOR capsuleUnderPosition = MV1GetFramePosition(this->modelHandle, this->frameIndexUseAttackColider[_colliderIndex][i][1]);
		if (_colliderIndex == static_cast<short>(AttackCollider::BREATH))
		{
			collider->rigidbody.SetPosition(capsuleTopPosition);
			VECTOR direction = VSub(capsuleUnderPosition, capsuleTopPosition);
			direction.y = 0.0f;
			VECTOR directionScaling = VScale(direction, this->breathLength);
			collider->topPositon = VAdd(capsuleTopPosition, directionScaling);
		}
		else
		{
			collider->rigidbody.SetPosition(capsuleTopPosition);
			collider->topPositon = capsuleUnderPosition;
		}
	}

	/*再生時間が終了の再生時間を超えていたら*/
	float endHitCheckPlayTime = this->endHitCheckPlayTime[_colliderIndex];
	if (_nowTotalPlayTime >= endHitCheckPlayTime)
	{
		OffAttackCollider(_colliderIndex);
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OffIsPlayEffect(SoundManager::EffectType::MONSTER_BREATH);
		this->isStartHitCheck[_colliderIndex] = false;
	}
}
/// <summary>
/// 指定の攻撃コライダーのフラグを下す
/// </summary>
void Dragon::OffAttackCollider(const short _index)
{
	for (auto& collider : this->attackCollider[_index])
	{
		collider->data->isDoHitCheck = false;
		collider->data->isHitAttack = false;
	}
}

/// <summary>
/// 攻撃コンボ回数のセット
/// </summary>
void Dragon::SetAttackCount()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*コンボ数の設定*/
	//if (this->bossState != static_cast<int>(BossState::NORMAL))
	//{
		this->attackCount = json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_COUNT"][this->bossState];
	//}
}

/// <summary>
/// 怒り状態の設定
/// </summary>
void Dragon::UpdateBossState()
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
			ChangeNormalColor();
			auto& json = Singleton<JsonManager>::GetInstance();
			float defenisivePower = json.GetJson(JsonManager::FileType::DRAGON)["DEFENSIVE_POWER"][this->bossState];
			for (auto& collider : this->partsCollider)
			{
				collider->data->defensivePower = defenisivePower;
			}
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
			float defenisivePower = json.GetJson(JsonManager::FileType::DRAGON)["DEFENSIVE_POWER"][this->bossState];
			for (auto& collider : this->partsCollider)
			{
				collider->data->defensivePower = defenisivePower;
			}
			ChangeTiredColor();
		}
		//怒り値が最大以上だったら状態をANGRYにする
		if (this->angryValue >= json.GetJson(JsonManager::FileType::ENEMY)["MAX_ANGRY_VALUE"])
		{
			this->tree->SetInterval(static_cast<int>(DragonBehaviorTree::ActionType::ROAR),1);
			this->bossState = static_cast<int>(BossState::ANGRY);
			this->attackCount = json.GetJson(JsonManager::FileType::ENEMY)["ANGRY_ATTACK_COMBO_COUNT"];
			float defenisivePower = json.GetJson(JsonManager::FileType::DRAGON)["DEFENSIVE_POWER"][this->bossState];
			for (auto& collider : this->partsCollider)
			{
				collider->data->defensivePower = defenisivePower;
			}
			ChangeAngryColor();
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
			ChangeNormalColor();
			this->tiredDuration = 0;
			float defenisivePower = json.GetJson(JsonManager::FileType::DRAGON)["DEFENSIVE_POWER"][this->bossState];
			for (auto& collider : this->partsCollider)
			{
				collider->data->defensivePower = defenisivePower;
			}
		}
		break;
	}
}
