//===============================================================
// @brief 物理、衝突判定するオブジェクトを登録し、
// 物理移動、衝突を通知する
//===============================================================
#pragma once
#include <list>
#include <vector>

namespace GoriLib
{
	class Collidable;
	class Physics final
	{
	public:
		void Entry(Collidable* collidable);//衝突物の登録
		void Exit(Collidable* collidable);//登録解除
		void Update();//更新(登録オブジェクトの物理移動、衝突通知)

		//指定ラインがオブジェクトとぶつかっているかどうかを判定し、
		//ぶつかっているオブジェクトを返す
		std::list<Collidable*> IsCollideLine(const VECTOR& _start, const VECTOR& _end)const;

		//重力と最大重力加速度
		static constexpr float GRAVITY = -0.01f;
		static constexpr float MAX_GRAVITY_ACCEL = -0.15f;
	private:
		static constexpr int BEFORE_FIX_INFO_COLOR = 0x0000ff;//補正前情報色
		static constexpr int AIM_INFO_COLOR = 0x0000aa;//補正前予定情報色
		static constexpr int AFTER_FIX_INFO_COLOR = 0xff00ff;//補正後情報色
		static constexpr int DIV_NUM = 16;
		static constexpr int MAX_CHECK_COUNT = 1000;

		std::list<Collidable*> collidables;//登録されたCollidableのリスト

		//OnCollidの遅延通知のためのデータ
		struct OnCollideInfo
		{
			Collidable* owner;
			Collidable* colider;
			void OnCollide() { owner->OnCollide(*colider); }
		};

		//当たり判定チェック
		std::vector<OnCollideInfo> CheckColide()const;
		bool IsCollide(const Collidable* _objectA, const Collidable* _objectB)const;

		//位置補正、決定
		void FixNextPosition(Collidable* _primary, Collidable* _secondary)const;
		void FixPosition();
	};
}
