#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Effect.h"
#include "BossImpactEffect.h"
#include "EnemyManager.h"
#include "PlayerManager.h"

BossImpactEffect::BossImpactEffect(const int _effectResourceHandle)
	: Effect(_effectResourceHandle)
{
}

/// <summary>
/// エフェクトの再生に必要な情報の設定
/// </summary>
void BossImpactEffect::SetInfoToPlayTheEffect()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	

	VECTOR position = player.GetRigidbody().GetPosition();
	position.y += json.GetJson(JsonManager::FileType::EFFECT)["BOSS_IMPACT_Y_OFFSET"];
	VECTOR scale = Convert(json.GetJson(JsonManager::FileType::EFFECT)["BOSS_IMPACT_SCALE"]);
	VECTOR rotation = enemy.GetRigidbody().GetRotation();

	this->transform->SetPosition(position);
	this->transform->SetScale(scale);
	this->transform->SetRotationOfRadian(rotation);
	this->startFrame = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_IMPACT_START_FRAME"];
	this->endFrame = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_IMPACT_END_FRAME"];
}

