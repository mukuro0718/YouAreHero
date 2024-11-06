#include <DxLib.h>
#include "AttackData.h"

AttackData::AttackData()
	: hitPosition	(VGet(0.0f, 0.0f, 0.0f))
	, isDoHitCheck	(false)
	, isHitAttack	(false)
	, damage		(0)
	, hitStopTime	(0)
	, hitStopType	(0)
	, hitStopDelay	(0)
	, slowFactor	(0.0f)
{

}