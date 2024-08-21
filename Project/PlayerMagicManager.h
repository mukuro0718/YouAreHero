//===========================================================
// @brief プレイヤー魔法クラス
//===========================================================
#pragma once
#include "Singleton.h"

class PlayerMagic;
class PlayerMagicManager :public Singleton<PlayerMagicManager>
{
public:
	friend class Singleton<PlayerMagicManager>;

	void Initialize();		//初期化
	void Update();			//更新
	const void Draw()const;	//描画
private:
	/*静的定数*/
	static constexpr int MAGIC_NUM = 5;

	/*列挙体*/
	enum class Element
	{
		FRAME,
		BOLT,
		ICE,
		EARTH,
		STORM,
	};

	/*内部処理関数*/
	PlayerMagicManager();//コンストラクタ
	~PlayerMagicManager();//デストラクタ

	/*メンバ変数*/
	std::array<PlayerMagic* ,MAGIC_NUM> magic;
};

