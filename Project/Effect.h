//========================================
// @brief エフェクトベースクラス
//========================================
#pragma once

class Transform;
class Effect abstract
{
public:
	Effect(const int _effectResourceHandle);//コンストラクタ
	virtual ~Effect();//デストラクタ

	void Initialize(); 
	void Update();//
	const void Draw()const;
	void OnIsPlayEffect() { this->isPlayEffect = true; }
	void SetPosition(const VECTOR _position);
	
	/*純粋仮想関数*/
	virtual void SetInfoToPlayTheEffect() abstract;
protected:
	const VECTOR Convert(std::vector<float> _in)const;

	int effectResourceHandle;
	vector<int> playingEffectHandle;
	int frameCount;
	bool isPlayEffect;//エフェクトを再生するか
	int startFrame;
	int endFrame;
	int firstFrame;
	Transform* transform;
	
};

