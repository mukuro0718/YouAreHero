//=======================================================
// @brief　SoundEffectクラス
//=======================================================
#pragma once

class Sound;
class SoundEffect : public Sound
{
public:
	SoundEffect(const int _soundHandle, const int _volume);	//コンストラクタ
	virtual ~SoundEffect();									//デストラクタ

	void Update() override;//更新
};

