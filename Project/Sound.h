//=======================================================
// @brief　soundの基底クラス
// 各soundクラスはこのクラスを継承する
//=======================================================
#pragma once
class Sound abstract
{
public:
			 Sound(const int _soundHandle, const int _volume);	//コンストラクタ
	virtual ~Sound();						//デストラクタ

	virtual void Update	 ()abstract;//更新
	virtual void OnIsPlay();		//再生フラグを立てる
	virtual void OffIsPlay();		//再生フラグを下す
	const bool GetCheckSoundState();
protected:
	/*定数*/
	const float DISTANCE_SOUND_CAN_HEARD;

	/*メンバ変数*/
	int  soundHandle;//サウンドハンドル
	bool isPlay;	 //再生するか
	int  volume;	 //音量
};

