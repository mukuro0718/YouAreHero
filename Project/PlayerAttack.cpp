#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackData.h"
#include "PlayerAttackData.h"
#include "AttackSphereColliderData.h"
#include "PlayerAttack.h"
#include "Debug.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerAttack::PlayerAttack()
	: isStartHitCheck(false)
	, frameCount(0)
	, collider(nullptr)
	, isNotOnHit(false)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*コライダーデータの作成*/
	AttackData* data = new PlayerAttackData();
	this->collider = new AttackSphereColliderData(ColliderData::Priority::STATIC, GameObjectTag::PLAYER_ATTACK, data);
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

	/*コライダーの初期化*/
	auto& collider = dynamic_cast<AttackSphereColliderData&>(*this->collider);
	auto& data = dynamic_cast<PlayerAttackData&>(*collider.data);
	collider.radius = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_RADIUS"];
	data.damage = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_DAMAGE"];
	data.hitStopTime = json.GetJson(JsonManager::FileType::PLAYER)["HIT_STOP_TIME"];

	/*物理挙動の初期化*/
	this->collider->rigidbody.Initialize(false);
	this->collider->rigidbody.SetPosition(VGet(0.0f, 500.0f, 0.0f));
	this->isStartHitCheck = false;
	this->frameCount = 0;
	this->isNotOnHit = false;
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
	//auto& player = Singleton<PlayerManager>::GetInstance();
	auto& collider = dynamic_cast<AttackSphereColliderData&>(*this->collider);
	auto& data = dynamic_cast<PlayerAttackData&>(*collider.data);

	/*当たり判定の確認が開始している*/
	if (this->isStartHitCheck)
	{
		//変数の準備
		const int	 START_HIT_CHECK_FRAME	= json.GetJson(JsonManager::FileType::PLAYER)["START_HIT_CHECK_FRAME"];//当たり判定開始フレーム
		const int	 END_HIT_CHECK_FRAME	= json.GetJson(JsonManager::FileType::PLAYER)["END_HIT_CHECK_FRAME"];	//当たり判定終了フレーム
		const float  POSITION_OFFSET		= json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET"];		//当たり判定座標オフセット
		const float  Y_OFFSET				= json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET_Y"];		//Y座標用オフセット
		const VECTOR DIRECTION				= _direction;																//プレイヤーの向き
		VECTOR		position				= _position;										//プレイヤーの座標
		position							= VAdd(position, VScale(DIRECTION, POSITION_OFFSET));			//プレイヤーの座標に、オフセット値を足す
		position.y						   += Y_OFFSET;																	//Y座標オフセット値を足す

		//フレームを増やす
		this->frameCount++;
		//フレームが定数を超えていなかったら早期リターン
		if (this->frameCount < START_HIT_CHECK_FRAME)return;

		//今回の攻撃中に当たり判定フラグが一度もたっていなかったら
		if (!this->isNotOnHit)
		{
			data.isDoHitCheck = true;
			this->isNotOnHit = true;
		}

		//当たり判定の座標のセット
		this->collider->rigidbody.SetPosition(position);
		//フレームが定数を超えている、当たり判定フラグが降りていたら当たり判定開始フラグを下す
		if (this->frameCount > END_HIT_CHECK_FRAME || (this->isNotOnHit && !data.isDoHitCheck))
		{
			this->isStartHitCheck = false;
			data.isDoHitCheck = false;
			this->frameCount = 0;
		}
	}
	//else
	//{
	//	//当たり判定の座標のセット
	//	this->collider->rigidbody.SetPosition(VGet(0.0f, 500.0f, 0.0f));
	//}
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
