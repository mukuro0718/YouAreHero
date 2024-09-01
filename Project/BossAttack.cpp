#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
#include "BossAttack.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossAttack::BossAttack(const int _attackNum)
	: Collidable(Collidable::Priority::STATIC, GameObjectTag::BOSS_ATTACK, GoriLib::ColliderData::Kind::SPHERE, false)
	, isStartHitCheck(false)
	, frameCount(0)
	, damage(0)
	, radius(0.0f)
	, basePosition(VGet(0.0f, 0.0f, 0.0f))
	, direction(VGet(0.0f, 0.0f, 0.0f))
	, isDontStartPrevFrame(false)
	, attackNum(_attackNum)
	, stackSpeed(0.0f)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->radius = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_RADIUS"][this->attackNum];
	/*コライダーデータの作成*/
	auto sphereColiderData = dynamic_cast<GoriLib::ColliderDataSphere*>(this->colliderData);
	sphereColiderData->radius = this->radius;
	sphereColiderData->damage = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_DAMAGE"][this->attackNum];
	sphereColiderData->hitNumber = 0;

}

/// <summary>
/// デストラクタ
/// </summary>
BossAttack::~BossAttack()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossAttack::Initialize(GoriLib::Physics* _physics, const float _radius)
{
	/*コライダーの初期化*/
	Collidable::Initialize(_physics);

	/*物理挙動の初期化*/
	this->rigidbody.Initialize(false);
	this->rigidbody.SetPosition(VGet(0.0f, 500.0f, 0.0f));
	this->isStartHitCheck = false;
	this->frameCount = 0;
	this->basePosition = VGet(0.0f, 500.0f, 0.0f);
	this->direction = VGet(0.0f, 0.0f, -1.0f);
	this->isDontStartPrevFrame = false;
	auto sphereColiderData = dynamic_cast<GoriLib::ColliderDataSphere*>(this->colliderData);
	sphereColiderData->radius = _radius;

}
/// <summary>
/// 後処理
/// </summary>
void BossAttack::Finalize(GoriLib::Physics* _physics)
{
	/*物理登録の解除*/
	Collidable::Finalize(_physics);
}
/// <summary>
/// 更新
/// </summary>
void BossAttack::Update(GoriLib::Physics* _physics, const VECTOR _position, const VECTOR _direction, const bool _isMove, const float _speed)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*当たり判定の確認が開始している*/
	if (this->isStartHitCheck)
	{

		if (!this->isDontStartPrevFrame)
		{
			auto sphereColiderData = dynamic_cast<GoriLib::ColliderDataSphere*>(this->colliderData);
			sphereColiderData->hitNumber = enemy.GetHitNumber();
			this->isDontStartPrevFrame = true;
			this->stackSpeed = 0.0f;
		}
		//変数の準備
		const int START_HIT_CHECK_FRAME = json.GetJson(JsonManager::FileType::ENEMY)["START_HIT_CHECK_FRAME"][this->attackNum];
		const int END_HIT_CHECK_FRAME = json.GetJson(JsonManager::FileType::ENEMY)["END_HIT_CHECK_FRAME"][this->attackNum];
		const float POSITION_OFFSET = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET"][this->attackNum];
		const float Y_OFFSET = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET_Y"][this->attackNum];

		//フレームを増やす
		this->frameCount++;
		//フレームが定数を超えていなかったら早期リターン
		if (this->frameCount < START_HIT_CHECK_FRAME)return;

		VECTOR position;
		//当たり判定の座標のセット
		if (_isMove)
		{
			position = VScale(this->direction, POSITION_OFFSET);
			position.y += Y_OFFSET;
			position = VAdd(this->basePosition, position);
			this->stackSpeed += _speed;
			position = VAdd(position, VScale(this->direction, this->stackSpeed));
		}
		else
		{
			position = _position;
		}

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
		this->rigidbody.SetPosition(VGet(0.0f, 500.0f, 0.0f));
		this->basePosition = _position;
		this->direction = _direction;
		this->isDontStartPrevFrame = false;
	}
}
/// <summary>
/// 衝突したか
/// </summary>
void BossAttack::OnCollide(const Collidable& _colider)
{
	//std::string message = "プレイヤーの攻撃が";
	//if (_colider.GetTag() == GameObjectTag::BOSS)
	//{
	//	message += "ボス";
	//}

	//message += "と当たった\n";
	//printfDx(message.c_str());

}

/// <summary>
/// 描画
/// </summary>
const void BossAttack::Draw()const
{
#if _DEBUG
	if (this->isStartHitCheck)
	{
		DrawSphere3D(this->rigidbody.GetPosition(), this->radius, 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
	}
	VECTOR position = rigidbody.GetPosition();
	printfDx("MAIN_1_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
#endif // _DEBUG
}

const VECTOR BossAttack::GetPosition()const
{
	return this->rigidbody.GetPosition();
}