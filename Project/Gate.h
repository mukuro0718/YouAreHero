//======================================================
// @brief ゲートクラス
// ダンジョンからボスのマップへ切り替えるための処理を行う
// モデルはドア
//======================================================
#pragma once

class Gate
{
public:
	 Gate();//コンストラクタ
	~Gate();//デストラクタ

		  void Initialize(const int _bossType);	//初期化
		  void Update	 ();					//更新
	const void Draw		 ()const;				//描画
private:
	/*定数*/
	const float ACTION_LENGTH;//ゲートアクションが可能な距離

	/*メンバ変数*/
	int		modelHandle;	//モデルハンドル
	VECTOR	position;		//座標
	bool	isNearPlayer;	//プレイヤーが近くにいるか
};

