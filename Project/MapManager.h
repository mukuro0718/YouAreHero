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

	void Initialize(GoriLib::Physics* _physics); //初期化
	void Finalize(GoriLib::Physics* _physics);
	void Update(GoriLib::Physics* _physics); //更新
	void OnCollide(const GoriLib::Collidable& _colider); //移動ベクトルの修正
	const void Draw	()const;//描画

private:
	/*内部処理関数*/
	 MapManager();//コンストラクタ
	~MapManager();//デストラクタ

	/*メンバ変数*/
	BossMap* ground;
	Skydome* skydome;
};

