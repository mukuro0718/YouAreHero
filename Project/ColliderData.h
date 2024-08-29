#pragma once

namespace GoriLib
{
	class ColliderData abstract
	{
	public:
		/*当たり判定種別*/
		enum class Kind
		{
			SPHERE,
			CAPSULE,
			PLANE,
			CUBE,
		};

		/*コンストラクタ*/
		ColliderData(Kind _kind, bool isTrigger)
		{
			this->kind = _kind;
			this->isTrigger = isTrigger;
		}

		virtual ~ColliderData() {}

		/*当たり判定種別取得*/
		Kind GetKind()const { return this->kind; }

		/*トリガーかどうか*/
		bool IsTrigger()const { return this->isTrigger; }

	private:
		Kind kind;
		bool isTrigger;
	};
}