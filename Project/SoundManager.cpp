#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Sound.h"
#include "Bgm.h"
#include "SoundEffect.h"
#include "SoundManager.h"
#include "LoadingAsset.h"
#include "DeleteInstance.h"

/// <summary>
/// コンストラクタ
/// </summary>
SoundManager::SoundManager()
{
	/*サウンドクラスの作成*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->bgms.emplace_back(new Bgm(asset.GetSound(LoadingAsset::SoundType::TITLE_BGM), json.GetJson(JsonManager::FileType::SOUND)["TITLE_BGM_VOLUME"]));
	this->effects.emplace_back(new SoundEffect(asset.GetSound(LoadingAsset::SoundType::CONFIRMATION), json.GetJson(JsonManager::FileType::SOUND)["CONFIRMATION_SOUND_VOLUME"]));
	this->effects.emplace_back(new SoundEffect(asset.GetSound(LoadingAsset::SoundType::CANCEL), json.GetJson(JsonManager::FileType::SOUND)["CANCEL_SOUND_VOLUME"]));
	this->effects.emplace_back(new SoundEffect(asset.GetSound(LoadingAsset::SoundType::CHANGE_SELECT_ICON), json.GetJson(JsonManager::FileType::SOUND)["CHANGE_SELECT_ICON_VOLUME"]));
	this->effects.emplace_back(new SoundEffect(asset.GetSound(LoadingAsset::SoundType::SWORD_SLASH_1), json.GetJson(JsonManager::FileType::SOUND)["SWORD_SLASH_1_VOLUME"]));
	this->effects.emplace_back(new SoundEffect(asset.GetSound(LoadingAsset::SoundType::SWORD_SLASH_1), json.GetJson(JsonManager::FileType::SOUND)["SWORD_SLASH_1_VOLUME"]));
	this->effects.emplace_back(new SoundEffect(asset.GetSound(LoadingAsset::SoundType::SWORD_SLASH_2), json.GetJson(JsonManager::FileType::SOUND)["SWORD_SLASH_2_VOLUME"]));
	this->effects.emplace_back(new SoundEffect(asset.GetSound(LoadingAsset::SoundType::SWORD_SLASH_3), json.GetJson(JsonManager::FileType::SOUND)["SWORD_SLASH_3_VOLUME"]));
}

/// <summary>
/// コンストラクタ
/// </summary>
SoundManager::~SoundManager()
{
	/*メンバインスタンスの開放*/
	for (auto& bgm : this->bgms)
	{
		DeleteMemberInstance(bgm);
	}
	for (auto& effect : this->effects)
	{
		DeleteMemberInstance(effect);
	}
	this->effects.clear();
	this->bgms.clear();
}

/// <summary>
/// 更新
/// </summary>
void SoundManager::Initialize()
{
}

/// <summary>
/// 更新
/// </summary>
void SoundManager::Update()
{
	//int startTime = GetNowCount();
	
	/*BGMの更新*/
	for (auto& bgm : this->bgms)
	{
		bgm->Update();
	}

	/*効果音（サウンドエフェクト）の更新*/
	for (auto& effect : this->effects)
	{
		effect->Update();
	}

	//int endTime = GetNowCount();
	//this->frameTime = endTime - startTime;


	/*HACK:StopSoundMem(soundHandle)音声ファイルの停止*/
	/*HACK:SetVolumeoundMem(0~255,soundHandle)音量の調整*/
	/*HACK:GetCurrentSoundMem(soundHandle)再生位置の取得（ここでいう再生位置は音声ファイルのどの位置か）*/
	/*HACK:SetCurrentSoundMem(second(1s==1000),soundHandle)再生位置を設定*/
}

/// <summary>
/// 描画
/// </summary>
const void SoundManager::Draw()const
{
	//printfDx("EFFEK_FRAMETIME:%d\n", this->frameTime);
}

/// <summary>
/// エフェクト再生フラグを立てる
/// </summary>
void SoundManager::OnIsPlayBgm(const BgmType _type)
{
	int type = static_cast<int>(_type);
	this->bgms[type]->OnIsPlay();
}
void SoundManager::OnIsPlayEffect(const EffectType _type)
{
	int type = static_cast<int>(_type);
	this->effects[type]->OnIsPlay();
}

/// <summary>
/// エフェクト再生フラグを下す
/// </summary>
void SoundManager::OffIsPlayBgm(const BgmType _type)
{
	int type = static_cast<int>(_type);
	this->bgms[type]->OffIsPlay();
}
void SoundManager::OffIsPlayEffect(const EffectType _type)
{
	int type = static_cast<int>(_type);
	this->effects[type]->OffIsPlay();
}