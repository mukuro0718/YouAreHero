//==========================================
// @brief •½–Ê(¡‰ñ‚Í‰~Œ`‚É‚·‚é‚Ì‚Å”¼Œa‚à‚Â)‚ÌColliderData
//==========================================
#pragma once

namespace GoriLib
{
	class ColliderData;
	class ColliderDataPlane : public ColliderData
	{
	public:
		ColliderDataPlane(bool _isTrigger);

		VECTOR norm;//–@ü
		float radius;
	};
}
