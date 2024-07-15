#include"Noise.h"

// この関数は、入力0 <= t <= 1に対して、0から1までの値の間でスムージングを行います。このスムージング効果により、生成されるパーリンノイズがより自然で滑らかになります。
float Noise::fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }
// 3次のスムージングカーブ
float Noise::fade3Dimension(float t) { return t * (t * (t * 6 - 15) + 10); };
// 4次のスムージングカーブ
float Noise::fade4Dimension(float t) { return t * t * (t * (t * 6 - 15) + 10); };
// 6次のスムージングカーブ
float Noise::fade6Dimension(float t) { return t * t * t * t * (t * (t * 6 - 15) + 10); };
// イーズイン・イーズアウトカーブ
float Noise::fadeInOut(float t) { return t * t * (3 - 2 * t); };
// バウンスカーブ（弾性カーブ）
float Noise::fadeElasticity(float t) { return t * sin(t * 3.14159f); };
// シグモイド関数
float Noise::fadeSigmoid(float t) { return t / (1 + exp(-t)); };
// カスタムカーブ（例: 4次のスムージングカーブ）
float Noise::fadeCustom(float t) {return t * t * t * (t * (t * 8 - 20) + 15);};

// 線形補間（Linear Interpolation）を行うための関数です。線形補間は、2つの値の間を滑らかに移動するための方法
float Noise::lerp(float t, float a, float b) { return a + t * (b - a); }

// ハッシュ値に基づいて座標に対する勾配ベクトルを計算します。これは、パーリンノイズ生成アルゴリズムにおいて、各格子点に対するランダムな勾配ベクトルを生成するために使用されます。
// 勾配ベクトル計算
float Noise::grad(int hash, float x, float y) {
	int h = hash & 15;
	float u = h < 8 ? x : y;
	float v = h < 4 ? y : h == 12 || h == 14 ? x : 0;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float Noise::grad2(int hash, float x, float y) {
	int h = hash & 255;
	float u = x, v = y;

	h = h * h * (h * 15731 + 789221) + 1376312589;
	h = h & 255;

	if (h < 64) {
		u = x;
		v = y;
	} else if (h < 128) {
		u = -x;
		v = y;
	} else if (h < 192) {
		u = x;
		v = -y;
	} else {
		u = -x;
		v = -y;
	}

	return u + v;
}

float Noise::grad3(int hash, float x, float y) {
	int h = hash & 15;
	float u, v;

	switch (h) {
	case 0:
		u = x;
		v = y;
		break;
	case 1:
		u = -x;
		v = y;
		break;
	case 2:
		u = x;
		v = -y;
		break;
	case 3:
		u = -x;
		v = -y;
		break;
	case 4:
		u = y;
		v = x;
		break;
	case 5:
		u = -y;
		v = x;
		break;
	case 6:
		u = y;
		v = -x;
		break;
	case 7:
		u = -y;
		v = -x;
		break;
	case 8:
		u = x + y;
		v = x - y;
		break;
	case 9:
		u = -x + y;
		v = -x - y;
		break;
	case 10:
		u = x - y;
		v = x + y;
		break;
	case 11:
		u = -x - y;
		v = -x + y;
		break;
	case 12:
		u = y - x;
		v = y + x;
		break;
	case 13:
		u = -y - x;
		v = -y + x;
		break;
	case 14:
		u = y + x;
		v = y - x;
		break;
	case 15:
		u = -y + x;
		v = -y - x;
		break;
	default:
		u = 0;
		v = 0;
		break;
	}

	return u + v;
} // ハッシュ関数
void Noise::init_hash(int seed) {
	srand(seed);
	for (int i = 0; i < HASH_SIZE; i++) {
		hashT[i] = rand() % HASH_SIZE;
	}
}

// 特定の座標におけるパーリンノイズの値を計算します。パーリンノイズは、特定の座標におけるランダムな値の集合であり、これを補間することで滑らかなノイズが生成されます。
float Noise::noise(float x, float y) {
	int X = (int)floor(x); // &Noise;
	int Y = (int)floor(y); // &255;
	x -= floor(x);
	y -= floor(y);
	float u = fade(x);
	float v = fade(y);
	float n00 = grad(X, x, y);
	float n01 = grad(X, x, y - 1);
	float n10 = grad(Y - 1, x - 1, y);
	float n11 = grad(Y - 1, x - 1, y - 1);
	float n0 = lerp(u, n00, n01);
	float n1 = lerp(u, n10, n11);
	return lerp(v, n0, n1);
}

// シード値を利用
//  パーリンノイズの基本ノイズ関数
//  2次元のパーリンノイズ
float Noise::noise2(float x, float y) {
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;
	x -= floor(x);
	y -= floor(y);
	// 補間をスムーズにするためのカーブを適用
	float u = fade(x);
	float v = fade(y);
	
	// ハッシュテーブルの参照
	int A = hashT[X] + Y;
	int B = hashT[X + 1] + Y;
	// グラディエントベクトルとドット積の計算
	// grad関数を使って、各グリッド点での勾配ベクトルを計算
	float n00 = grad3(hashT[A], x, y);
	float n01 = grad3(hashT[A + 1], x, y - 1);
	float n10 = grad3(hashT[B], x - 1, y);
	float n11 = grad3(hashT[B + 1], x - 1, y - 1);
	// 線形補間
	float n0 = lerp(u, n00, n01);
	float n1 = lerp(u, n10, n11);
	return lerp(v, n0, n1);
}

// 特定の座標におけるパーリンノイズの値を計算するための関数です。この関数は、複数のオクターブを組み合わせて生成されたパーリンノイズを返します。
float Noise::perlin_noise(float x, float y) {
	float total = 0;
	float frequency = Frequency;
	float amplitude = Amplitude;
	float max_value = Max_value;

	for (int i = 0; i < OCTAVES; ++i) {
		total += noise2(x * frequency, y * frequency) * amplitude;
		max_value += amplitude;
		amplitude *= PERSISTENCE;
		frequency *= 2;
	}
	return total * 40 / max_value;
}
float Noise::perlin_noise2(float x, float y) {
	float total = 0;
	float frequency = Frequency;
	float amplitude = Amplitude;
	float max_value = 0;

	for (int i = 0; i < OCTAVES; ++i) {
		total += noise2(x, y) * amplitude;
		max_value += amplitude;
		x *= PERSISTENCE;
		y *= PERSISTENCE;
		frequency *= 2;
	}
	return total * 40/ max_value;
}



float Noise::neighbor_deviation(float noise_center, float noise) {
	
	// 偏差の計算
	float deviation = std::abs(noise_center - noise);
	// 偏差の平均を返す
	return deviation; //(deviation_right + deviation_left + deviation_up + deviation_down) / 4.0f;
}


void Noise::NoiseImGui() {

	
	// ハッシュテーブルの初期化
	int seed = Seed; // 任意のシード値
	init_hash(seed);

	ImGui::Begin("noiseClass");
	ImGui::DragFloat("Amplitude ", &Amplitude,0.1f);
	ImGui::DragFloat("Max_value ", &Max_value);
	//ImGui::DragFloat("PERSISTENCE ", &PERSISTENCE,0.0001f);
	//ImGui::DragInt("OCTAVES ", &OCTAVES);
	//ImGui::DragFloat("Frequency ", &Frequency);
	ImGui::DragInt("Seed ", &Seed);
	ImGui::End();


}


