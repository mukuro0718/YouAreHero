#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "Effect.h"
#include "Character.h"
#include "Player.h"
#include "BossImpactEffect.h"
#include "BossRoarEffect.h"
#include "PlayerGuardHitEffect.h"
#include "PlayerHealEffect.h"
#include "PlayerImpactEffect.h"
#include "BeastBreathEffect.h"
#include "BeastWeakBreathEffect.h"
#include "BeastBackingBreathEffect.h"
#include "BeastExplosionEffect.h"
#include "BeastSuperNovaEffect.h"
#include "BeastRaiseLevelEffect.h"
#include "BeastExplosionRangeEffect.h"
#include "DragonBreathEffect.h"
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

	this->effect.emplace_back(new BossImpactEffect			(asset.GetEffect(LoadingAsset::EffectType::PLAYER_IMPACT)));
	this->effect.emplace_back(new BossRoarEffect			(asset.GetEffect(LoadingAsset::EffectType::BOSS_ROAR)));
	this->effect.emplace_back(new PlayerGuardHitEffect		(asset.GetEffect(LoadingAsset::EffectType::PLAYER_GUARD_HIT)));
	this->effect.emplace_back(new PlayerHealEffect			(asset.GetEffect(LoadingAsset::EffectType::PLAYER_HEAL)));
	this->effect.emplace_back(new PlayerImpactEffect		(asset.GetEffect(LoadingAsset::EffectType::PLAYER_IMPACT)));
	this->effect.emplace_back(new BeastBreathEffect			(asset.GetEffect(LoadingAsset::EffectType::BEAST_PARTS_FIRE)));
	this->effect.emplace_back(new BeastBackingBreathEffect	(asset.GetEffect(LoadingAsset::EffectType::BEAST_PARTS_FIRE)));
	this->effect.emplace_back(new BeastWeakBreathEffect		(asset.GetEffect(LoadingAsset::EffectType::BEAST_PARTS_FIRE)));
	this->effect.emplace_back(new BeastExplosionEffect		(asset.GetEffect(LoadingAsset::EffectType::BEAST_EXPLOSION)));
	this->effect.emplace_back(new BeastSuperNovaEffect		(asset.GetEffect(LoadingAsset::EffectType::BEAST_SUPER_NOVA)));
	this->effect.emplace_back(new BeastRaiseLevelEffect		(asset.GetEffect(LoadingAsset::EffectType::BOSS_ROAR)));
	this->effect.emplace_back(new BeastExplosionRangeEffect (asset.GetEffect(LoadingAsset::EffectType::BEAST_EXPLOSION_RANGE)));
	this->effect.emplace_back(new DragonBreathEffect		(asset.GetEffect(LoadingAsset::EffectType::BEAST_PARTS_FIRE)));
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
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
}

/// <summary>
/// 更新
/// </summary>
void EffectManager::Update()
{
	//int startTime = GetNowCount();
	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
	for (int i = 0; i < this->effect.size(); i++)
	{
		this->effect[i]->Update();
	}
	//int endTime = GetNowCount();
	//this->frameTime = endTime - startTime;
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
	//printfDx("EFFEK_FRAMETIME:%d\n", this->frameTime);
}
void EffectManager::OnIsEffect(const EffectType _type)
{
	this->effect[static_cast<int>(_type)]->OnIsPlayEffect();
}
void EffectManager::SetPosition(const EffectType _type, const VECTOR _position)
{
	this->effect[static_cast<int>(_type)]->SetPosition(_position);
}