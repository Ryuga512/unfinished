//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_MAX		(1)		// エネミーのMax人数
#define BOSS_HP_MAX     (1000.0f)
#define OTHER_DEFAULT               (0)
#define OTHER_FRONT                 (1)
#define OTHER_SIDE                  (2)
#define OTHER_ROLLING               (3)


//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct ENEMY
{
	bool			use;			// true:使っている  false:未使用
	float			w, h;			// 幅と高さ
	D3DXVECTOR3		pos;			// ポリゴンの座標
	D3DXVECTOR3		rot;			// ポリゴンの回転量
    D3DXVECTOR3     p_pos;
	int				countAnim;		// アニメーションカウント
	int				patternAnim;	// アニメーションパターンナンバー
	int				texNo;			// テクスチャ番号
	bool			left;			// true:左へ移動
	bool			up;				// true:上へ移動
    float           hp;             // 体力
    bool            boss;           // ボスかどうか
    int             atk_delay;      // 攻撃準備
    float           r;              // レッド
    float           g;              // グリーン
    float           b;              // ブルー
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY *GetEnemy_p(void);


