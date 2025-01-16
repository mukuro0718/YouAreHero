#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "Effect.h"
#include "PlayerImpactEffect.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerImpactEffect::PlayerImpactEffect(const int _effectResourceHandle)
	: Effect		(_effectResourceHandle)
	, MAX_EFFECT_NUM(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::EFFECT)["PLAYER_IMPACT_NUM"])
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->scale		 = Gori::Convert(json.GetJson(JsonManager::FileType::EFFECT)["PLAYER_IMPACT_SCALE"]);
	this->startFrame = json.GetJson(JsonManager::FileType::EFFECT)["PLAYER_IMPACT_START_FRAME"];
	this->endFrame	 = json.GetJson(JsonManager::FileType::EFFECT)["PLAYER_IMPACT_END_FRAME"];

	for (int i = 0; i < this->MAX_EFFECT_NUM; i++)
	{
		this->isPlay.emplace_back(false);
		this->playingEffectHandle.emplace_back(-1);
		this->frameCount.emplace_back(0);
	}
}

/// <summary>
/// 更新
/// </summary>
void PlayerImpactEffect::Update()
{
	/*再生フラグが立っていなければ早期リターン*/
	if (this->isPlayEffect)
	{
		int count = 0;
		for (count = 0; count < this->MAX_EFFECT_NUM; count++)
		{
			if (!this->isPlay[count])
			{
				this->isPlay[count] = true;
				break;
			}
		}
		this->isPlayEffect = false;
	}

	for (int i = 0; i < this->MAX_EFFECT_NUM; i++)
	{
		if (!this->isPlay[i])continue;
		/*処理の開始時に一度だけ呼ぶ*/
		if (this->frameCount[i] == 0)
		{
			//トランスフォームの設定
			auto& player = Singleton<PlayerManager>::GetInstance();
			VECTOR rotation = player.GetRigidbody().GetRotation();
			this->transform->SetScale(this->scale);
			this->transform->SetRotationOfRadian(rotation);
		}

		/*フレーム計測*/
		this->frameCount[i]++;
		//開始フレームを超えていなければ早期リターン
		if (this->frameCount[i] < this->startFrame)return;

		/*エフェクトの更新*/
		//再生エフェクトのハンドル
		if (this->frameCount[i] == this->startFrame)
		{
			if (this->isPlay[i])
			{
				//再生するエフェクトのハンドルを取得
				this->playingEffectHandle[i] = PlayEffekseer3DEffect(this->effectResourceHandle);
				//回転率、拡大率、座標を設定
				SetRotationPlayingEffekseer3DEffect(this->playingEffectHandle[i], this->transform->GetRotation().x, this->transform->GetRotation().y, this->transform->GetRotation().z);
				SetScalePlayingEffekseer3DEffect(this->playingEffectHandle[i], this->transform->GetScale().x, this->transform->GetScale().y, this->transform->GetScale().z);
				SetPosPlayingEffekseer3DEffect(this->playingEffectHandle[i], this->transform->GetPosition().x, this->transform->GetPosition().y, this->transform->GetPosition().z);
			}
		}
		//Effekseerにより再生中のエフェクトを更新する。
		UpdateEffekseer3D();

		//終了フレームを超えていたら初期化
		if (this->frameCount[i] >= this->endFrame)
		{
			this->isPlayEffect = false;
			this->frameCount[i] = 0;
			this->isPlay[i] = false;
			StopEffekseer3DEffect(this->playingEffectHandle[i]);
			this->playingEffectHandle[i] = -1;
		}
	}
}

/// <summary>
/// 描画
/// </summary>
const void PlayerImpactEffect::Draw()const
{
	for (int i = 0; i < this->MAX_EFFECT_NUM; i++)
	{
		/*再生フラグが立っていないまたは開始フレームを超えていなければ早期リターン*/
		if (!this->isPlay[i] || this->frameCount[i] < this->startFrame)return;
		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer3D();
	}
}
