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
		SPHERE				= 0,//スフィア
		CAPSULE				= 1,//カプセル
		PLANE				= 2,//円平面
		CUBE				= 3,//キューブ
		CHARACTER_CAPSULE	= 4,//キャラクターカプセル
		ATTACK_SPHERE		= 5,//攻撃スフィア
		ATTACK_CAPSULE		= 6,//攻撃カプセル
		MODEL = 7,
	};

	/*位置補正の優先度判定に使う*/
	enum class Priority
	{
		LOW		= 0,//低
		HIGH	= 1,//高
		STATIC	= 2,//動かない
	};

	/*コンストラクタ*/
	ColliderData(const Kind _kind, const Priority _priority, const GameObjectTag _tag);

	/*デストラクタ*/
	virtual ~ColliderData();
	
	/*当たり判定種別取得*/
	Kind			GetKind			()const					 { return this->kind; }				//種類の取得
	GameObjectTag	GetTag			()const					 { return this->tag; }				//タグ情報
	Priority		GetPriority		()const					 { return this->priority; }			//優先権の取得
	VECTOR			GetNextPosition	()const					 { return this->nextPositon; }		//次の座標の取得
	void			SetNextPosition	(const VECTOR _position) { this->nextPositon = _position; }	//次の座標のセット

	/*メンバ変数*/
	Rigidbody rigidbody;//リジッドボディ
protected:
	Kind	kind;		//種類
	bool	isHit;		//当たったか
	VECTOR	nextPositon;//次の座標
private:
	GameObjectTag	tag;		//ゲームオブジェクトタグ
	Priority		priority;	//優先権
};
