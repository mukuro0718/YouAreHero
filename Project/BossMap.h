//===========================================
// @brief 地面クラス
// 地面の描画を行う
//===========================================
#pragma once

class Physics;
class Collidable;
class BossMap : public GoriLib::Collidable
{
public:
	BossMap();//コンストラクタ
	~BossMap();//デストラクタ

	void		Initialize(GoriLib::Physics* _physics);		//初期化
	void		Finalize(GoriLib::Physics* _physics);		//後処理
	void		Update(GoriLib::Physics* _physics);		//更新
	void		OnCollide(const Collidable& _colider)override;//衝突したとき
	const void Draw()const;	//描画

private:
	/*メンバ変数*/
	int modelHandle;
};

