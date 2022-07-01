//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================

#include "main.h"
#include "game.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"

#include "player.h"
#include "enemy.h"
#include "attack.h"
#include "bg.h"
#include "fade.h"
#include "sound.h"
#include "object.h"
#include "effect.h"
#include "hp.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
    // hpの初期化処理
    InitHp();

    // オブジェクトの初期化処理
    InitObject();

	// プレイヤーの初期化処理
	InitPlayer();

	// エネミーの初期化処理
	InitEnemy();

	// バレットの初期化処理
	InitBullet();

	// BGの初期化処理
	InitBg();

    // エフェクトの初期化処理
    InitEffect();

	// BGM再生
	PlaySound(SOUND_LABEL_BGM_sample001);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
    // エフェクトの終了処理
    UninitEffect();

	// BGの終了処理
	UninitBg();

	// バレットの終了処理
	UninitBullet();

	// エネミーの終了処理
	UninitEnemy();

	// プレイヤーの終了処理
	UninitPlayer();

    //オブジェクトの終了処理
    UninitObject();

    // エフェクトの終了処理
    UninitHp();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
    // オブジェクトの更新処理
    UpdateObject();

	// プレイヤーの更新処理
	UpdatePlayer();

	// エネミーの更新処理
	UpdateEnemy();

	// バレットの更新処理
	UpdateBullet();

	// BGの更新処理
	UpdateBg();

	// 当たり判定
    HitCheck();

    //hpの更新処理
    UpdateHp();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{
	// BGの描画処理
	DrawBg();

	// エネミーの描画処理
	DrawEnemy();

	// バレットの描画処理
	DrawBullet();

    // オブジェクトの描画処理
    DrawObject();

	// プレイヤーの描画処理
	DrawPlayer();

    // エフェクトの描画処理
    DrawEffect();

    // hpの描画処理
    DrawHp();
}

