//========================================
// @brief マップマネージャークラス
// マップの管理を行う
//========================================
#pragma once
#include "Singleton.h"

class BossMap;
class Skydome;
class Dungeon;
class MutantDungeon;
class Gate;
class MapManager : public Singleton<MapManager>
{
public:
	friend class Singleton<MapManager>;

		  void Initialize();	 //初期化
		  void Finalize	 ();	 //後処理
		  void Update	 ();	 //更新
	const void Draw		 ()const;//描画

	/*列挙体*/
	//マップの種類
	enum class MapType
	{
		DUNGEON,
		BOSS,
	};

	/*getter*/
	const int	  GetStageModelHandle()const;
	const MapType GetMapType		 ()const { return this->mapType; }
		  void	  OnIsChangeStage	 () { this->isChangeStage = true; }
private:
	/*内部処理関数*/
	 MapManager();//コンストラクタ
	~MapManager();//デストラクタ

	/*メンバ変数*/
	BossMap*		 bossMap;		//ボスのマップ
	Skydome*		 skydome;		//スカイドーム
	Gate*			 gate;			//ゲート
	vector<Dungeon*> dungeon;		//ダンジョン
	int				 bossType;		//ボスの種類
	MapType			 mapType;		//現在表示中のマップ
	bool			 isChangeStage;	//ダンジョンからボスのマップに変更するときに使用するフラグ
};

