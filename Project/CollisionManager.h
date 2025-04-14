//===============================================================
// @brief 物理、衝突判定するオブジェクトを登録し、
// 物理移動、衝突を通知する
//===============================================================
#pragma once
#include "Singleton.h"

class ColliderData;
class CollisionManager : public Singleton<CollisionManager>
{
public:
	/*SingletonクラスがColliderManagerにアクセスできるようにする*/
	friend class Singleton<CollisionManager>;

	virtual ~CollisionManager() {}//デストラクタ


	void Entry(ColliderData& _data);	//衝突物の登録
	void Exit(ColliderData& _data);		//登録解除
	void Update();						//更新(登録オブジェクトの物理移動、衝突通知)
private:
	/*静的定数*/
	static constexpr float	GRAVITY					= -0.098f;	//重力
	static constexpr float	MAX_GRAVITY_ACCEL		= -1.5f;	//最大重力加速度
	static constexpr int	BEFORE_FIX_INFO_COLOR	= 0x0000ff;	//補正前情報色
	static constexpr int	AIM_INFO_COLOR			= 0x0000aa;	//補正前予定情報色
	static constexpr int	AFTER_FIX_INFO_COLOR	= 0xff00ff;	//補正後情報色
	static constexpr int	DIV_NUM					= 16;		//デバック表示図形分割数
	static constexpr int	MAX_CHECK_COUNT			= 1000;		//当たり判定を確認する最大回数
	static constexpr float  HIT_SLIDE_LENGTH		= 0.1f;
	static constexpr int	HIT_TRY_NUM				= 4;
	static constexpr int	MAX_HIT_POLY			= 1000;
	static constexpr VECTOR FLOOR_NORM				= { 0.0f,1.0f,0.0f };
	static constexpr VECTOR X_PLUS_NORM				= { 1.0f,0.0f,0.0f };
	static constexpr VECTOR X_MINUS_NORM			= { -1.0f,0.0f,0.0f };
	static constexpr VECTOR Z_PLUS_NORM				= { 0.0f,0.0f,1.0f };
	static constexpr VECTOR Z_MINUS_NORM			= { 0.0f,0.0f,-1.0f };
	static constexpr float  MAX_DEGREE				= 60.0f;

	/*内部処理関数*/
			CollisionManager() {}												//コンストラクタ
	void	CheckColide		();													//当たり判定チェック
	bool	IsCollide		(ColliderData& _objectA, ColliderData& _objectB);	//当たったか
	void	FixNextPosition	(ColliderData& _primary, ColliderData& _secondary);	//次の座標を補正
	void	FixPosition		();													//座標確定
	float	GetDegree		(const VECTOR _norm1, const VECTOR _norm2);
	void	JudgeNorm		(const MV1_COLL_RESULT_POLY_DIM _hitPolyDim, vector<MV1_COLL_RESULT_POLY>& _floor, vector<MV1_COLL_RESULT_POLY>& _xpWall, vector<MV1_COLL_RESULT_POLY>& _xmWall, vector<MV1_COLL_RESULT_POLY>& _zpWall, vector<MV1_COLL_RESULT_POLY>& _zmWall, const VECTOR _position);
	bool	JudgeDegree		(const VECTOR _norm1, const VECTOR _norm2);
	float	JudgeMax		(const float _nowMax, const float _judgeValue);
	bool	HitCheckWall	(float& _max, vector<MV1_COLL_RESULT_POLY> _wall, const VECTOR _pos, const float _height, const float _radius, const bool _sign, const bool isX, const bool isZ);
	void CheckHitXWall(vector<MV1_COLL_RESULT_POLY> _xpWall, vector<MV1_COLL_RESULT_POLY> _xmWall, bool& _isHit, VECTOR& _maxFix, float _hitHeight, float _radius, VECTOR _nowNextPosition, VECTOR& _newNextPosition, VECTOR _moveVector);
	void CheckHitZWall(vector<MV1_COLL_RESULT_POLY> _xpWall, vector<MV1_COLL_RESULT_POLY> _xmWall, bool& _isHit, VECTOR& _maxFix, float _hitHeight, float _radius, VECTOR _nowNextPosition, VECTOR& _newNextPosition, VECTOR _moveVector);

	/*メンバ変数*/
	std::list<ColliderData*> collidables;//登録されたCollidableのリスト
	int frameTime;//処理までにかかった時間
	enum class HitCheckType
	{
		SAME_CHARA,
		CHARA_ATTACK_S,
		CHARA_ATTACK_C,
		CHARA_PLANE,
		CHARA_MODEL,
	};
	std::list<HitCheckType> hitCheckLog;
	std::vector<std::vector<bool>> alreadyProcessed;
};

