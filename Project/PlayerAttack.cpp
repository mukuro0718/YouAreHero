#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackData.h"
#include "AttackSphereColliderData.h"
#include "PlayerAttack.h"
#include "Debug.h"
#include "EffectManager.h"
#include "Character.h"
#include "Player.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerAttack::PlayerAttack()
	: isStartHitCheck(false)
	, frameCount(0)
	, collider(nullptr)
	, isNotOnHit(false)
{
	/*コライダーデータの作成*/
	this->collider = new AttackSphereColliderData(ColliderData::Priority::STATIC, GameObjectTag::PLAYER, new AttackData());
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerAttack::~PlayerAttack()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerAttack::Initialize()
{	
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	auto& collider	= dynamic_cast<AttackSphereColliderData&>(*this->collider);
	
	collider.radius		  = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_RADIUS"];
	collider.data->hitStopTime	  = json.GetJson(JsonManager::FileType::PLAYER)["HIT_STOP_TIME"];
	this->isStartHitCheck = false;
	this->frameCount	  = 0;
	this->isNotOnHit	  = false;
	this->collider->rigidbody.Initialize(false);
	this->collider->rigidbody.SetPosition(VGet(0.0f, 500.0f, 0.0f));
}
/// <summary>
/// 後処理
/// </summary>
void PlayerAttack::Finalize()
{
}
/// <summary>
/// 更新
/// </summary>
void PlayerAttack::Update(const VECTOR _position, const VECTOR _direction)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& collider = dynamic_cast<AttackSphereColliderData&>(*this->collider);

	/*当たり判定の確認が開始している*/
	if (this->isStartHitCheck)
	{
		//変数の準備
		const int	 START_HIT_CHECK_FRAME	= json.GetJson(JsonManager::FileType::PLAYER)["START_HIT_CHECK_FRAME"];//当たり判定開始フレーム
		const int	 END_HIT_CHECK_FRAME	= json.GetJson(JsonManager::FileType::PLAYER)["END_HIT_CHECK_FRAME"];	//当たり判定終了フレーム
		const float  POSITION_OFFSET		= json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET"];		//当たり判定座標オフセット
		const float  Y_OFFSET				= json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET_Y"];		//Y座標用オフセット
		//向きの設定
		VECTOR firstDirection = Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["FIRST_DIRECTION"]);
		VECTOR direction	  = VTransform(firstDirection,MGetRotY(player.GetRigidbody().GetRotation().y));
		//座標の設定
		VECTOR position	=  _position;											//プレイヤーの座標
		position		=  VAdd(position, VScale(direction, POSITION_OFFSET));	//プレイヤーの座標に、オフセット値を足す
		position.y		+= Y_OFFSET;											//Y座標オフセット値を足す

		//フレームを増やす
		this->frameCount++;
		//フレームが定数を超えていなかったら早期リターン
		if (this->frameCount < START_HIT_CHECK_FRAME)return;

		//今回の攻撃中に当たり判定フラグが一度もたっていなかったらフラグを立てる
		if (!this->isNotOnHit)
		{
			collider.data->isDoHitCheck = true;
			this->isNotOnHit = true;
		}
		//攻撃が当たっていたらエフェクトを再生
		if (collider.data->isHitAttack)
		{
			auto& effect = Singleton<EffectManager>::GetInstance();
			effect.OnIsEffect(EffectManager::EffectType::PLAYER_IMPACT);
			effect.SetPosition(EffectManager::EffectType::PLAYER_IMPACT, this->collider->rigidbody.GetPosition());
			collider.data->isHitAttack = false;
		}

		//当たり判定の座標のセット
		this->collider->rigidbody.SetPosition(position);

		//フレームが定数を超えている、当たり判定フラグが降りていたら当たり判定開始フラグを下す
		if (this->frameCount > END_HIT_CHECK_FRAME || (this->isNotOnHit && !collider.data->isDoHitCheck))
		{
			this->isStartHitCheck = false;
			collider.data->isDoHitCheck = false;
			this->frameCount = 0;
		}
	}
}

/// <summary>
/// ダメージの設定
/// </summary>
void PlayerAttack::SetDamage(const float _damage)
{
	auto& collider = dynamic_cast<AttackSphereColliderData&>(*this->collider);

	collider.data->damage = static_cast<int>(_damage);
}

/// <summary>
/// 描画
/// </summary>
const void PlayerAttack::Draw()const
{
	auto& debug = Singleton<Debug>::GetInstance();

	if (debug.IsShowDebugInfo(Debug::ItemType::PLAYER))
	{
		if (this->isStartHitCheck)
		{
			auto& collider = dynamic_cast<AttackSphereColliderData&>(*this->collider);
			DrawSphere3D(this->collider->rigidbody.GetPosition(), collider.radius, 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
		}
		VECTOR position = this->collider->rigidbody.GetPosition();
		printfDx("PLAYER_ATTACK X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	}
}
