#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
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
	, speed			(1.0f)
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
/// ラープ
/// </summary>
float Character::Lerp(const float _start, const float _end, const float _percent)
{
	return _start + _percent * (_end - _start);
}
VECTOR Character::Lerp(const VECTOR _start, const VECTOR _end, const VECTOR _percent)
{
	VECTOR out = (Gori::ORIGIN);
	out.x = Lerp(_start.x, _end.x, _percent.x);
	out.y = Lerp(_start.y, _end.y, _percent.y);
	out.z = Lerp(_start.z, _end.z, _percent.z);
	return out;
}

/// <summary>
/// atan2で出した値を360度に直し補完しながら回転させる
/// </summary>
VECTOR Character::Lerp360Angle(const VECTOR _start, const VECTOR _end, const VECTOR _percent)
{
	vector<float> startBase = { _start.x,_start.y,_start.z };
	vector<float> endBase = { _end.x,_end.y,_end.z };
	vector<float> t = { _percent.x,_percent.y,_percent.z };
	vector<float> out;
	const float PI_2 = (2.0f * DX_PI_F);

	for (int i = 0; i < startBase.size(); i++)
	{
		/*開始地点と終了地点を求める*/
		float start = startBase[i];
		float end = endBase[i];
		//終了地点がマイナスだったら2πを足す
		if (end < 0.0f) end += PI_2;

		/*開始地点と終了地点の差を求める*/
		float diff = start - end;
		//もし差がマイナスだったら-1をかける
		if (diff < 0.0f) diff *= -1.0f;

		/*余りを求める*/
		float excess = PI_2 - diff;

		/*差と余りを求めて、差のほうが大きければBaseのほうで計算*/
		if (diff > excess)
		{
			//スタートベースは360度に変換されているので、π ~ -πの範囲に戻す
			if (startBase[i] >= DX_PI_F)
			{
				startBase[i] -= PI_2;
			}
			out.emplace_back(Lerp(startBase[i], endBase[i], t[i]));
		}

		/*余りのほうが大きければそのまま計算*/
		else
		{
			out.emplace_back(Lerp(start, end, t[i]));
		}

		/*360度の範囲を超えないようにする*/
		if (out[i] <= 0.0f)
		{
			out[i] += PI_2;
		}
		else if (out[i] >= PI_2)
		{
			out[i] -= PI_2;
		}
	}

	VECTOR rotation = VGet(0.0f, out[1], 0.0f);
	return rotation;
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

/// <summary>
/// 回転率のセット
/// </summary>
void Character::SetRotation(const VECTOR _now, const VECTOR _next)
{
	this->nextRotation = _next;
	this->collider->rigidbody.SetRotation(_now);
}

/// <summary>
/// ベロシティの設定
/// </summary>
void Character::SetVelocity(const VECTOR _velocity)
{
	this->collider->rigidbody.SetVelocity(_velocity);
}