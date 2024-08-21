#include <DxLib.h>
#include "PlayerMagic.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerMagic::PlayerMagic()
	: position  { 0.0f,0.0f,0.0f }
	, moveVector  { 0.0f,0.0f,0.0f }
	, radius	  (0.0f)
	, isStart	  (false)
	, isInitialize(false)
{

}

