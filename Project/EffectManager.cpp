#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "Effect.h"
#include "BossImpactEffect.h"
#include "BossRoarEffect.h"
#include "PlayerGuardHitEffect.h"
#include "PlayerHealEffect.h"
#include "PlayerImpactEffect.h"
#include "PlayerChargeAttackEffect.h"
#include "EffectManager.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "LoadingAsset.h"
#include "DeleteInstance.h"

/// <summary>
/// コンストラクタ
/// </summary>
EffectManager::EffectManager()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	this->effect.emplace_back(new BossImpactEffect		(asset.GetEffect(LoadingAsset::EffectType::BOSS_IMPACT)));
	this->effect.emplace_back(new BossRoarEffect		(asset.GetEffect(LoadingAsset::EffectType::BOSS_ROAR)));
	this->effect.emplace_back(new PlayerGuardHitEffect	(asset.GetEffect(LoadingAsset::EffectType::PLAYER_GUARD_HIT)));
	this->effect.emplace_back(new PlayerHealEffect		(asset.GetEffect(LoadingAsset::EffectType::TMP_EFFECT)));
	this->effect.emplace_back(new PlayerImpactEffect	(asset.GetEffect(LoadingAsset::EffectType::PLAYER_IMPACT)));
	this->effect.emplace_back(new PlayerChargeAttackEffect(asset.GetEffect(LoadingAsset::EffectType::PLAYER_CHARGE_ATTACK)));
}

/// <summary>
/// コンストラクタ
/// </summary>
EffectManager::~EffectManager()
{
	for (int i = 0; i < this->effect.size(); i++)
	{
		DeleteMemberInstance(this->effect[i]);
	}
	this->effect.clear();
}

/// <summary>
/// 更新
/// </summary>
void EffectManager::Initialize()
{
	for (int i = 0; i < this->effect.size(); i++)
	{
		this->effect[i]->Initialize();
	}
}

/// <summary>
/// 更新
/// </summary>
void EffectManager::Update()
{
	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
	for (int i = 0; i < this->effect.size(); i++)
	{
		this->effect[i]->Update();
	}
	//this->bossFlame->Update();
}

/// <summary>
/// 描画
/// </summary>
const void EffectManager::Draw()const
{
	for (int i = 0; i < this->effect.size(); i++)
	{
		this->effect[i]->Draw();
	}
}
void EffectManager::OnIsEffect(const EffectType _type)
{
	this->effect[static_cast<int>(_type)]->OnIsPlayEffect();
}
void EffectManager::SetPosition(const EffectType _type, const VECTOR _position)
{
	this->effect[static_cast<int>(_type)]->SetPosition(_position);
}