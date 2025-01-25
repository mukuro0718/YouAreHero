//=======================================================
// @brief　BGMクラス
//=======================================================
#pragma once

class Sound;
class Bgm : public Sound
{
public:
			 Bgm(const int _soundHandle, const int _volume);//コンストラクタ
	virtual ~Bgm();											//デストラクタ

	void OnIsPlay() override;//再生フラグを立てる
	void Update	 () override;//更新
private:
	int tmpSoundHandle;
};

