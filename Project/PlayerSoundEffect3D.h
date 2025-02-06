//=======================================================
// @brief　3DSoundEffectクラス
//=======================================================
#pragma once

class SoundEffect;
class PlayerSoundEffect3D : public SoundEffect
{
public:
	 PlayerSoundEffect3D(const int _soundHandle, const int _volume);//コンストラクタ
	~PlayerSoundEffect3D();											//デストラクタ

	void OnIsPlay() override;//再生フラグを立てる
	void Update() override;//更新
};

