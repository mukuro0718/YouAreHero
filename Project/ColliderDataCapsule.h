//==========================================
// @brief ƒJƒvƒZƒ‹‚ÌColliderData
//==========================================
#pragma once

namespace GoriLib
{
	class ColliderData;
	class ColliderDataCapsule : public ColliderData
	{
	public:
		ColliderDataCapsule(bool _isTrigger);
		const int GetHP()const { return this->hp; }
		const int GetHitNumber()const { return this->hitNumber; }
		float radius;
		float height;
		int hp;
		int hitNumber;
		bool isCutDamage;
	};
}
