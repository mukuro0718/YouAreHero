#pragma once

class Character;
class Rigidbody;
class BossAction;
class BossChase : public BossAction
{
public:
	BossChase();
	virtual ~BossChase();

	void		Initialize	 ()					override;
	void		Finalize	 ()					override;
	Rigidbody&	Update		 (Character& _boss) override;
	void		CalcParameter(const int _hp, const int _angryValue, const float _distance)					override;

private:
	
};

