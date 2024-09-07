#include <cassert>
#include <DxLib.h>
#include "UseJson.h"
#include "DeleteInstance.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
using namespace GoriLib;
#include "BossAttack.h"
#include "BossFlyAttack.h"
#include "EnemyManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
BossFlyAttack::BossFlyAttack(const int _attackIndex)
	: BossAttack(Collidable::Priority::STATIC, GameObjectTag::BOSS_ATTACK, GoriLib::ColliderData::Kind::SPHERE, false)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*この攻撃がボスの何番目の攻撃かを保存しておく*/
	this->attackIndex = _attackIndex;

	/*コライダーデータの作成*/
	auto sphereColiderData		 = dynamic_cast<GoriLib::ColliderDataSphere*>(this->colliderData);
	sphereColiderData->radius	 = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_RADIUS"][this->attackIndex];
	sphereColiderData->damage	 = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_DAMAGE"][this->attackIndex];
	sphereColiderData->hitNumber = 0;

}

/// <summary>
/// デストラクタ
/// </summary>
BossFlyAttack::~BossFlyAttack()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossFlyAttack::Initialize(GoriLib::Physics* _physics)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*コライダーの初期化*/
	Collidable::Initialize(_physics);

	/*変数の初期化*/
	this->frameCount		   = 0;
	this->stackSpeed		   = 0.0f;
	this->isStartHitCheck	   = false;
	this->basePosition		   = Convert(json.GetJson(JsonManager::FileType::ENEMY)["OUT_POSITION"]);
	this->direction			   = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_DIRECTION"]);
	this->isStartHitCheck	   = false;
	this->isDontStartPrevFrame = false;

	/*物理挙動の初期化*/
	this->rigidbody.Initialize(false);
	this->rigidbody.SetPosition(this->basePosition);

	/*コライダーデータの初期化*/
	auto sphereColiderData = dynamic_cast<GoriLib::ColliderDataSphere*>(this->colliderData);
	sphereColiderData->radius = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_RADIUS"][this->attackIndex];
	sphereColiderData->damage = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_DAMAGE"][this->attackIndex];

}
/// <summary>
/// 更新
/// </summary>
void BossFlyAttack::Update(GoriLib::Physics* _physics)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*当たり判定の確認が開始している*/
	if (this->isStartHitCheck)
	{
		//前のフレームで当たり判定を行っていなかったら
		if (!this->isDontStartPrevFrame)
		{
			auto sphereColiderData		 = dynamic_cast<GoriLib::ColliderDataSphere*>(this->colliderData);
			sphereColiderData->hitNumber = enemy.GetHitNumber();
			this->isDontStartPrevFrame	 = true;
			this->stackSpeed			 = 0.0f;
		}
		//変数の準備
		const int	START_HIT_CHECK_FRAME = json.GetJson(JsonManager::FileType::ENEMY)["START_HIT_CHECK_FRAME"]	[this->attackIndex];
		const int	END_HIT_CHECK_FRAME	  = json.GetJson(JsonManager::FileType::ENEMY)["END_HIT_CHECK_FRAME"]	[this->attackIndex];
		const float POSITION_OFFSET		  = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET"]			[this->attackIndex];
		const float Y_OFFSET			  = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET_Y"]		[this->attackIndex];

		//フレームを増やす
		this->frameCount++;
		//フレームが定数を超えていなかったら早期リターン
		if (this->frameCount < START_HIT_CHECK_FRAME)return;
		//当たり判定位置の更新
		VECTOR position = MV1GetFramePosition(enemy.GetModelHandle(), 6);
		this->rigidbody.SetPosition(position);
		//フレームが定数を超えていたら当たり判定開始フラグを下す
		if (this->frameCount > END_HIT_CHECK_FRAME)
		{
			this->isStartHitCheck = false;
			this->frameCount = 0;
		}
	}
	else
	{
		//当たり判定の座標のセット
		this->isDontStartPrevFrame = false;
		this->direction			   = enemy.GetDirection();
		this->basePosition		   = Convert(json.GetJson(JsonManager::FileType::ENEMY)["OUT_POSITION"]);
		this->rigidbody.SetPosition(this->basePosition);
	}
}

/// <summary>
/// 描画
/// </summary>
const void BossFlyAttack::Draw()const
{
#if _DEBUG
	if (this->isStartHitCheck)
	{
		DrawSphere3D(this->rigidbody.GetPosition(), this->GetRadius(), 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
	}
	VECTOR position = rigidbody.GetPosition();
	printfDx("MAIN_1_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
#endif // _DEBUG
}


const float BossFlyAttack::GetRadius()const
{
	auto sphereColiderData = dynamic_cast<GoriLib::ColliderDataSphere*>(this->colliderData);
	return sphereColiderData->radius;
}

void BossFlyAttack::SetRadius(const float _radius)
{
	auto sphereColiderData = dynamic_cast<GoriLib::ColliderDataSphere*>(this->colliderData);
	sphereColiderData->radius = _radius;
}