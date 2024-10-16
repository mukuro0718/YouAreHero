#pragma once
#include "Singleton.h"

class Effect;
class EffectManager : public Singleton<EffectManager>
{
public:
	friend class Singleton<EffectManager>;
	/*列挙体*/
	enum class EffectType
	{
		BOSS_IMPACT,
		PLAYER_GUARD_HIT,
		PLAYER_HEAL,
		PLAYER_IMPACT,
		PLAYER_JUST_GUARD,
	};
	/*エフェクトの描画用関数*/
	void OnIsEffect(const EffectType _type);
	void SetPosition(const EffectType _type, const VECTOR _position);
	void Initialize();
	void Update();
	const void Draw()const;
private:
	/*静的定数*/
	static constexpr int PLAYER_EFFECT = 1;
	/*内部処理関数*/
	EffectManager();//コンストラクタ
	~EffectManager();//デストラクタ
	const VECTOR Convert(std::vector<float> _in)const;
	

	/*メンバ変数*/
	std::vector<Effect*> effect;
};

