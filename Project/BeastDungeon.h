//===================================================
// @brief ビーストダンジョン
//===================================================
#pragma once

class Dungeon;
class BeastDungeon : public Dungeon
{
public:
			 BeastDungeon();//コンストラクタ
	virtual ~BeastDungeon();//デストラクタ

		  void	Initialize() override;		//初期化
		  void	Update	  () override;		//更新
	const void	Draw	  () const override;//描画
private:
	MV1_REF_POLYGONLIST refPoly;
};

