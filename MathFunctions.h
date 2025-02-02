#pragma once
#define _USE_MATH_DEFINES
#include<cmath>
#include<assert.h>
#include<Matrix4x4.h>
#include "Vector3.h"


float DegreesToRadians(float degrees);

Vector3 Add(const Vector3& v1, const Vector3& v2);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);

float Dot(const Vector3& v1, const Vector3& v2);

float Length(const Vector3& v);
float Length(const float& v);
Vector3 Nomalize(const Vector3& v);
float Nomalize(const float& v);
float Clanp(float t);
Vector3 Multiply(const Vector3& v1, const Vector3& v2);
////線形補完
float Lerp(float a, float b, float t);
// イージング
float EaseOut(float t, float str, float end);
// 行列の積
Matrix4x4 Multiply(const Matrix4x4& v1, const Matrix4x4& v2);
Vector3 Multiply(const Vector3& v1, const float& v2);
// 移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// 回転行列X
Matrix4x4 MakeRotateXMatrix(float rotate);
// 回転行列Y
Matrix4x4 MakeRotateYMatrix(float rotate);
// 回転行列Z
Matrix4x4 MakeRotateZMatrix(float rotate);
// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
//
Matrix4x4 Identity();
// アフィン変換
Matrix4x4 MakeAffineMatrixMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
// 透視射影行列
Matrix4x4 MakePerspectiveFovMatrix(float forY, float aspectRatio, float nearClip, float farClip);
// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float leht, float top, float width, float height, float minDepth, float maxDepth);
// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
// TransformNormal//ベクトル変換
Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix);
//
