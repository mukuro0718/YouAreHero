#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "Effect.h"
#include "BossImpactEffect.h"
#include "BossFlameEffect.h"
#include "PlayerGuardHitEffect.h"
#include "PlayerHealEffect.h"
#include "PlayerImpactEffect.h"
#include "PlayerJustGuardEffect.h"
#include "EffectManager.h"
#include "EnemyManager.h"
#include "BossAttackManager.h"
#include "PlayerManager.h"
#include "LoadingAsset.h"

/// <summary>
/// コンストラクタ
/// </summary>
EffectManager::EffectManager()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	this->effect.emplace_back(new BossImpactEffect		(asset.GetEffect(LoadingAsset::EffectType::BOSS_IMPACT)));
	this->effect.emplace_back(new PlayerGuardHitEffect	(asset.GetEffect(LoadingAsset::EffectType::PLAYER_GUARD_HIT)));
	this->effect.emplace_back(new PlayerHealEffect		(asset.GetEffect(LoadingAsset::EffectType::PLAYER_HEAL)));
	this->effect.emplace_back(new PlayerImpactEffect	(asset.GetEffect(LoadingAsset::EffectType::PLAYER_IMPACT)));
	this->effect.emplace_back(new PlayerJustGuardEffect	(asset.GetEffect(LoadingAsset::EffectType::PLAYER_JUST_GUARD)));

	int bossFlameEffectNum = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_FLAME_EFFECT_NUM"];
	this->bossFlame = new BossFlameEffect(asset.GetEffect(LoadingAsset::EffectType::BOSS_FLAME));
}

/// <summary>
/// コンストラクタ
/// </summary>
EffectManager::~EffectManager()
{

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
	this->bossFlame->Initialize();
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
	//this->bossFlame->Draw();
}
void EffectManager::OnIsEffect(const EffectType _type)
{
	if (_type == EffectType::BOSS_FLAME)
	{
		//this->bossFlame->OnIsPlayEffect();
	}
	else
	{
		this->effect[static_cast<int>(_type)]->OnIsPlayEffect();
	}
}
void EffectManager::SetPosition(const EffectType _type, const VECTOR _position)
{
	this->effect[static_cast<int>(_type)]->SetPosition(_position);
}
/// <summary>
/// vector<float>をVECTORに変換
/// </summary>
const VECTOR EffectManager::Convert(std::vector<float> _in)const
{
	VECTOR out = VGet(0.0f, 0.0f, 0.0f);
	out.x = _in[0];
	out.y = _in[1];
	out.z = _in[2];
	return out;
}
