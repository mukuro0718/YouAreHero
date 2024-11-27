//=============================================
// @brief VECTOR To Useful
// 構造体VECTORを使いやすいように、機能を追加する
//=============================================
#pragma once

namespace Gori
{
	static constexpr VECTOR ORIGIN = { 0.0f,0.0f,0.0f };
	static constexpr VECTOR UP_VEC = { 0.0f,1.0f,0.0f };
	static constexpr float PI_2 = (2.0f * DX_PI_F);
	inline const VECTOR Convert(const std::vector<float> _value);
	inline const float LerpFloat(const float _start, const float _end, const float _percent)
	{
		return _start + _percent * (_end - _start);
	}
	inline const VECTOR LerpVECTOR(const VECTOR _start, const VECTOR _end, const VECTOR _percent)
	{
		VECTOR out = ORIGIN;
		out.x = LerpFloat(_start.x, _end.x, _percent.x);
		out.y = LerpFloat(_start.y, _end.y, _percent.y);
		out.z = LerpFloat(_start.z, _end.z, _percent.z);
		return out;
	}
	const float LerpAngle(const float _start, const float _end, const float _percent)
	{
		float start = _start;
		float end = _end;
		float out = 0.0f;
		float start_base = _start;
		float end_base = _end;

		//終了地点がマイナスだったら2πを足す
		if (end < 0.0f) end += PI_2;

		/*開始地点と終了地点の差を求める*/
		float diff = start - end;
		//もし差がマイナスだったら-1をかける
		if (diff < 0.0f) diff *= -1.0f;

		/*余りを求める*/
		float excess = PI_2 - diff;

		/*差と余りを求めて、差のほうが大きければBaseのほうで計算*/
		if (diff > excess)
		{
			//スタートベースは360度に変換されているので、π ~ -πの範囲に戻す
			if (_start >= DX_PI_F)
			{
				start_base -= PI_2;
			}
			out = LerpFloat(start_base, end_base, _percent);
		}

		/*余りのほうが大きければそのまま計算*/
		else
		{
			out = LerpFloat(start, end, _percent);
		}

		/*360度の範囲を超えないようにする*/
		if (out <= 0.0f)
		{
			out += PI_2;
		}
		else if (out >= PI_2)
		{
			out -= PI_2;
		}
		return out;
	}
	const VECTOR LerpAngle(const VECTOR _start, const VECTOR _end, const VECTOR _percent)
	{
		VECTOR out;

		out.x = LerpAngle(_start.x, _end.x, _percent.x);
		out.y = LerpAngle(_start.y, _end.y, _percent.y);
		out.z = LerpAngle(_start.z, _end.z, _percent.z);

		VECTOR rotation = VGet(0.0f, out.y, 0.0f);
		return rotation;
	}
}
VECTOR operator +(const VECTOR _a, const VECTOR _b);
VECTOR operator -(const VECTOR _a, const VECTOR _b);
VECTOR operator *(const VECTOR _a, const float _b);
