#include <DxLib.h>
#include "GoriLib.h"

using namespace GoriLib;


ColliderDataCapsule::ColliderDataCapsule(bool _isTrigger)
	: ColliderData(ColliderData::Kind::CAPSULE, _isTrigger)
	, radius(0.0f)
	, height(0.0f)
{
	//ˆ—‚È‚µ
}