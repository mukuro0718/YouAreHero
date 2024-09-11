//=========================================================
// 当たり判定データクラス
//=========================================================
#pragma once
#include "GameObjectTag.h"

class Rigidbody;
class ColliderData abstract
{
public:
	/*当たり判定種別*/
	enum class Kind
	{
		SPHERE,
		CAPSULE,
		PLANE,
		CUBE,
		CHARACTER_CAPSULE,
		ATTACK_SPHERE,
		ATTACK_CAPSULE,
	};
	/*位置補正の優先度判定に使う*/
	enum class Priority
	{
		LOW,	//低
		HIGH,	//高
		STATIC,//動かない
	};

	/*コンストラクタ*/
	ColliderData(const Kind _kind, const Priority _priority, const GameObjectTag _tag);
	/*デストラクタ*/
	virtual ~ColliderData();
	
	/*当たり判定種別取得*/
	Kind		  GetKind	 ()const { return this->kind; }
	GameObjectTag GetTag	 ()const { return this->tag; }//タグ情報
	Priority	  GetPriority()const { return this->priority; }
	VECTOR GetNextPosition()const { return this->nextPositon; }
	void SetNextPosition(const VECTOR _position) { this->nextPositon = _position; }
	Rigidbody rigidbody;
protected:
	Kind kind;
	bool isHit;
	VECTOR nextPositon;
private:
	GameObjectTag tag;
	Priority priority;
};
