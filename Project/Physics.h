//===============================================================
// @brief 物理、衝突判定するオブジェクトを登録し、
// 物理移動、衝突を通知する
//===============================================================
#pragma once

class Collidable;
class Physics final
{
public:
	void Entry(Collidable* collidable);//衝突物の登録
	void Exit(Collidable* collidable);//登録解除
	void Update();//更新(登録オブジェクトの物理移動、衝突通知)
private:
	std::list<Collidable*> collidables;//登録されたCollidableのリスト

};

