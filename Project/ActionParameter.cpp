#include "ActionParameter.h"

/// <summary>
/// コンストラクタ
/// </summary>
ActionParameter::ActionParameter()
	: desireValue(-1)
	, interval	 (0)
{

}

/// <summary>
/// 欲求値の追加
/// </summary>
void ActionParameter::AddDesireValue(const int _addValue)
{
	this->desireValue += _addValue;

	/*最大、最小の範囲から外れないように調整*/
	if (this->desireValue >= this->MAX_PARAMETER)
	{
		this->desireValue = this->MAX_PARAMETER;
	}
	else if (this->desireValue < 0)
	{
		this->desireValue = 0;
	}
}

/// <summary>
/// インターバルの計算
/// </summary>
bool ActionParameter::CalcInterval()
{
	/*インターバルが０より多かったらfalseを返してインターバルを減らす*/
	if (this->interval > 0)
	{
		this->interval--;
		return false;
	}
	return true;
}

/// <summary>
/// インターバルのセット
/// </summary>
void ActionParameter::SetInterval(const int _set)
{
	/*もしアクションが選択されていたら*/
	this->interval = _set;
}

const int ActionParameter::GetWeight(const int _sum)const
{
	if (this->desireValue == 0)
	{
		return 0;
	}
	return static_cast<int>(this->desireValue / _sum * this->BASE_WEIGHT);
}