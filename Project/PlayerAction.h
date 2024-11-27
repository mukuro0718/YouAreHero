//===================================================
// @プレイヤーアクションベースクラス
//===================================================
#pragma once

class Player;
class PlayerAction abstract
{
public:
			 PlayerAction(bool _isChangeAction = false) :isChangeAction(_isChangeAction) {}//コンストラクタ
	virtual ~PlayerAction()																 {}//デストラクタ
	virtual void Initialize	() abstract;				//初期化
	virtual void Finalize	() abstract;				//後処理
	virtual void Update		(Player& _player) abstract;	//更新
	const bool GetIsChangeAction()const { return this->isChangeAction; }//アクション変更フラグの取得
protected:
	void UpdateRotation	(VECTOR& _nextRotation, VECTOR _nowRotation);
	void UpdateSpeed	(float& _nowSpeed, const float _maxSpeed, const VECTOR nowRotation, const VECTOR _nextRotation);
	VECTOR UpdateVelocity	(const VECTOR _rotation, const VECTOR _prevVelocity, const float _speed, const bool _isLerp);
	bool isChangeAction;//アクションの変更ができる状態になればこのフラグを立てる

};

