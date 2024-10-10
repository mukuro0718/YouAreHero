#include <DxLib.h>
#include "ReactionType.h"
#include "CharacterData.h"

/// <summary>
/// コンストラクタ
/// </summary>
CharacterData::CharacterData()
	: hp(0)
	, isHit(false)
	, hitPosition(VGet(0.0f, 0.0f, 0.0f))
	, isInvinvible	(false)
	, isGuard	(false)
	, playerReaction(0)
{

}