//=========================================================
// @brief 物理、衝突判定に必要なデータ
//=========================================================
#pragma once

namespace GoriLib
{
	class Collidable;
	class Rigidbody
	{
	public:
		Rigidbody();//コンストラクタ
		void Initialize(bool _useGravity = false);//初期化

		/*getter/setter*/
		const	VECTOR& GetPosition	()const { return this->position; }
		const	VECTOR& GetRotation	()const { return this->rotation; }
		const	VECTOR& GetScale	()const { return this->scale; }
		const	VECTOR& GetDirection()const { return this->direction; }
		const	VECTOR& GetVelocity	()const { return this->velocity; }
				bool	UseGravity	()const { return this->useGravity; }

		void SetPosition	(const VECTOR& _set) { this->position = _set; }
		void SetRotation	(const VECTOR& _set) { this->rotation = _set; }
		void SetScale		(const VECTOR& _set) { this->scale = _set; }
		void SetVelocity	(const VECTOR& _set);
		void SetUseGravity	(bool _set)			 { this->useGravity = _set; }
	private:
		VECTOR	position;	//座標
		VECTOR  rotation;	//回転率
		VECTOR  scale;		//拡大率
		VECTOR	direction;	//向き
		VECTOR	velocity;	//移動量
		bool	useGravity;	//重力を使用するか
	};

}