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
	this->collider					= new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BOSS, new CharacterData());
	this->maxPartsColliderNum		= json.GetJson(JsonManager::FileType::DRAGON)["COLLIDER_NUM"];
	this->frameIndexUsePartsColider = json.GetJson(JsonManager::FileType::DRAGON)["FRAME_INDEX_USE_PARTS_COLLIDER"];
	for (int i = 0; i < this->maxPartsColliderNum; i++)
	{
		this->partsCollider.emplace_back(new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BOSS, new CharacterData()));
		this->partsCollider[i]->data->hp			= this->maxHp;
		this->partsCollider[i]->radius				= json.GetJson(JsonManager::FileType::DRAGON)["PARTS_COLL_RADIUS"][i];
		this->partsCollider[i]->isUseCollWithChara	= true;
		this->partsCollider[i]->isUseCollWithGround = false;
		this->partsCollider[i]->isSetTopPosition	= true;
		this->partsCollider[i]->rigidbody.Initialize(false);
		this->prevPartsHp.emplace_back(this->maxHp);
	}
	this->frameIndexUseAttackColider = json.GetJson(JsonManager::FileType::DRAGON)["FRAME_INDEX_USE_ATTACK_COLLIDER"];
	this->breathLength = json.GetJson(JsonManager::FileType::DRAGON)["BREATH_LENGTH"];
	short maxAttackColliderNum = json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_COLLIDER_NUM"];
	for (int i = 0; i < maxAttackColliderNum; i++)
	{
		this->attackCollider.emplace_back(new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BOSS_ATTACK, new AttackData()));
		this->attackCollider[i]->radius				= json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_HIT_RADIUS"][i];
		this->attackCollider[i]->data->damage		= json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_DAMAGE"][i];
		this->attackCollider[i]->data->reactionType = static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
		this->attackCollider[i]->data->hitStopTime	= json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_HIT_STOP_TIME"][i];
		this->attackCollider[i]->data->hitStopType	= static_cast<int>(HitStop::Type::STOP);
		this->attackCollider[i]->data->hitStopDelay = json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_HIT_STOP_DELAY"][i];
		this->attackCollider[i]->data->slowFactor	= json.GetJson(JsonManager::FileType::DRAGON)["ATTACK_HIT_STOP_SLOW_FACTOR"][i];
		this->attackCollider[i]->data->isHitAttack	= false;
		this->attackCollider[i]->data->isDoHitCheck = false;
	}

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
	/*変数の初期化*/
	auto& json = Singleton<JsonManager>::GetInstance();
	this->isAlive		= true;
	this->isGround		= true;
	this->isDraw		= true;
	this->speed			= 0.0f;
	this->entryInterval	= 0;
	
	/*コライダーの初期化*/
	const VECTOR POSITION = Gori::Convert(json.GetJson(JsonManager::FileType::DRAGON)["INIT_POSITION"]);//座標
	const VECTOR ROTATION = Gori::Convert(json.GetJson(JsonManager::FileType::DRAGON)["INIT_ROTATION"]);//回転率
	const VECTOR SCALE = Gori::Convert(json.GetJson(JsonManager::FileType::DRAGON)["INIT_SCALE"]);	 //拡大率
	float height						= json.GetJson(JsonManager::FileType::DRAGON)["HIT_HEIGHT"];
	this->collider->topPositon			= VGet(0.0f, height, 0.0f);
	this->collider->radius				= json.GetJson(JsonManager::FileType::DRAGON)["HIT_RADIUS"];
	this->collider->isUseCollWithGround = true;
	this->collider->isUseCollWithChara  = false;
	this->collider->data->hp			= json.GetJson(JsonManager::FileType::DRAGON)["HP"];
	this->collider->data->isHit			= false;
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
			float damage = this->prevPartsHp[i] - this->partsCollider[i]->data->hp;
			this->prevPartsHp[i] = this->partsCollider[i]->data->hp;
			this->collider->data->hp -= damage;
		}
	}

	/*ステージ外に出たらデス*/
	if (this->collider->rigidbody.GetPosition().y < -30.0f)
	{
		DyingIfOutOfStage();
	}

	/*ビヘイビアツリーの更新*/
	auto& tree = Singleton<DragonBehaviorTree>::GetInstance();
	tree.Update();
}

/// <summary>
/// アニメーションの再生
/// </summary>
void Dragon::PlayAnimation(const float _playAnimation, const float _playTime)
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
	auto& debug = Singleton<Debug>::GetInstance();
	auto& tree = Singleton<DragonBehaviorTree>::GetInstance();
	tree.Draw();
	for (int i = 0; i < this->maxPartsColliderNum; i++)
	{
		//VECTOR pos1 = this->pos1[i];
		//VECTOR pos2 = this->pos2[i];
		//DrawCapsule3D(pos1, pos2, this->partsCollider[i]->radius, 16, GetColor(0, 0, 255), GetColor(0, 0, 255), FALSE);
		VECTOR underPos = this->partsCollider[i]->rigidbody.GetPosition();
		VECTOR topPos = this->partsCollider[i]->topPositon;
		DrawCapsule3D(underPos, topPos, this->partsCollider[i]->radius, 16, GetColor(0, 255, 0), GetColor(0, 255, 0), FALSE);
	}
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
	this->attackCollider[_index]->data->isDoHitCheck = true;
}
/// <summary>
/// 指定の攻撃コライダーを更新
/// </summary>
void Dragon::UpdateAttackCollider(const short _index)
{
	/*座標の更新*/
	VECTOR capsuleTopPosition = MV1GetFramePosition(this->modelHandle, this->frameIndexUseAttackColider[_index][0]);
	VECTOR capsuleUnderPosition = MV1GetFramePosition(this->modelHandle, this->frameIndexUseAttackColider[_index][1]);
	if (_index == static_cast<short>(AttackCollider::BREATH))
	{
		this->collider->rigidbody.SetPosition(capsuleTopPosition);
		VECTOR direction = VSub(capsuleUnderPosition, capsuleTopPosition);
		direction.y = 0.0f;
		VECTOR directionScaling = VScale(direction, this->breathLength);
		this->collider->topPositon = VAdd(capsuleTopPosition, directionScaling);
	}
	else
	{
		this->collider->rigidbody.SetPosition(capsuleTopPosition);
		this->collider->topPositon = capsuleUnderPosition;
	}

	/*攻撃が当たっていたらヒットフラグを下す*/
	if (this->attackCollider[_index]->data->isHitAttack)
	{
		this->attackCollider[_index]->data->isHitAttack = false;
	}
}
/// <summary>
/// 指定の攻撃コライダーのフラグを下す
/// </summary>
void Dragon::OffAttackCollider(const short _index)
{
	this->attackCollider[_index]->data->isDoHitCheck = false;
	this->attackCollider[_index]->data->isHitAttack = false;
}

