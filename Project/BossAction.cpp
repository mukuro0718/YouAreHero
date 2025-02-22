#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"	
#include "DeleteInstance.h"
#include "VECTORtoUseful.h"
#include "Rigidbody.h"
#include "ReactionType.h"
#include "ActionParameter.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "BossAttack.h"
#include "Boss.h"
#include "BossAction.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossAction::BossAction()
	: parameter				(nullptr)
	, attack				(nullptr)
	, moveTarget			(Gori::ORIGIN)
	, isSelect				(false)
	, frameCount			(0)
	, isInitialize			(false)
	, isPriority			(false)
	, isAllowAction			(false)
	, frameTime				(0)
	, nextAnimation			(0)
	, animationPlayTime		(0.0f)
	, maxDesireValue		(0)
	, nowTotalAnimPlayTime	(0.0f)
	, interval				(0)
{
	/*インスタンスの作成*/
	this->parameter = new ActionParameter();
}

/// <summary>
/// デストラクタ
/// </summary>
BossAction::~BossAction()
{
	DeleteMemberInstance(this->attack);
	DeleteMemberInstance(this->parameter);
}
/// <summary>
/// 描画
/// </summary>
const void BossAction::Draw()const
{
	//if (this->attack != nullptr)
	//{
	//	this->attack->Draw();
	//}
	//printfDx("ENEMY_FRAMETIME:%d\n", this->frameTime);
}

/// <summary>
/// 欲求値の取得
/// </summary>
const int BossAction::GetDesireValue() const
{
	return static_cast<int>(this->parameter->desireValue);
}

/// <summary>
/// 重みの取得
/// </summary>
const int BossAction::GetWeight(const int _sum)const
{
	return this->parameter->GetWeight(_sum);
}

/// <summary>
/// 重みのベースの取得
/// </summary>
const int BossAction::GetBaseWeight()const
{
	return static_cast<int>(this->parameter->BASE_WEIGHT);
}
/// <summary>
/// 指定したフレームの計測
/// </summary>
bool BossAction::FrameCount(const int _maxFrame)
{
	//カウントを増加させる
	this->frameCount++;
	//もし最大を越していたらフラグを下してカウントを初期化する
	if (this->frameCount >= _maxFrame)
	{
		return true;
	}
	return false;
}

/// <summary>
/// 変数の初期化
/// </summary>
void BossAction::OffIsSelect(const int _maxInterval)
{
	/*選択されていたら*/
	if (this->isSelect)
	{
		this->isSelect = false;
		this->isPriority = false;
		this->isAllowAction = false;
		this->frameCount = 0;
		this->parameter->desireValue = 0;
		this->interval = _maxInterval;
	}
}

/// <summary>
/// 補完した回転率の取得
/// </summary>
VECTOR BossAction::GetLerpRotation(Boss& _boss, const VECTOR _positionToTargetVector, const VECTOR _nowRotation, const VECTOR _lerpValue)
{
	VECTOR nextRotation = Gori::ORIGIN;
	VECTOR rotation = Gori::ORIGIN;
	//次の回転率を求める
	nextRotation.y = static_cast<float>(atan2(static_cast<double>(_positionToTargetVector.x), static_cast<double>(_positionToTargetVector.z)));
	//回転率を保管する
	rotation = _boss.Lerp360Angle(_nowRotation, nextRotation, _lerpValue);
	return rotation;
}

/// <summary>
/// 移動ベクトルの計算
/// </summary>
VECTOR BossAction::CalcVelocity(const VECTOR _prevVelocity, const VECTOR _nowRotation, const float _speed)
{
	//回転率をもとに移動ベクトルを出す
	VECTOR direction = VGet(-sinf(_nowRotation.y), 0.0f, -cosf(_nowRotation.y));
	//移動ベクトルを正規化
	direction = VNorm(direction);
	//新しい移動ベクトルを出す（重力を考慮して、Y成分のみ前のものを使用する）
	VECTOR aimVelocity = VScale(direction, _speed);					 //算出された移動ベクトル
	VECTOR newVelocity = VGet(aimVelocity.x, _prevVelocity.y, aimVelocity.z);//新しい移動ベクトル

	return newVelocity;
}


/// <summary>
/// ラープ
/// </summary>
float BossAction::Lerp(const float _start, const float _end, const float _percent)
{
	return _start + _percent * (_end - _start);
}
COLOR_F BossAction::LerpColor(const COLOR_F _start, const COLOR_F _end, const COLOR_F _percent)
{
	COLOR_F out;
	out.r = Lerp(_start.r, _end.r, _percent.r);
	out.g = Lerp(_start.g, _end.g, _percent.g);
	out.b = Lerp(_start.b, _end.b, _percent.b);
	out.a = Lerp(_start.a, _end.a, _percent.a);
	return out;
}

/// <summary>
/// vector<float>をCOLOR_Fに変換
/// </summary>
const COLOR_F BossAction::ColorConvert(const std::vector<float> _value)
{
	const COLOR_F out = { _value[0],_value[1], _value[2],_value[3] };
	return out;
}
