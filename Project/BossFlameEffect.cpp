#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Effect.h"
#include "BossFlameEffect.h"
#include "EnemyManager.h"

BossFlameEffect::BossFlameEffect(const int _effectResourceHandle)
	: Effect(_effectResourceHandle)
{
}

/// <summary>
/// エフェクトの再生に必要な情報の設定
/// </summary>
void BossFlameEffect::SetInfoToPlayTheEffect()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	VECTOR scale = Convert(json.GetJson(JsonManager::FileType::EFFECT)["BOSS_FLAME_SCALE"]);
	VECTOR rotation = enemy.GetRigidbody().GetRotation();

	this->transform->SetScale(scale);
	this->transform->SetRotation(rotation);

	this->startFrame = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_FLAME_START_FRAME"];
	this->endFrame = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_FLAME_END_FRAME"];
	
}

/// <summary>
/// 更新
/// </summary>
void BossFlameEffect::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

		int flameNum = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_FLAME_EFFECT_NUM"];
	while(this->playingEffectHandle.size() == flameNum)
	{
		for (int i = 0; i < flameNum; i++)
		{
			this->playingEffectHandle.emplace_back(-1);
		}
	}

	/*再生フラグが立っていなければ早期リターン*/
	if (!this->isPlayEffect)return;

	/*エフェクトの再生に必要な情報の設定*/
	SetInfoToPlayTheEffect();

	/*フレーム計測*/
	//フレームの増加
	this->frameCount++;
	//開始フレームを超えていなければ早期リターン
	if (this->frameCount < this->startFrame)return;

	/*エフェクトの更新*/
	//再生エフェクトのハンドル
	vector<int> frameIndex = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_FRAME_INDEX"];
	for (int i = 0; i < flameNum; i++)
	{
		if (this->frameCount == this->startFrame)
		{
			this->playingEffectHandle[i] = PlayEffekseer3DEffect(this->effectResourceHandle);
			SetRotationPlayingEffekseer3DEffect(this->playingEffectHandle[i], this->transform->GetRotation().x, this->transform->GetRotation().y, this->transform->GetRotation().z);
			SetScalePlayingEffekseer3DEffect(this->playingEffectHandle[i], this->transform->GetScale().x, this->transform->GetScale().y, this->transform->GetScale().z);
		}
		// 再生中のエフェクトを移動する
		VECTOR position = MV1GetFramePosition(enemy.GetModelHandle(), frameIndex[i]);
		SetPosPlayingEffekseer3DEffect(this->playingEffectHandle[i], position.x, position.y, position.z);
		//Effekseerにより再生中のエフェクトを更新する。
		UpdateEffekseer3D();

		//終了フレームを超えていたら再生フラグを下す
		if (this->frameCount >= this->endFrame)
		{
			this->isPlayEffect = false;
			this->frameCount = this->firstFrame;
			StopEffekseer3DEffect(this->playingEffectHandle[i]);
			this->playingEffectHandle[i] = -1;
		}
	}
}

/// <summary>
/// 座標の設定
/// </summary>
void BossFlameEffect::SetPosition(const VECTOR _position)
{
	this->transform->SetPosition(_position);
}