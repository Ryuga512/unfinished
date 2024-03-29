//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EFFECT_MAX		(5)		// エフェクトのMax人数



//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct EFFECT
{
    bool			use;			// true:使っている  false:未使用
    float			w, h;			// 幅と高さ
    D3DXVECTOR3		pos;			// ポリゴンの座標
    D3DXVECTOR3		rot;			// ポリゴンの回転量
    int				countAnim;		// アニメーションカウント
    int				patternAnim;	// アニメーションパターンナンバー
    int				texNo;			// テクスチャ番号
    float           alpha;          // アルファ
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect(void);
void UninitEffect(void); 
void UpdateEffect(D3DXVECTOR3 PosArg, int i, bool ans);
void DrawEffect(void);

EFFECT *GetEffect_p(void);


#pragma once
