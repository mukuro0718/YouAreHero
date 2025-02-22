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
	, stamina		(0.0f)
	, reactionType	(0)
	, hitStopTime	(0)
	, hitStopType	(0)
	, hitStopDelay	(0)
	, slowFactor	(0.0f)
	, defensivePower(1.0f)
	, damage		(0.0f)
{

}