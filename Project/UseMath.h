#pragma once
#include <math.h>

namespace GORI
{
	VECTOR GetClosestPointOnSegment(VECTOR _point, VECTOR _start, VECTOR _end)
	{
		/*最近接点がstart側線分外領域の場合*/
		VECTOR startToPoint = VSub(_point, _start);
		VECTOR startToEnd = VSub(_end, _start);
		VECTOR startToEndNorm = VNorm(startToEnd);
		//内積の結果がマイナスの値だったらstartが最近接点になる
		if (VDot(startToPoint, startToEndNorm) < 0)
		{
			return _start;
		}

		/*最近接点がstart側線分外領域の場合*/
		VECTOR endToPoint = VSub(_point, _end);
		VECTOR endToStart = VSub(_start, _end);
		VECTOR endToStartNorm = VNorm(endToStart);
		//内積の結果がマイナスの値だったらendが最近接点になる
		if (VDot(endToPoint, endToStartNorm) < 0)
		{
			return _start;
		}
		//中間領域の場合
		else
		{
			float t = VDot(startToEndNorm, startToPoint);
			return VAdd(_start, VScale(startToEndNorm, t));
		}
	}
}