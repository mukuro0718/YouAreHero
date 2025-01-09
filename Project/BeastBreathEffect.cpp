#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Effect.h"
#include "BeastBreathEffect.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BeastBreathEffect::BeastBreathEffect(const int _effectResourceHandle)
	: Effect		(_effectResourceHandle)
	, useFrameIndex1(-1)
	, useFrameIndex2(-1)
	, useEffectNum	(0)
	, maxEffectNum	(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->scale			 = Gori::Convert(json.GetJson(JsonManager::FileType::EFFECT)["BEAST_BREATH_SCALE"]);
	this->startFrame	 = json.GetJson(JsonManager::FileType::EFFECT)["BEAST_BREATH_START_FRAME"];
	this->endFrame		 = json.GetJson(JsonManager::FileType::EFFECT)["BEAST_BREATH_END_FRAME"];
	this->useFrameIndex1 = json.GetJson(JsonManager::FileType::BEAST)["BREATH_FRAME_INDEX_USED_CAPSULE_DIRECTION"][0];
	this->useFrameIndex2 = json.GetJson(JsonManager::FileType::BEAST)["BREATH_FRAME_INDEX_USED_CAPSULE_DIRECTION"][1];
	this->maxEffectNum	 = json.GetJson(JsonManager::FileType::EFFECT)["BEAST_BREATH_MAX_PLAY_NUM"];
	this->moveSpeed		 = json.GetJson(JsonManager::FileType::EFFECT)["BEAST_BREATH_SPEED"];
	this->maxDistance	 = json.GetJson(JsonManager::FileType::EFFECT)["BEAST_BREATH_MAX_DISTANCE"];
	for (int i = 0; i < this->maxEffectNum; i++)
	{
		this->playingEffectHandle.emplace_back(-1);
		this->position.emplace_back(Gori::ORIGIN);
		this->velocity.emplace_back(Gori::ORIGIN);
		this->isPlay.emplace_back(false);
	}
}

/// <summary>
/// 更新
/// </summary>
void BeastBreathEffect::Update()
{
	/*再生フラグが立っていなければ早期リターン*/
	if (!this->isPlayEffect)return;

	/*処理の開始時に一度だけ呼ぶ*/
	if (this->frameCount == 0)
	{
		//スケールの設定
		this->transform->SetScale(this->scale);
	}

	/*フレーム計測*/
	this->frameCount++;
	//開始フレームを超えていなければ早期リターン
	if (this->frameCount < this->startFrame)return;

	auto& enemy = Singleton<EnemyManager>::GetInstance();
	VECTOR facePosition = MV1GetFramePosition(enemy.GetModelHandle(), this->useFrameIndex1);
	/*エフェクトの更新*/
	//再生エフェクトのハンドル
	if (this->frameCount == this->startFrame || this->frameCount % 3 == 0)
	{
		if (this->useEffectNum != this->maxEffectNum - 1)
		{
			//再生するエフェクトのハンドルを取得
			this->playingEffectHandle[this->useEffectNum] = PlayEffekseer3DEffect(this->effectResourceHandle);
			//回転率、拡大率、座標を設定
			SetScalePlayingEffekseer3DEffect(this->playingEffectHandle[this->useEffectNum], this->transform->GetScale().x, this->transform->GetScale().y, this->transform->GetScale().z);
			VECTOR nosePosition = MV1GetFramePosition(enemy.GetModelHandle(), this->useFrameIndex2);
			VECTOR direction = VSub(nosePosition, facePosition);
			direction.y = 0.0f;
			this->velocity[this->useEffectNum] = VScale(VNorm(direction), this->moveSpeed);
			this->position[this->useEffectNum] = facePosition;
			this->isPlay[this->useEffectNum] = true;
			this->useEffectNum++;
		}
	}
	//エフェクトの移動
	for (int i = 0; i < this->maxEffectNum; i++)
	{
		if (this->isPlay[i])
		{
			//初期位置からどれだけ離れているかを求める
			this->position[i] = VAdd(this->position[i], this->velocity[i]);
			float distance = VSquareSize(VSub(this->position[i], facePosition));
			//最大距離を超えていたら初期位置に戻す
			if (distance >= this->maxDistance)
			{
				this->position[i] = facePosition;
			}
			SetPosPlayingEffekseer3DEffect(this->playingEffectHandle[i], this->position[i].x, this->position[i].y, this->position[i].z);
		}
	}
	//Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	//終了フレームを超えていたら初期化
	if (this->frameCount >= this->endFrame)
	{
		this->isPlayEffect = false;
		this->frameCount = 0;
		this->useEffectNum = 0;
		for (int i = 0; i < this->maxEffectNum; i++)
		{
			StopEffekseer3DEffect(this->playingEffectHandle[i]);
			this->playingEffectHandle[i] = -1;
			this->isPlay[i] = false;
		}
	}
}