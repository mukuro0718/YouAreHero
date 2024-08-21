#include "BitFlag.h"

/// <summary>
/// 引数のフラグがONになっているかどうか
/// </summary>
const bool BitFlag::CheckFlag(const unsigned int _flag)const
{
	//もし引数のフラグが立っていなかったらfalseを返す
	if (!(this->flags & _flag)) return false;
	return true;
}

/// <summary>
/// フラグをONにする
/// </summary>
void BitFlag::SetFlag(const unsigned int _flag)
{
	this->flags |= _flag;
}

/// <summary>
/// フラグをOFFにする
/// </summary>
void BitFlag::ClearFlag(const unsigned int _flag)
{
	this->flags &= ~_flag;
}
