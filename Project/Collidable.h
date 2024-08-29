//==========================================================
// @brief 衝突できる可能性のあるものに継承
//==========================================================
#pragma once
#include <list>
#include "GameObjectTag.h"

namespace GoriLib
{
	class Rigidbody;
	class Physics;
	class ColliderData;
	class Collidable abstract
	{
	public:
		/*位置補正の優先度判定に使う*/
		enum class Priority
		{
			LOW,	//低
			HIGH,	//高
			STATIC,//動かない
		};

		Collidable(Priority _priority, GameObjectTag _tag, ColliderData::Kind _colliderKind, bool _isTrigger);//コンストラクタ
		virtual ~Collidable();//デストラクタ
		virtual void Initialize(GoriLib::Physics* _physics);
		virtual void Finalize(GoriLib::Physics* _physics);

		virtual void OnCollide(const Collidable& _colider) abstract;//衝突したとき

		GameObjectTag GetTag()const { return tag; }//タグ情報
		Priority GetPriority()const { return priority; }

		//当たり判定を無視するタグの追加/削除
		void AddThroughTag(GameObjectTag _tag);
		void RemoveThroughTag(GameObjectTag _tag);

		//当たり判定を無視する対象かどうか
		bool IsThroughTarget(const Collidable* _target)const;

	protected:
		Rigidbody rigidbody;//物理データ
		ColliderData* colliderData;//当たり判定データ
	private:
		ColliderData* CreateColliderData(ColliderData::Kind _kind, bool _isTrigger);

		GameObjectTag tag;
		Priority priority;
		std::list<GameObjectTag> throughTags;//当たり判定を無視するためのリスト
		
		
		friend Physics;//PhysicsがCollidableを自由に管理するためにフレンド化
		/*以下フレンドであるPhysicsのみが扱う型や変数*/
	private:
		//これはPhysicsが持つほうが望ましい
		VECTOR nextPosition;

	};
}
