//=============================================
// @brief VECTOR To Useful
// 構造体VECTORを使いやすいように、機能を追加する
//=============================================
#pragma once

const VECTOR Convert(const std::vector<float> _value);
VECTOR operator +(const VECTOR _a, const VECTOR _b);
VECTOR operator -(const VECTOR _a, const VECTOR _b);
VECTOR operator *(const VECTOR _a, const float _b);
