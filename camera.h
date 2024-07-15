#pragma once
#include "MathFunctions.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Plane {
public:
	float a, b, c, d;

	Plane() : a(0), b(0), c(0), d(0) {}

	void Normalize() {
		float length = sqrtf(a * a + b * b + c * c);
		a /= length;
		b /= length;
		c /= length;
		d /= length;
	}
};

class Frustum {
public:
	Plane planes[6];

	void ConstructFrustum(float screenDepth, ViewProjection* viewProjection) {
		// プロジェクション行列とビュー行列の掛け算
		Matrix4x4 matrix;
		matrix = Multiply(viewProjection->matView, viewProjection->matProjection);

		// 視錐台の遠近クリッピング平面の更新
		viewProjection->farZ = screenDepth;

		// Near plane
		planes[0].a = matrix.m[0][3] + matrix.m[0][2];
		planes[0].b = matrix.m[1][3] + matrix.m[1][2];
		planes[0].c = matrix.m[2][3] + matrix.m[2][2];
		planes[0].d = matrix.m[3][3] + matrix.m[3][2];
		planes[0].Normalize();

		// Far plane
		planes[1].a = matrix.m[0][3] - matrix.m[0][2];
		planes[1].b = matrix.m[1][3] - matrix.m[1][2];
		planes[1].c = matrix.m[2][3] - matrix.m[2][2];
		planes[1].d = matrix.m[3][3] - matrix.m[3][2];
		planes[1].Normalize();

		// Left plane
		planes[2].a = matrix.m[0][3] + matrix.m[0][0];
		planes[2].b = matrix.m[1][3] + matrix.m[1][0];
		planes[2].c = matrix.m[2][3] + matrix.m[2][0];
		planes[2].d = matrix.m[3][3] + matrix.m[3][0];
		planes[2].Normalize();

		// Right plane
		planes[3].a = matrix.m[0][3] - matrix.m[0][0];
		planes[3].b = matrix.m[1][3] - matrix.m[1][0];
		planes[3].c = matrix.m[2][3] - matrix.m[2][0];
		planes[3].d = matrix.m[3][3] - matrix.m[3][0];
		planes[3].Normalize();

		// Top plane
		planes[4].a = matrix.m[0][3] - matrix.m[0][1];
		planes[4].b = matrix.m[1][3] - matrix.m[1][1];
		planes[4].c = matrix.m[2][3] - matrix.m[2][1];
		planes[4].d = matrix.m[3][3] - matrix.m[3][1];
		planes[4].Normalize();

		// Bottom plane
		planes[5].a = matrix.m[0][3] + matrix.m[0][1];
		planes[5].b = matrix.m[1][3] + matrix.m[1][1];
		planes[5].c = matrix.m[2][3] + matrix.m[2][1];
		planes[5].d = matrix.m[3][3] + matrix.m[3][1];
		planes[5].Normalize();
	}

	bool CheckCube(WorldTransform* worldTransform, float radius) {
		for (int i = 0; i < 6; i++) {
			if (planes[i].a * (worldTransform->translation_.x - radius) + planes[i].b * (worldTransform->translation_.y - radius) + planes[i].c * (worldTransform->translation_.z - radius) +
			        planes[i].d >
			    0.0f) {
				continue;
			}
			if (planes[i].a * (worldTransform->translation_.x + radius) + planes[i].b * (worldTransform->translation_.y - radius) + planes[i].c * (worldTransform->translation_.z - radius) +
			        planes[i].d >
			    0.0f) {
				continue;
			}
			if (planes[i].a * (worldTransform->translation_.x - radius) + planes[i].b * (worldTransform->translation_.y + radius) + planes[i].c * (worldTransform->translation_.z - radius) +
			        planes[i].d >
			    0.0f) {
				continue;
			}
			if (planes[i].a * (worldTransform->translation_.x + radius) + planes[i].b * (worldTransform->translation_.y + radius) + planes[i].c * (worldTransform->translation_.z - radius) +
			        planes[i].d >
			    0.0f) {
				continue;
			}
			if (planes[i].a * (worldTransform->translation_.x - radius) + planes[i].b * (worldTransform->translation_.y - radius) + planes[i].c * (worldTransform->translation_.z + radius) +
			        planes[i].d >
			    0.0f) {
				continue;
			}
			if (planes[i].a * (worldTransform->translation_.x + radius) + planes[i].b * (worldTransform->translation_.y - radius) + planes[i].c * (worldTransform->translation_.z + radius) +
			        planes[i].d >
			    0.0f) {
				continue;
			}
			if (planes[i].a * (worldTransform->translation_.x - radius) + planes[i].b * (worldTransform->translation_.y + radius) + planes[i].c * (worldTransform->translation_.z + radius) +
			        planes[i].d >
			    0.0f) {
				continue;
			}
			if (planes[i].a * (worldTransform->translation_.x + radius) + planes[i].b * (worldTransform->translation_.y + radius) + planes[i].c * (worldTransform->translation_.z + radius) +
			        planes[i].d >
			    0.0f) {
				continue;
			}

			return false;
		}

		return true;
	}
};