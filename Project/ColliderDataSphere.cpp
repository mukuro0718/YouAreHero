#include <DxLib.h>
#include "GoriLib.h"

using namespace GoriLib;


ColliderDataSphere::ColliderDataSphere(bool _isTrigger)
	: ColliderData(ColliderData::Kind::SPHERE,_isTrigger)
	, radius(0.0f)
	, damage(0)
	, hitNumber(0)
{
	//ˆ—‚È‚µ
}