//==========================================
// @brief ‹…‚ÌColliderData
//==========================================
#pragma once

namespace GoriLib
{
	class ColliderData;
	class ColliderDataSphere : public ColliderData
	{
	public:
		ColliderDataSphere(bool _isTrigger);
		const int GetDamage()const { return this->damage; }
		const int GetHitNumber()const { return this->hitNumber; }

		float radius;
		int damage;
		int hitNumber;
	};
}
