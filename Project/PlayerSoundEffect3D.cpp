#include <DxLib.h>
#include "UseSTL.h"
#include "Sound.h"
#include "SoundEffect.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "PlayerSoundEffect3D.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerSoundEffect3D::PlayerSoundEffect3D(const int _soundHandle, const int _volume)
	: SoundEffect(_soundHandle, _volume)
{
	//音が聞こえる距離を設定する
	Set3DRadiusSoundMem(this->DISTANCE_SOUND_CAN_HEARD, this->soundHandle);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerSoundEffect3D::~PlayerSoundEffect3D()
{
	StopSoundMem(this->soundHandle);
}

/// <summary>
/// 更新
/// </summary>
void PlayerSoundEffect3D::Update()
{
	//音の再生位置を設定
	auto& player = Singleton<PlayerManager>::GetInstance();
	Set3DPositionSoundMem(player.GetRigidbody().GetPosition(), this->soundHandle);
}

/// <summary>
/// 再生フラグを立てる
/// </summary>
void PlayerSoundEffect3D::OnIsPlay()
{
	//フラグを立てる
	this->isPlay = true;
	//音を停止する
	StopSoundMem(this->soundHandle);
	//再生位置を初期化する
	SetCurrentPositionSoundMem(0, this->soundHandle);
	//音の再生位置を設定
	auto& player = Singleton<PlayerManager>::GetInstance();
	Set3DPositionSoundMem(player.GetRigidbody().GetPosition(), this->soundHandle);
	//再生
	PlaySoundMem(this->soundHandle, DX_PLAYTYPE_BACK);
}
