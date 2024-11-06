//=========================================================
// @brief 物理、衝突判定に必要なデータ
//=========================================================
#pragma once

class Rigidbody
{
public:
	Rigidbody();//コンストラクタ
	void Initialize(bool _useGravity = false);//初期化
	/*getter/setter*/
	const	VECTOR& GetPosition	()const { return this->position;	}//座標の取得
	const	VECTOR& GetRotation	()const { return this->rotation;	}//回転率の取得
	const	VECTOR& GetScale	()const { return this->scale;		}//拡大率の取得
	const	VECTOR& GetDirection()const { return this->direction;	}//向きの取得
	const	VECTOR& GetVelocity	()const { return this->velocity;	}//Velocityの取得
			bool	UseGravity	()const { return this->useGravity;	}//重力の取得
	void SetPosition	(const VECTOR& _set) { this->position = _set;	}//座標の設定
	void SetRotation	(const VECTOR& _set) { this->rotation = _set;	}//回転率の設定
	void SetScale		(const VECTOR& _set) { this->scale = _set;		}//拡大率の設定
	void SetUseGravity	(bool _set)			 { this->useGravity = _set; }//重力の設定
	void SetVelocity	(const VECTOR& _set);							 //Velocityの設定
private:
	VECTOR	position;	//座標
	VECTOR  rotation;	//回転率
	VECTOR  scale;		//拡大率
	VECTOR	direction;	//向き
	VECTOR	velocity;	//移動量
	bool	useGravity;	//重力を使用するか
};
