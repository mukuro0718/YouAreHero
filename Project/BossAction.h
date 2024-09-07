//================================================
// @brief ボスアクションの基底クラス
//================================================
#pragma once

class Boss;
class BossAction abstract
{
public:
	BossAction(){}
	virtual ~BossAction(){}

	virtual void Initialize	() abstract;
	virtual void Finalize	() abstract;
	virtual void Update		(Boss* _boss) abstract;

protected:
};

