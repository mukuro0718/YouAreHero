#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Effect.h"
#include "BossRoarEffect.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossRoarEffect::BossRoarEffect(const int _effectResourceHandle)
	: Effect(_effectResourceHandle)
{
}

/// <summary>
/// エフェクトの再生に必要な情報の設定
/// </summary>
void BossRoarEffect::SetInfoToPlayTheEffect()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	VECTOR position = enemy.GetRigidbody().GetPosition();
	position.y += json.GetJson(JsonManager::FileType::EFFECT)["BOSS_ROAR_Y_OFFSET"];
	VECTOR scale = Gori::Convert(json.GetJson(JsonManager::FileType::EFFECT)["BOSS_ROAR_SCALE"]);
	VECTOR rotation = enemy.GetRigidbody().GetRotation();
	//rotation.y += json.GetJson(JsonManager::FileType::EFFECT)["BOSS_SLASH_ROTATE_OFFSET"] * 180.0f / DX_PI_F;
	
	this->transform->SetPosition(position);
	this->transform->SetScale(scale);
	this->transform->SetRotationOfRadian(rotation);
	this->startFrame = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_ROAR_START_FRAME"];
	this->endFrame   = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_ROAR_END_FRAME"];
}

