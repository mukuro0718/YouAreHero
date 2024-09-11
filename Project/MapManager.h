//========================================
// @brief マップマネージャークラス
// マップの管理を行う
//========================================
#pragma once
#include "Singleton.h"

class BossMap;
class Skydome;
class MapManager : public Singleton<MapManager>
{
public:
	friend class Singleton<MapManager>;

	void Initialize(); //初期化
	void Finalize();
	void Update(); //更新
	const void Draw	()const;//描画

private:
	/*内部処理関数*/
	 MapManager();//コンストラクタ
	~MapManager();//デストラクタ

	/*メンバ変数*/
	BossMap* ground;
	Skydome* skydome;
};

