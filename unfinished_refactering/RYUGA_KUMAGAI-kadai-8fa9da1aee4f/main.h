//=============================================================================
//
// Mainヘッダー [main.h]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#pragma once


#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止
#include <stdio.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <d3dx9.h>

#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include "dinput.h"
#include "mmsystem.h"

#pragma warning(pop)


//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment (lib, "d3d11.lib")		
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3dx11.lib")	
#pragma comment (lib, "d3dx9.lib")	
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH	(960)				// ウインドウの幅
#define SCREEN_HEIGHT	(540)				// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標

#define	NUM_VERTEX		(4)					// 頂点数

typedef enum
{
    MODE_TITLE = 0,							// タイトル画面
    MODE_TUTORIAL,                          // チュートリアル画面
    MODE_GAME,								// ゲーム画面
    MODE_RESULT,							// リザルト画面
    MODE_MAX
} MODE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
long GetMousePosX(void);
long GetMousePosY(void);
char *GetDebugStr(void);

//=============================================================================
// BBによる当たり判定処理
// 回転は考慮しない
// 戻り値：当たってたらtrue
//=============================================================================
bool HitCheckBB(D3DXVECTOR3 m_pos, float m_hw, float m_hh,
                D3DXVECTOR3 y_pos, float y_hw, float y_hh);


void HitCheck(void);
void SetMode(MODE mode);
MODE GetMode(void);



//衝突があればプレイヤー構造体のhitのところに代入
void CollisionCheckPlayer(int j);


void CollisionCheckBB(D3DXVECTOR3 m_pos, float m_hw, float m_hh,
                      D3DXVECTOR3 y_pos, float y_hw, float y_hh, int j);