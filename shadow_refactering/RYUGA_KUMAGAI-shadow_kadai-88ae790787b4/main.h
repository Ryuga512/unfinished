//=============================================================================
//
// メイン処理 [main.h]
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
#define SCREEN_WIDTH	(960)			// ウインドウの幅
#define SCREEN_HEIGHT	(540)			// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標
#define WALL_LEFT       (-640)
#define WALL_RIGHT      (640)
#define WALL_UP         (640)
#define WALL_DOWN       (-640)
#define GROUND          (0.0f)          

// 補間用のデータ構造体を定義
struct INTERPOLATION_DATA
{
    D3DXVECTOR3 pos;		// 頂点座標(親の座標を原点とした相対座標)
    D3DXVECTOR3 rot;		// 回転(親の回転に対して自分を回転)
    D3DXVECTOR3 scl;		// 拡大縮小(親の拡縮に対して自分を拡縮)
    float		frame;		// 実行フレーム数 ( dt = 1.0f/frame )
};

typedef enum
{
    MODE_TITLE = 0,							// タイトル画面
    MODE_GAME,								// ゲーム画面
    MODE_RESULT,							// リザルト画面
    MODE_MAX
} MODE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
long GetMousePosX(void);
long GetMousePosY(void);
char* GetDebugStr(void);
void HitCheck(void);
bool HitCheckBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2);
bool HitCheckBB(D3DXVECTOR3 mpos, float mhw, float mhh, D3DXVECTOR3 ypos, float yhw, float yhh);
void SetMode(MODE mode);
MODE GetMode(void);