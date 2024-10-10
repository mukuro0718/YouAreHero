//#include <DxLib.h>
//#include "EffekseerForDXLib.h"
//#include "UseSTL.h"
//#include "UseJson.h"
//#include "Transform.h"
//#include "Rigidbody.h"
//#include "Effect.h"
//#include "BossRotatePunchEffect.h"
//#include "EnemyManager.h"
//
//BossRotatePunchEffect::BossRotatePunchEffect(const int _effectResourceHandle)
//	: Effect(_effectResourceHandle)
//{
//}
//
///// <summary>
///// エフェクトの再生に必要な情報の設定
///// </summary>
//void BossRotatePunchEffect::SetInfoToPlayTheEffect()
//{
//	/*シングルトンクラスのインスタンスの取得*/
//	auto& json = Singleton<JsonManager>::GetInstance();
//	auto& enemy = Singleton<EnemyManager>::GetInstance();
//
//	VECTOR position = enemy.GetRigidbody().GetPosition();
//	position.y += json.GetJson(JsonManager::FileType::EFFECT)["BOSS_PUNCH_Y_OFFSET"];
//	VECTOR scale = Convert(json.GetJson(JsonManager::FileType::EFFECT)["BOSS_PUNCH_SCALE"]);
//	VECTOR rotation = enemy.GetRigidbody().GetRotation();
//
//	this->transform->SetPosition(position);
//	this->transform->SetScale(scale);
//	this->transform->SetRotation(rotation);
//
//	this->startFrame = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_PUNCH_START_FRAME"];
//	this->endFrame = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_PUNCH_END_FRAME"];
//}
//
