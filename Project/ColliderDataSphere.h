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

		float radius;
	};
}
