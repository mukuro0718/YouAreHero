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

	/*項目の種類*/
	enum class ItemType
	{
		CAMERA = 0,
		PLAYER = 1,
		FPS = 2,
		ENEMY = 3,
	};

	/*getter*/
	const bool IsUseDebugMode()const;						//デバックモードを使用するか
	const bool IsShowDebugInfo(const ItemType _type)const;	//デバック情報を表示するか
private:
	
	
	/*内部処理関数*/
		 Debug			  ();//コンストラクタ
		 ~Debug			  ();//デストラクタ
	void ReloadingJsonFile();//Jsonファイルを再読み込み
	/*メンバ変数*/
	std::map<int, int> jsonIndexMap;		 //項目ごとの関数
	bool			   isReloadingJsonFile;	 //Jsonファイルを再読み込みを読み込んだか（連続読み込みを防ぐ）
};

