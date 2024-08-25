#include <DxLib.h>
#include "UseSTL.h"
#include "Transform.h"
#include "DeleteInstance.h"
#include "Animation.h"
#include "Model.h"

/// <summary>
/// コンストラクタ
/// </summary>
Model::Model(const int _modelHandle)
	: modelHandle(-1)
	, transform(nullptr)
{
	this->transform = new Transform();
	this->animation = new Animation();
	this->modelHandle = MV1DuplicateModel(_modelHandle);
}

/// <summary>
/// デストラクタ
/// </summary>
Model::~Model()
{
	MV1DeleteModel(this->modelHandle);
	DeleteMemberInstance(this->transform);
	DeleteMemberInstance(this->animation);
}

/// <summary>
/// 座標の取得
/// </summary>
const VECTOR Model::GetPosition()const
{
	return this->transform->GetPosition();
}

/// <summary>
/// 回転率の取得
/// </summary>
const VECTOR Model::GetRotation()const
{
	return this->transform->GetRotation();
}

/// <summary>
/// 拡大率の取得
/// </summary>
const VECTOR Model::GetScale()const
{
	return this->transform->GetScale();
}

/// <summary>
/// 座標の設定
/// </summary>
void Model::SetPosition(const VECTOR _position)
{
	this->transform->SetPosition(_position);
	MV1SetPosition(this->modelHandle, this->transform->GetPosition());
}

/// <summary>
/// 回転率の設定
/// </summary>
void Model::SetRotation(const VECTOR _rotation)
{
	this->transform->SetRotation(_rotation);
	MV1SetRotationXYZ(this->modelHandle, this->transform->GetRotation());
}
void Model::SetRotationOfRadian(const VECTOR _rotation)
{
	this->transform->SetRotation(_rotation);
	MV1SetRotationXYZ(this->modelHandle, this->transform->GetRotation());
}

/// <summary>
/// 拡大率の設定
/// </summary>
void Model::SetScale(const VECTOR _scale)
{
	this->transform->SetScale(_scale);
	MV1SetScale(this->modelHandle, this->transform->GetScale());
}

/// <summary>
/// トランスフォームの設定
/// </summary>
void Model::SetTransform(const VECTOR _position, const VECTOR _rotation, const VECTOR _scale)
{
	this->transform->SetPosition(_position);
	this->transform->SetRotation(_rotation);
	this->transform->SetScale	(_scale);

	MV1SetPosition	 (this->modelHandle, this->transform->GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->transform->GetRotation());
	MV1SetScale		 (this->modelHandle, this->transform->GetScale());

}

/// <summary>
/// 描画
/// </summary>
const void Model::Draw()const
{
	MV1DrawModel(this->modelHandle);
}

/// <summary>
/// アニメーションの追加
/// </summary>
void Model::AddAnimation(const std::vector<string> _animationHandle, const std::vector<int> _animationIndex)
{
	/*アニメーションの追加*/
	for (int i = 0; i < _animationIndex.size(); i++)
	{
		int animationHandle = MV1LoadModel(_animationHandle[i].c_str());
		this->animation->Add(animationHandle, _animationIndex[i]);
	}

	/*アニメーションのアタッチ*/
	this->animation->Attach(&this->modelHandle);
}

/// <summary>
/// アニメーションの再生
/// </summary>
void Model::PlayAnimation(const int _nowAnimation, const float _animationPlayTime)
{
	VECTOR position = this->transform->GetPosition();
	this->animation->Play(&this->modelHandle, position,_nowAnimation, _animationPlayTime);
	this->transform->SetPosition(position);
}

/// <summary>
/// アニメーション変更許可フラグの取得
/// </summary>
const bool Model::GetIsChangeAnim() const
{
	return this->animation->GetIsChangeAnim();
}