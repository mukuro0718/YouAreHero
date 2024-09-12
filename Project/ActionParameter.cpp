#include "ActionParameter.h"

/// <summary>
/// コンストラクタ
/// </summary>
ActionParameter::ActionParameter()
	: desireValue(0)
	, interval	 (0)
{

}


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