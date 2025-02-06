//=======================================================
// @brief　3DSoundEffectクラス
//=======================================================
#pragma once

class SoundEffect;
class EnemySoundEffect3D : public SoundEffect
{
public:
	 EnemySoundEffect3D(const int _soundHandle, const int _volume);	//コンストラクタ
	~EnemySoundEffect3D();											//デストラクタ

	void OnIsPlay() override;//再生フラグを立てる
	void Update() override;//更新
};

