#include <DxLib.h>
#include "UseSTL.h"
#include "DeleteInstance.h"
#include "ReactionType.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "ColliderData.h"
#include "CharacterColliderData.h"
#include "BitFlag.h"
#include "Animation.h"
#include "Character.h"

/// <summary>
/// コンストラクタ
/// </summary>
Character::Character()
	: modelHandle	(-1)
	, animation		(nullptr)
	, state			(nullptr)
	, collider		(nullptr)
	, speed			(0.0f)
	, isAlive		(false)
	, isGround		(false)
	, entryInterval	(0)
{
	this->animation = new Animation();
	this->state = new BitFlag();
}

/// <summary>
/// デストラクタ
/// </summary>
Character::~Character()
{
	DeleteMemberInstance(this->animation);
	DeleteMemberInstance(this->state);
	DeleteMemberInstance(this->collider);
	MV1DeleteModel(this->modelHandle);
}

/// <summary>
/// 描画
/// </summary>
const void Character::Draw() const
{


	/*キャラクター情報の表示*/
	DrawCharacterInfo();

	/*モデルの描画*/
	if (this->isDraw)
	{
		MV1SetPosition(this->modelHandle, this->collider->rigidbody.GetPosition());
		MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
		MV1SetScale(this->modelHandle, this->collider->rigidbody.GetScale());
		MV1DrawModel(this->modelHandle);
	}
}

/// <summary>
/// 線形補完
/// </summary>
float Character::Lerp(const float _start, const float _end, const float _percent)
{
	return _start + _percent * (_end - _start);
}
VECTOR Character::Lerp(const VECTOR _start, const VECTOR _end, const VECTOR _percent)
{
	VECTOR out = { 0.0f,0.0f,0.0f };
	out.x = Lerp(_start.x, _end.x, _percent.x);
	out.y = Lerp(_start.y, _end.y, _percent.y);
	out.z = Lerp(_start.z, _end.z, _percent.z);
	return out;
}

/// <summary>
/// ダメージデータの取得
/// </summary>
const CharacterData& Character::GetCharacterData()const
{
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	return *collider.data;
}

const int Character::GetHP()const
{
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	return collider.data->hp;
}

/// <summary>
/// リジッドボディの取得
/// </summary>
const Rigidbody& Character::GetRigidbody()const
{
	return this->collider->rigidbody;
}

/// <summary>
/// アニメーション変更フラグの取得
/// </summary>
const bool Character::GetIsChangeAnimation()const
{
	return this->animation->GetIsChangeAnim();
}
