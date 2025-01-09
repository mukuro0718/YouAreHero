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
	auto& json = Singleton<JsonManager>::GetInstance();
	this->yOffset	 = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_ROAR_Y_OFFSET"];
	this->scale		 = Gori::Convert(json.GetJson(JsonManager::FileType::EFFECT)["BOSS_ROAR_SCALE"]);
	this->startFrame = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_ROAR_START_FRAME"];
	this->endFrame	 = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_ROAR_END_FRAME"];
}

/// <summary>
/// 更新
/// </summary>
void BossRoarEffect::Update()
{
	/*再生フラグが立っていなければ早期リターン*/
	if (!this->isPlayEffect)return;

	/*処理の開始時に一度だけ呼ぶ*/
	if (this->frameCount == 0)
	{
		//トランスフォームの設定
		auto& enemy = Singleton<EnemyManager>::GetInstance();
		VECTOR position = enemy.GetRigidbody().GetPosition();
		position.y += this->yOffset;
		VECTOR rotation = enemy.GetRigidbody().GetRotation();
		this->transform->SetPosition(position);
		this->transform->SetScale(this->scale);
		this->transform->SetRotationOfRadian(rotation);
	}

	/*フレーム計測*/
	this->frameCount++;
	//開始フレームを超えていなければ早期リターン
	if (this->frameCount < this->startFrame)return;

	/*エフェクトの更新*/
	//再生エフェクトのハンドル
	if (this->frameCount == this->startFrame)
	{
		//再生するエフェクトのハンドルを取得
		this->playingEffectHandle = PlayEffekseer3DEffect(this->effectResourceHandle);
		//回転率、拡大率、座標を設定
		SetRotationPlayingEffekseer3DEffect(this->playingEffectHandle, this->transform->GetRotation().x, this->transform->GetRotation().y, this->transform->GetRotation().z);
		SetScalePlayingEffekseer3DEffect(this->playingEffectHandle, this->transform->GetScale().x, this->transform->GetScale().y, this->transform->GetScale().z);
		SetPosPlayingEffekseer3DEffect(this->playingEffectHandle, this->transform->GetPosition().x, this->transform->GetPosition().y, this->transform->GetPosition().z);
	}
	//Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	//終了フレームを超えていたら初期化
	if (this->frameCount >= this->endFrame)
	{
		this->isPlayEffect = false;
		this->frameCount = 0;
		StopEffekseer3DEffect(this->playingEffectHandle);
		this->playingEffectHandle = -1;
	}
}