//=============================================================================
//
// メイン処理 [main.cpp]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include "tree.h"
#include "bullet.h"
#include "score.h"
#include "sound.h"
#include "particle.h"
#include "game.h"
#include "title.h"
#include "fade.h"
#include "StencilShadow.h"
#include "result.h"
#include "shadow.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"				// ウインドウのクラス名
#define WINDOW_NAME		"影表示"	            // ウインドウのキャプション名
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
long g_MouseX = 0;
long g_MouseY = 0;

MODE g_Mode = MODE_TITLE;

#ifdef _DEBUG
int		g_CountFPS;							// FPSカウンタ
char	g_DebugStr[2048] = WINDOW_NAME;		// デバッグ文字表示用

#endif

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// 時間計測用
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND		hWnd;
	MSG			msg;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,																		// ウィンドウの左座標
		CW_USEDEFAULT,																		// ウィンドウの上座標
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// ウィンドウ横幅
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
		NULL,
		NULL,
		hInstance,
		NULL);

	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// メッセージループ
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1秒ごとに実行
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

#ifdef _DEBUG	// デバッグ版の時だけFPSを表示する
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif

				Update();			// 更新処理
				Draw();				// 描画処理

#ifdef _DEBUG	// デバッグ版の時だけ表示する
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " MX:%d MY:%d", GetMousePosX(), GetMousePosY());
				SetWindowText(hWnd, g_DebugStr);
#endif

				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);				// 分解能を戻す

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_MOUSEMOVE:
		g_MouseX = LOWORD(lParam);
		g_MouseY = HIWORD(lParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
    InitSound(hWnd);

	InitRenderer(hInstance, hWnd, bWindow);
    InitStencilShadow();

	InitCamera();

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

    InitFade();

    // 最初のモードをセット
    SetMode(g_Mode);	// ここはSetModeのままで！

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
    // 終了のモードをセット
    SetMode(MODE_MAX);

    UninitFade();

	//入力の終了処理
	UninitInput();

	UninitRenderer();

    UninitSound();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
    // 入力の更新処理
	UpdateInput();

	// カメラ更新
	UpdateCamera();

    // モードによって処理を分ける
    switch (g_Mode)
    {
    case MODE_TITLE:		// タイトル画面の更新
        UpdateTitle();
        break;

    case MODE_GAME:			// ゲーム画面の更新
        UpdateGame();
        break;

    case MODE_RESULT:		// リザルト画面の更新
        UpdateResult();
        break;
    }

    UpdateFade();
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファクリア
	Clear();

	SetCamera();

    // モードによって処理を分ける
    switch (g_Mode)
    {
    case MODE_TITLE:		// タイトル画面の描画
        DrawTitle();
        break;

    case MODE_GAME:			// ゲーム画面の描画
        DrawGame();
        break;

    case MODE_RESULT:		// リザルト画面の描画
        DrawResult();
        break;
    }

    DrawFade();

	// バックバッファ、フロントバッファ入れ替え
	Present();
}


long GetMousePosX(void)
{
	return g_MouseX;
}


long GetMousePosY(void)
{
	return g_MouseY;
}


#ifdef _DEBUG
char* GetDebugStr(void)
{
	return g_DebugStr;
}
#endif

//=============================================================================
// BCによる当たり判定処理
// サイズは半径
// 戻り値：当たってたらtrue
//=============================================================================
bool HitCheckBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2)
{
    float len = (size1 + size2) * (size1 + size2);	// 半径を2乗した物
    D3DXVECTOR3 temp = pos1 - pos2;
    float fLengthSq = D3DXVec3LengthSq(&temp);		// 2点間の距離（2乗した物）

    if (len >= fLengthSq)
    {
        return true;
    }

    return false;
}

//=============================================================================
// BBによる当たり判定処理
// 回転は考慮しない
// 戻り値：当たってたらtrue
//=============================================================================
bool HitCheckBB(D3DXVECTOR3 mpos, float mhw, float mhh, D3DXVECTOR3 ypos, float yhw, float yhh)
{
    bool ans = false;

    if ((mpos.x + mhw > ypos.x - yhw) &&
        (mpos.x - mhw < ypos.x + yhw) &&
        (mpos.y + mhh > ypos.y - yhh) &&
        (mpos.y - mhh < ypos.y + yhh))
    {
        ans = true;
    }
    return ans;
}

//=============================================================================
// 当たり判定処理
//=============================================================================
void HitCheck(void)
{
    PLAYER *player = GetPlayer();
    ENEMY  *enemy = GetEnemy();
    BULLET *bullet = GetBullet();
    SHADOW *shadow = GetShadow();

    // プレイヤーと敵との当たり判定
    //for (int i = 0; i < MAX_PLAYER; i++)
    {
        if (player->use == true)// 死んでいたら行わない
        {

            // 敵は複数居ますよね？→for文使うよ
            for (int j = 0; j < MAX_ENEMY; j++)
            {
                if (enemy[j].use == false) continue;	// 死んでいたら行わない

                //バウンディングサークル(BC)での当たり判定を行う
                bool ans = HitCheckBC(player->pos, enemy[j].pos, player->radius, enemy[j].radius);
                if (ans == true)
                {
                    //今はエネミーを消している
                    enemy[j].use = false;
                    ReleaseShadow(enemy[j].shadowIdx);
                    if (enemy[j].stop)
                    {
                        PlaySound(SOUND_LABEL_SE_lockon000);
                        AddScore(10);
                    }
                    else
                    {
                        PlaySound(SOUND_LABEL_SE_defend000);
                    }
                }

            }
        }
    }

    for (int i = 0; i < MAX_BULLET; i++)
    {
        if (!bullet[i].bUse)continue;
        for (int j = 0; j < MAX_ENEMY; j++)
        {
            if (!enemy[j].use) continue;

            if (HitCheckBC(bullet[i].pos, enemy[j].pos, bullet[i].radius, enemy[j].radius))
            {
                enemy[j].use = false;
                ReleaseShadow(enemy[j].shadowIdx);
                AddScore(1);
            }
        }
    }

    for (int i = 0; i < MAX_BULLET; i++)
    {
        if (!bullet[i].bUse)continue;
        for (int j = 0; j < MAX_ENEMY; j++)
        {
            if (!enemy[j].use) continue;

            if (HitCheckBC(bullet[i].pos, shadow[enemy[j].shadowIdx].pos, bullet[i].radius, /*shadow[enemy[j].shadowIdx].scl.x*/15.0f))
            {
                PlaySound(SOUND_LABEL_SE_defend001);
                enemy[j].stop = true;
                ReleaseShadow(enemy[j].shadowIdx);
            }
        }
    }
}


//=============================================================================
// モードの設定
//=============================================================================
void SetMode(MODE mode)
{
    // モードを変える前に全部メモリを解放しちゃう
    // タイトル画面の終了処理
    UninitTitle();

    // ゲーム画面の終了処理
    if (mode == MODE_GAME)
    {
        UninitGame();
    }

    // リザルト画面の終了処理
    UninitResult();


    g_Mode = mode;

    switch (g_Mode)
    {
    case MODE_TITLE:
        // タイトル画面の初期化
        InitTitle();
        break;

    case MODE_GAME:
        // ゲーム画面の初期化
        InitGame();
        break;

    case MODE_RESULT:
        // リザルト画面の初期化
        InitResult();
        break;

        // ゲーム終了時の処理
    case MODE_MAX:
        break;
    }
}

//=============================================================================
// モードの取得
//=============================================================================
MODE GetMode(void)
{
    return g_Mode;
}
