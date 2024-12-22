#include <DxLib.h>
#include "ReactionType.h"
#include "CharacterData.h"

/// <summary>
/// コンストラクタ
/// </summary>
CharacterData::CharacterData()
	: hitPosition	(VGet(0.0f, 0.0f, 0.0f))
	, isHit			(false)
	, isGuard		(false)
	, isInvinvible	(false)
	, hp			(0)
	, reactionType	(0)
	, hitStopTime	(0)
	, hitStopType	(0)
	, hitStopDelay	(0)
	, slowFactor	(0.0f)
{

}