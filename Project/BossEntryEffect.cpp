#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Effect.h"
#include "BossEntryEffect.h"
#include "EnemyManager.h"

BossEntryEffect::BossEntryEffect(const int _effectResourceHandle)
	: Effect(_effectResourceHandle)
{
}

/// <summary>
/// エフェクトの再生に必要な情報の設定
/// </summary>
void BossEntryEffect::SetInfoToPlayTheEffect()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	//this->transform->SetPosition(enemy.GetRigidbody().GetPosition());
	this->transform->SetScale(Convert(json.GetJson(JsonManager::FileType::EFFECT)["BOSS_ENTRY_SCALE"]));
	this->transform->SetRotation(enemy.GetRigidbody().GetRotation());

	this->startFrame = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_ENTRY_START_FRAME"];
	this->endFrame = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_ENTRY_END_FRAME"];
}

