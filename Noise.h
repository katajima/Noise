#pragma once
#include <math.h>
#include<imgui.h>
#include"MathFunctions.h"
#include<random>
#include<time.h>
#include <numeric>

class Noise {
private:
	// この関数は、入力0 <= t <= 1に対して、0から1までの値の間でスムージングを行います。このスムージング効果により、生成されるパーリンノイズがより自然で滑らかになります。
	float fade(float t);
	// 線形補間（Linear Interpolation）を行うための関数です。線形補間は、2つの値の間を滑らかに移動するための方法
	float lerp(float t, float a, float b);

	// ハッシュ値に基づいて座標に対する勾配ベクトルを計算します。これは、パーリンノイズ生成アルゴリズムにおいて、各格子点に対するランダムな勾配ベクトルを生成するために使用されます。
	float grad(int hash, float x, float y);

	// ハッシュ関数
	void init_hash(int seed);

	// 特定の座標におけるパーリンノイズの値を計算します。パーリンノイズは、特定の座標におけるランダムな値の集合であり、これを補間することで滑らかなノイズが生成されます。
	float noise(float x, float y);

	// シード値を利用
	//  パーリンノイズの基本ノイズ関数
	//  2次元のパーリンノイズ
	float noise2(float x, float y);


	public:
	// 特定の座標におけるパーリンノイズの値を計算するための関数です。この関数は、複数のオクターブを組み合わせて生成されたパーリンノイズを返します。
	float perlin_noise(float x, float y);

	void NoiseImGui();

	float neighbor_deviation(float noise_center, float noise);
	

private:
	float PERSISTENCE = 0.5f; // 揺れ幅

	int OCTAVES = 1; // 多くのオクターブ設定すると 生成されるノイズはより複雑になります。

	float Amplitude = 1;

	float Max_value = 0;

	float Frequency = 1;

	int Seed = 1;

#define HASH_SIZE 512
	// ハッシュテーブルの初期化
	int hashT[HASH_SIZE];
};



