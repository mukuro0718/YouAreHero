#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Effect.h"
#include "PlayerGuardHitEffect.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerGuardHitEffect::PlayerGuardHitEffect(const int _effectResourceHandle)
	: Effect(_effectResourceHandle)
{
}

/// <summary>
/// エフェクトの再生に必要な情報の設定
/// </summary>
void PlayerGuardHitEffect::SetInfoToPlayTheEffect()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	VECTOR position = player.GetRigidbody().GetPosition();
	position.y += json.GetJson(JsonManager::FileType::EFFECT)["PLAYER_GUARD_IMPACT_Y_OFFSET"];
	VECTOR scale = Convert(json.GetJson(JsonManager::FileType::EFFECT)["PLAYER_GUARD_IMPACT_SCALE"]);
	VECTOR rotation = player.GetRigidbody().GetRotation();

	this->transform->SetPosition(position);
	this->transform->SetScale(scale);
	this->transform->SetRotationOfRadian(rotation);
	this->startFrame = json.GetJson(JsonManager::FileType::EFFECT)["PLAYER_GUARD_IMPACT_START_FRAME"];
	this->endFrame = json.GetJson(JsonManager::FileType::EFFECT)["PLAYER_GUARD_IMPACT_END_FRAME"];
}


