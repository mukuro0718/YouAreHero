#include <DxLib.h>
#include "UseJson.h"
#include "BossAction.h"
#include "GoriLib.h"
#include "Boss.h"
#include "BossChase.h"
#include "PlayerManager.h"

BossChase::BossChase()
{

}

BossChase::~BossChase()
{

}

void BossChase::Initialize()
{
}

void BossChase::Finalize()
{

}

void BossChase::Update(Boss* _boss)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	/*使用する値の準備*/
	const VECTOR TARGET		  = player.GetPosition();	//目標
	const VECTOR position	  = _boss->GetPosition();	//座標
		  VECTOR rotation	  = _boss->GetRotation();	//回転率
		  VECTOR nextRotation = VGet(0.0f, 0.0f, 0.0f);

	/*回転率の設定*/
	//プレイヤーから自分の座標までのベクトルを出す
	VECTOR positonToTargetVector = VSub(position, TARGET);
	//アタン２を使って角度を求める
	nextRotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));
	//次の回転率へらーぷを使って回転する
	rotation = _boss->Lerp(rotation, nextRotation, VGet(0.1f, 0.1f, 0.1f));
	//回転率を設定
	_boss->SetRotation(rotation);

	/*移動速度の設定*/
	//このクラスは歩いているときに呼ばれるはずなので、歩きのスピードを入れる
	float speed = json.GetJson(JsonManager::FileType::ENEMY)["SPEED"];
	//スピードの設定
	_boss->SetSpeed(speed);

	/*移動量を出す*/
	VECTOR direction = VGet(0.0f, 0.0f, 0.0f);
	//回転率をもとに移動方向を出す
	direction = VGet(-sinf(rotation.y), 0.0f, -cosf(rotation.y));
	//向きベクトルを正規化
	direction = VNorm(direction);
	//重力を考慮するため、Yのみ前の移動ベクトルを参照する
	VECTOR aimVelocity = VScale(direction, speed);
	VECTOR prevVelocity = _boss->GetVelocity();
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);
	//移動ベクトルの設定
	_boss->SetVelocity(newVelocity);
}