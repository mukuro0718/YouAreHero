//==========================================
// @brief ダメージ用カプセルColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class CharacterData;
class AttackData;
class CharacterColliderData : public ColliderData
{
public:
	/*コンストラクタ*/
	CharacterColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, CharacterData* _data);

	void OnHit(const AttackData& _data, const VECTOR _attackPosition);//ヒット処理

	/*getter/setter*/
	const CharacterData& GetCharacterData ()const;			//キャラクターデータの取得

	/*メンバ変数*/
	CharacterData*	data;		//キャラクターデータ
	float			radius;		//半径
	VECTOR			topPositon;	//カプセル上座標
	bool			isUseCollWithGround;//地面との当たり判定を行うか
	bool			isUseCollWithChara;//キャラクターとの当たり判定を行うか
	bool			isSetTopPosition;//カプセル上座標を設定しているか（falseならyにheightのみ入っている）
};
