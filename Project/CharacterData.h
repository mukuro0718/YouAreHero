#pragma once

class CharacterData abstract
{
public:
	CharacterData();
	virtual ~CharacterData(){}

	int hp;//HP
	bool isHit;//UŒ‚‚Éƒqƒbƒg‚µ‚½‚©
	VECTOR hitPosition;
	bool isCutDamage;
};

