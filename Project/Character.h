//=============================================================
// @brief キャラクターベースクラス
//=============================================================
#pragma once

class ColliderData;
class CharacterData;
class Rigidbody;
class BitFlag;
class Animation;
class Character abstract
{
public:
	Character();//コンストラクタ
	virtual ~Character();//デストラクタ
	
	virtual void		Initialize		 () abstract;//初期化
	virtual void		Finalize		 () abstract;//後処理
	virtual void		Update			 () abstract;//更新
	virtual const void	DrawCharacterInfo() const abstract;
	const void			Draw			 () const;//描画

	/*getter/setter*/
	virtual const bool			 GetIsAttack		 ()const abstract;
			const CharacterData& GetCharacterData	 ()const;
			const Rigidbody&	 GetRigidbody		 ()const;
			const int			 GetHP				 ()const;
			const int			 GetModelHandle		 ()const { return this->modelHandle; }
			const bool			 GetIsChangeAnimation()const;
			const bool			 GetIsAlive			 ()const { return this->isAlive; }
				  void			 OffIsAlive			 () { this->isAlive = false; }
			const VECTOR		 GetNextRotation()const { return this->nextRotation; }
	/*らーぷ関数*/
	float  Lerp(const float _start, const float _end, const float _percent);	//らーぷ関数
	VECTOR Lerp(const VECTOR _start, const VECTOR _end, const VECTOR _percent);//らーぷ関数
	VECTOR Lerp360Angle(const VECTOR _start, const VECTOR _end, const VECTOR _percent);

	const float GetSpeed()const { return this->speed; }
	void SetRotation(const VECTOR _now, const VECTOR _next);
	void SetSpeed(float _speed) { this->speed = _speed; }
	void SetVelocity(const VECTOR _velocity);

protected:

	/*メンバ変数*/
	Animation*		animation;	//アニメーション
	BitFlag*		state;		//状態
	ColliderData*	collider;	//コライダー
	VECTOR			nextRotation;		//次の回転率
	int				modelHandle;//モデルハンドル
	bool			isAlive;	//キャラクターが生存しているか
	bool			isGround;	//地面に当たっている
	float			speed;		//速度
	int				entryInterval;
	bool isDraw;
	bool isInitialize;
};

