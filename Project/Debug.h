//==================================================
// @brief デバッククラス
// デバッグ機能を追加する
//==================================================
#pragma once
#include "Singleton.h"

class BitFlag;
class Debug : public Singleton<Debug>
{
public:
	friend class Singleton<Debug>;

	void Initialize();		//初期化
	void Update	   ();		//更新
	const void Draw()const;	//描画

	/*getter*/
	const bool CheckFPSFlag()const;
	const bool CheckPlayerFlag()const;
	const bool CheckEnemyFlag()const;
	const bool CheckCameraFlag()const;//カメラのデバック機能がONになっているかどうか
	const int GetActionType()const { return this->currentlySelectedAction; }
private:
	/*ファンクション*/
	typedef std::function<void(void)> FlagsState;//フラグの切り替えを行う
	
	/*クラス・構造体*/
	//フラグの切り替えを行う関数群
	class FlagsStateSet
	{
	public:
		FlagsState set;
		FlagsState clear;
	};
	//項目の種類
	enum class ItemType
	{
		CAMERA	= 0,
		PLAYER	= 1,
		FPS		= 2,
		ENEMY	= 3,
	};
	//ボスのアクション
	enum class ActionType
	{
		NONE			= -1,
		DYING			= 0,//デス
		IDLE			= 1,//待機
		ROAR			= 2,//咆哮
		WALK			= 3,//歩き
		WALK_LEFT		= 4,//歩き
		WALK_RIGHT		= 5,//歩き
		REST			= 6,//歩き
		SLASH			= 7,//スラッシュ
		FLY_ATTACK		= 8,//飛び攻撃
		HURRICANE_KICK	= 9,//回転蹴り
		STAB			= 10,//突き刺し攻撃
		ROTATE_PUNCH	= 11,//回転パンチ
	};

	/*静的定数*/
	static const int COLOR_WHITE;//白色
	static const int COLOR_BLACK;//黒色
	static constexpr unsigned int CAMERA		 = (1 << 0);//カメラ
	static constexpr unsigned int PLAYER		 = (1 << 1);//プレイヤー
	static constexpr unsigned int FPS			 = (1 << 2);//FPS
	static constexpr unsigned int ENEMY			 = (1 << 3);//エネミー
	static constexpr unsigned int MASK_ALL_FLAGS = CAMERA | PLAYER | FPS | ENEMY;
	
	/*内部処理関数*/
			   Debug();																			 //コンストラクタ
			   ~Debug();																		 //デストラクタ
		  void ChangeSelectedItem();															 //選択項目の変更
		  void ChangeFlagsState	 ();															 //フラグの状態の変更
		  void AddItemFunction	 (const int _item,const FlagsState _set,const FlagsState _clear);//項目ごとの関数の追加
	const bool IsShow			 ()const;														 //デバック項目を表示させるか
	const unsigned int GetBitFlagInMap(const int _itemName)const;
	void ChangeBossActionType();
	/*メンバ変数*/
	std::map<int, FlagsStateSet> itemFunctionMap;		//項目ごとの関数
	BitFlag*					 flags;					//フラグ（デバック機能の使用状況）
	bool						 isShow;				//デバック項目を表示させるか
	int							 previousSelectedItem;	//以前選択されていた項目
	int							 currentlySelectedItem;	//今選択されている項目
	int prevSelectedAction;
	int currentlySelectedAction;
};

