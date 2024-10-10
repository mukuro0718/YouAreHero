#pragma once
class CharacterData abstract
{
public:
	CharacterData();
	virtual ~CharacterData(){}

	int hp;//HP
	bool isHit;//UŒ‚‚Éƒqƒbƒg‚µ‚½‚©
	VECTOR hitPosition;
	bool isGuard;
	bool isInvinvible;
	int playerReaction;
	int hitStopTime;
	int hitStopType;
	int hitStopDelay;
	float slowFactor;
};

