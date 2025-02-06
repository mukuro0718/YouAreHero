//=============================================
// @brief ボス回転スラッシュ攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossRotateSlashAction : public BossAction
{
public:
	 BossRotateSlashAction();//コンストラクタ
	~BossRotateSlashAction();//デストラクタ

	void Initialize		()					override;//初期化
	void Update			(Boss& _boss)		override;//更新
	void CalcParameter	(const Boss& _boss)	override;//パラメーターの計算
private:
	/*定数*/
	const int	 ATTACK_TYPE;		//攻撃の種類
	const int	 HIT_STOP_DELAY;	//ヒットストップディレイ
	const float	 HIT_STOP_TIME;		//ヒットストップ時間
	const int	 HIT_STOP_TYPE;		//ヒットストップの種類
	const float	 SLOW_FACTOR;		//スロー係数
	const VECTOR ROTATE_LERP_VALUE;	//回転補正量
	const float	 ACTION_DISTANCE;	//アクションが可能になる距離
	const short	 CHECK_STATE;		//どの状態で確認するか
	const short  MAX_INTERVAL;		//最大インターバル
	const short	 ROTATE_PLAY_TIME;	//回転補正用

	/*メンバ変数*/
	bool	isClose;			//近づいたか
};