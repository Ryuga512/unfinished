#pragma once
//=============================================================================
//
// 木処理 [Bullet.h]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================

#define	MAX_BULLET			(10)			// 弾最大数

typedef struct
{
    D3DXMATRIX	mtxWorld;		// ワールドマトリックス
    D3DXVECTOR3 pos;			// 位置
    D3DXVECTOR3 scl;			// スケール
    MATERIAL	material;		// マテリアル
    float		fWidth;			// 幅
    float		fHeight;		// 高さ
    int			nIdxShadow;		// 影ID
    bool		bUse;			// 使用しているかどうか
    float       speed;
    float       radius;
    D3DXVECTOR3 rot;
} BULLET;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

//弾発射
int SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
BULLET *GetBullet(void);

