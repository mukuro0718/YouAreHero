//===================================================
// @brief ドラゴンダンジョン
//===================================================
#pragma once

class Dungeon;
class DragonDungeon : public Dungeon
{
public:
			 DragonDungeon();//コンストラクタ
	virtual ~DragonDungeon();//デストラクタ

		  void	Initialize() override;		//初期化
		  void	Update	  () override;		//更新
	const void	Draw	  () const override;//描画
private:
	MV1_REF_POLYGONLIST refPoly;
};

