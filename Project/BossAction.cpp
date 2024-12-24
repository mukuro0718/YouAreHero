#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"	
#include "DeleteInstance.h"
#include "VECTORtoUseful.h"
#include "Rigidbody.h"
#include "ReactionType.h"
#include "ActionParameter.h"
#include "HitStop.h"
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
	: parameter		(nullptr)
	, attack		(nullptr)
	, hitStop		(nullptr)
	, moveTarget	(Gori::ORIGIN)
	, isSelect		(false)
	, isInitialize	(false)
	, isPriority	(false)
	, isAllowAction (false)
	, frameCount	(0)
{
	/*インスタンスの作成*/
	this->parameter = new ActionParameter();
	this->hitStop	= new HitStop();

	/*functionMapの作成*/
	auto getDif = [](COLOR_F& _base, COLOR_F& _now, const int _modelHandle) {_base = _now = MV1GetDifColorScale(_modelHandle); };
	auto getSpc = [](COLOR_F& _base, COLOR_F& _now, const int _modelHandle) {_base = _now = MV1GetSpcColorScale(_modelHandle); };
	auto getEmi = [](COLOR_F& _base, COLOR_F& _now, const int _modelHandle) {_base = _now = MV1GetEmiColorScale(_modelHandle); };
	auto getAmb = [](COLOR_F& _base, COLOR_F& _now, const int _modelHandle) {_base = _now = MV1GetAmbColorScale(_modelHandle); };
	this->getColorScaleMap.emplace(static_cast<int>(ColorType::DIF), getDif);
	this->getColorScaleMap.emplace(static_cast<int>(ColorType::SPC), getSpc);
	this->getColorScaleMap.emplace(static_cast<int>(ColorType::EMI), getEmi);
	this->getColorScaleMap.emplace(static_cast<int>(ColorType::AMB), getAmb);
	auto setDif = [](const COLOR_F _color, const int _modelHandle) {MV1SetDifColorScale(_modelHandle, _color); };
	auto setSpc = [](const COLOR_F _color, const int _modelHandle) {MV1SetSpcColorScale(_modelHandle, _color); };
	auto setEmi = [](const COLOR_F _color, const int _modelHandle) {MV1SetEmiColorScale(_modelHandle, _color); };
	auto setAmb = [](const COLOR_F _color, const int _modelHandle) {MV1SetAmbColorScale(_modelHandle, _color); };
	this->setColorScaleMap.emplace(static_cast<int>(ColorType::DIF), setDif);
	this->setColorScaleMap.emplace(static_cast<int>(ColorType::SPC), setSpc);
	this->setColorScaleMap.emplace(static_cast<int>(ColorType::EMI), setEmi);
	this->setColorScaleMap.emplace(static_cast<int>(ColorType::AMB), setAmb);

	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*colorScaleListの作成*/
	COLOR_F color = { 1.0f,1.0f,1.0f,1.0f };
	for (int i = 0; i < json.GetJson(JsonManager::FileType::ENEMY)["COLOR_SCALE_NUM"]; i++)
	{
		this->baseColorScale.emplace_back(color);
		this->nowColorScale.emplace_back(color);
	}
}

/// <summary>
/// デストラクタ
/// </summary>
BossAction::~BossAction()
{
	DeleteMemberInstance(this->attack);
	DeleteMemberInstance(this->hitStop);
	DeleteMemberInstance(this->parameter);
	this->getColorScaleMap.clear();
	this->setColorScaleMap.clear();
	this->baseColorScale.clear();
	this->nowColorScale.clear();
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
	return this->parameter->desireValue;
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
	return this->parameter->BASE_WEIGHT;
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
