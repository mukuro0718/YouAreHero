#pragma once
#include "Singleton.h"

class EffectManager : public Singleton<EffectManager>
{
public:
	friend class Singleton<EffectManager>;

	/*エフェクトの描画用関数*/
	void AddEffect();
	void RemoveEffect();
	void OnIsPlayEffect(const int _in){}

	void Initialize();
	void Finalize();

private:
	/*内部処理関数*/
	EffectManager();//コンストラクタ
	~EffectManager();//デストラクタ

	/*メンバ変数*/
};

