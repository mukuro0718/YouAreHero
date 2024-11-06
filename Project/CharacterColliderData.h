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
		  void			 SetPlayerReaction(const int _type);//プレイヤーリアクションのセット

	/*メンバ変数*/
	CharacterData*	data;		//キャラクターデータ
	float			radius;		//半径
	VECTOR			topPositon;	//カプセル上座標
};
