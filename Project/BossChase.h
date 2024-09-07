#pragma once

class BossAction;
class BossChase : public BossAction
{
public:
	BossChase();
	virtual ~BossChase();

	void Initialize	() override;
	void Finalize	() override;
	void Update		(Boss* _boss) override;
private:
	
};

