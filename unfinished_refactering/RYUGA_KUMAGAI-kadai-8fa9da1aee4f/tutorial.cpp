//=============================================================================
//
// ゲーム画面処理 [tutorial.cpp]
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
HRESULT InitTutorial(void)
{
    InitHp();
    // オブジェクトの初期化処理
    InitObject();

    // プレイヤーの初期化処理
    InitPlayer();

    // BGの初期化処理
    InitBg();

    // エフェクトの初期化処理
    InitEffect();

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTutorial(void)
{
    // エフェクトの終了処理
    UninitEffect();

    // BGの終了処理
    UninitBg();

    // プレイヤーの終了処理
    UninitPlayer();

    //オブジェクトの終了処理
    UninitObject();

    UninitHp();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTutorial(void)
{
    // オブジェクトの更新処理
    UpdateObject();

    // プレイヤーの更新処理
    UpdatePlayer();

    // BGの更新処理
    UpdateBg();

    HitCheck();

    UpdateHp();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTutorial(void)
{

    // BGの描画処理
    DrawBg();

    // オブジェクトの描画処理
    DrawObject();

    // プレイヤーの描画処理
    DrawPlayer();

    // エフェクトの描画処理
    DrawEffect();

    DrawHp();

}
