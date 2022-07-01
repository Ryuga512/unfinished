//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MAX		(1)		// プレイヤーのMax人数

#define UP                          (0)
#define DOWN                        (1)
#define RIGHT                       (2)
#define LEFT                        (3)
#define ATACK_DELAY                 (64)    // エフェクトのアニメ枚数10*3(ループ数)*2(一枚描画するのに必要な時間)


//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct PLAYER
{
	bool			use;			// true:使っている  false:未使用
	float			w, h;			// 幅と高さ
	D3DXVECTOR3		pos;			// ポリゴンの座標
	D3DXVECTOR3		rot;			// ポリゴンの回転量
    D3DXVECTOR3     pre_pos;        // 前の座標
	int				countAnim;		// アニメーションカウント
	int				patternAnim;	// アニメーションパターンナンバー
	int				texNo;			// テクスチャ番号
    int             dir;            // 向いてる方向
    bool            hit[4];         // 衝突判定
    bool            in_attack;      // 攻撃中か
    int             attack;         // 攻撃力
    float           hp;             // 体力
    float           atk_h;
    float           atk_w;
    int             atk_delay;
    float           pre_tx;
    int             pre_texNo;
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer_p(void);


