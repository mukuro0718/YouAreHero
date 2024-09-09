#include <DxLib.h>
#include "GoriLib.h"

using namespace GoriLib;


ColliderDataPlane::ColliderDataPlane(bool _isTrigger)
	: ColliderData(ColliderData::Kind::PLANE, _isTrigger)
	, norm	(VGet(0.0f, 0.0f, 0.0f))
	, radius(0.0f)
{
	//ˆ—‚È‚µ
}