#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include<vector>
#include"Noise.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	
	//プレイヤー
	Player* player_ = nullptr;
	
	//プレイヤーモデル
	Model* model_ = nullptr;
	
	//プレイヤーテクスチャ
	uint32_t textureHandle_ = 0;

	// スプライト
	Sprite* sprite_ = nullptr;

	// ブロックの3Dモデル
	Model* blockModel_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// ブロック用のワールドトランスフォーム。
	// 複数並べるために配列にする
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	//デバックカメラ有効
	bool isDebugCameraActive_ = false;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// 天球の3Dモデル
	Model* modelSkydome_ = nullptr;
	// 天球クラス
	Skydome* skydome_ = nullptr;


	// ブロック1個分の横幅
	const float kBlockWidth = 1.0f;
	const float kBlockHeight = 1.0f;
	// 要素数
	const uint32_t kNumBlockVirtical = 100;
	const uint32_t kNumBlockHorizontal = 100;
	const uint32_t kNumBlockDepth = 10; 

	Noise* noise = nullptr; 

	float perlin_grid[100][100];
	float perlin_grid_size[100][100];

	std::unique_ptr<Model> blockGrass;
	//int Seed = 1;
};
