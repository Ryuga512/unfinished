//=============================================================================
//
// Main処理 [main.cpp]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "camera.h"

#include "player.h"
#include "enemy.h"
#include "input.h"
#include "attack.h"
#include "bg.h"
#include "sound.h"

#include "game.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "object.h"
#include "effect.h"
#include "tutorial.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME			"AppClass"						// ウインドウのクラス名
#define WINDOW_NAME			"Unfinished"	// ウインドウのキャプション名

//*****************************************************************************
// 構造体定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void HitCheck(void);
bool HitCheckBB(D3DXVECTOR3 m_pos, float m_hw, float m_hh,
    D3DXVECTOR3 y_pos, float y_hw, float y_hh);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
long g_MouseX = 0;
long g_MouseY = 0;
int  stop     = 0;
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
	//UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	//UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

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
		(HBRUSH)(COLOR_WINDOW+1),
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
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME)*2,									// ウィンドウ横幅
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME)*2+GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
						NULL,
						NULL,
						hInstance,
						NULL);

    bool mode = true;

    int id = MessageBox(NULL, "Windowモードでプレイしますか？", "起動モード", MB_YESNOCANCEL | MB_ICONQUESTION);

    switch (id)
    {
    case IDYES:                                                   //YESならWindowモードで起動
        mode = true;
        break;

    case IDNO:                                                    //NOならフルスクリーンモードで起動
        mode = false;
        break;

    case IDCANCEL:                                                //CANCELなら終了
    default:
        return -1;
        break;
    }
	
	if(FAILED(Init(hInstance, hWnd, mode)))                       // DirectXの初期化(ウィンドウを作成してから行う)
	{
		return -1;
	}

	                                                              // フレームカウント初期化
	timeBeginPeriod(1);                         	              // 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	              // システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;
	
	                                                
	ShowWindow(hWnd, nCmdShow);                                  // ウインドウの表示(Init()の後に呼ばないと駄目)
	UpdateWindow(hWnd);
	
	
	while(1)                                                     // メッセージループ
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{                                                     // PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				                                    
				TranslateMessage(&msg);                            // メッセージの翻訳とディスパッチ
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();					        // システム時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)        	// 1秒ごとに実行
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				        // FPSを測定した時刻を保存
				dwFrameCount = 0;							        // カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	                    // 処理した時刻を保存

#ifdef _DEBUG	                                                    // デバッグ版の時だけFPSを表示する
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif

				Update();			                                // 更新処理
				Draw();				                                // 描画処理

#ifdef _DEBUG	                                                    // デバッグ版の時だけ表示する
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " MX:%d MY:%d", GetMousePosX(), GetMousePosY());
				SetWindowText(hWnd, g_DebugStr);
#endif

				dwFrameCount++;		                                // 処理回数のカウントを加算
			}
		}
	}
	
	timeEndPeriod(1);				                                // 分解能を戻す

	                                                                
	UnregisterClass(CLASS_NAME, wcex.hInstance);                    // ウィンドウクラスの登録を解除

	
	Uninit();                                                       // 終了処理

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:					// [ESC]キーが押された
			DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
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
    // レンダリング処理の初期化
    InitRenderer(hInstance, hWnd, bWindow);

    // カメラ処理の初期化
    InitCamera();

    // 入力処理の初期化
    InitInput(hInstance, hWnd);

    // サウンド処理の初期化 
    InitSound(hWnd);

    // フェードの初期化
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

    // フェードの終了処理
    UninitFade();

    // サウンド終了処理
    UninitSound();

    // 入力の終了処理
    UninitInput();

    // カメラの終了処理
    UninitCamera();

    // レンダリングの終了処理
    UninitRenderer();

}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
    // 入力の更新処理
    UpdateInput();

    // カメラの更新処理
    UpdateCamera();

#ifdef _DEBUG
    if (GetKeyboardPress(DIK_P))
    {
        stop++;
    }

    if (stop % 10 > 1)
    {
        return;
    }
#endif

    // モードによって処理を分ける
    switch (g_Mode)
    {
    case MODE_TITLE:		// タイトル画面の更新
        UpdateTitle();
        break;

    case MODE_TUTORIAL:     // チュートリアル画面の更新
        UpdateTutorial();
        break;

    case MODE_GAME:			// ゲーム画面の更新
        UpdateGame();
        break;

    case MODE_RESULT:		// リザルト画面の更新
        UpdateResult();
        break;
    }

    // フェード処理の更新
    UpdateFade();
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
    // バックバッファクリア
    Clear();

    // カメラをセット
    SetCamera();

    // 2D描画なので深度無効
    SetDepthEnable(false);

    // モードによって処理を分ける
    switch (g_Mode)
    {
    case MODE_TITLE:		// タイトル画面の描画
        DrawTitle();
        break;

    case MODE_TUTORIAL:     // チュートリアル画面の描画
        DrawTutorial();
        break;

    case MODE_GAME:			// ゲーム画面の描画
        DrawGame();
        break;

    case MODE_RESULT:		// リザルト画面の描画
        DrawResult();
        break;
    }

    // フェード描画
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
char *GetDebugStr(void)
{
	return g_DebugStr;
}
#endif



void CollisionCheckPlayer(int j)
{
    OBJECT *object = GetObject_p();
    PLAYER *player = GetPlayer_p();

    for (int i = 0; i < 4; i++)
    {
        player[j].hit[i] = false;
    }

    for (int i = 0; i < OBJECT_MAX; i++)
    {
        if (object[i].use == false)continue;    //オブジェクトが消えてたら行わない

        if (player[j].use == false)continue;    //プレイヤーが死んでたら行わない

        CollisionCheckBB(player[j].pos, player[j].w * 0.5, player[j].h * 0.5,
                         object[i].pos, object[i].w * 0.5, object[i].h * 0.5, j);
        CollisionCheckBB(player[j].pre_pos, player[j].w * 0.5, player[j].h * 0.5,
            object[i].pos, object[i].w * 0.5, object[i].h * 0.5, j);

    }
}

void CollisionCheckBB(D3DXVECTOR3 m_pos, float m_hw, float m_hh,
                      D3DXVECTOR3 y_pos, float y_hw, float y_hh, int j)
{
    PLAYER *player = GetPlayer_p();

    //オブジェクトが上にあるとき
    if ((m_pos.y > y_pos.y) && (m_pos.x > y_pos.x - y_hw) && (m_pos.x < y_pos.x + y_hw))
    {
    //プレイヤーの上端がオブジェクトの下端を超えたとき
        if (m_pos.y - m_hh <= y_pos.y + y_hh)
        {
            player[j].hit[UP] = true;
            if (m_pos.y - m_hh < y_pos.y + y_hh)
            {
                player[j].pos.y = y_pos.y + y_hh + m_hh;
            }
        }
        else
        {
            player[j].hit[UP] = false;
        }
    }

    //オブジェクトが下にあるとき
    if ((m_pos.y < y_pos.y) && (m_pos.x > y_pos.x - y_hw) && (m_pos.x < y_pos.x + y_hw))
    {
    //プレイヤーの下端がオブジェクトの上端を超えたとき
        if (m_pos.y + m_hh >= y_pos.y - y_hh)
        {
            player[j].hit[DOWN] = true;
            if (m_pos.y + m_hh > y_pos.y - y_hh)
            {
                player[j].pos.y = y_pos.y - y_hh - m_hh;
            }
        }
        else
        {
            player[j].hit[DOWN] = false;
        }
    }

    //オブジェクトが右側にある時
    if ((m_pos.x < y_pos.x) && (m_pos.y > y_pos.y - y_hh) && (m_pos.y < y_pos.y + y_hh))
    {
    //プレイヤーの右端がオブジェクトの左端を超えたとき
        if (m_pos.x + m_hw >= y_pos.x - y_hw)
        {
            player[j].hit[RIGHT] = true;
            if (m_pos.x + m_hw > y_pos.x - y_hw)
            {
                player[j].pos.x = y_pos.x - y_hw - m_hw;
            }
        }
        else
        {
            player[j].hit[RIGHT] = false;
        }
    }

    //オブジェクトが左側にある時
    if ((m_pos.x > y_pos.x) && (m_pos.y > y_pos.y - y_hh) && (m_pos.y < y_pos.y + y_hh))
    {
    //プレイヤーの左端がオブジェクトの右端を超えたとき
        if (m_pos.x - m_hw <= y_pos.x + y_hw)
        {
            player[j].hit[LEFT] = true;
            if (m_pos.x - m_hw < y_pos.x + y_hw)
            {
                player[j].pos.x = y_pos.x + y_hw + m_hw;
            }

        }
        else
        {
            player[j].hit[LEFT] = false;
        }
    }
}


void HitCheck(void)
{
    BULLET* bullet = GetBullet();
    ENEMY * enemy  = GetEnemy_p();
    PLAYER* player = GetPlayer_p();

    //エネミーとプレイヤーの衝突処理
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (enemy[i].use == false)continue;     //エネミーが死んでたら行わない

        for (int j = 0; j < PLAYER_MAX; j++)
        {

            if (player[j].use == false)continue;//プレイヤーが死んでたら行わない

            //攻撃の範囲
            bool ans_atk = false;
            //攻撃時の当たり判定
            if (enemy[i].texNo == OTHER_FRONT && enemy[i].patternAnim == 4 && enemy[i].countAnim >= enemy[i].atk_delay)
            {
                if (HitCheckBB(player[j].pos, player[j].w * 0.2, player[j].h * 0.3,
                                enemy[i].pos,  enemy[i].w * 0.2,  enemy[i].h * 0.45))
                {
                    player[j].hp -= 20;
                    enemy[i].atk_delay = enemy[i].countAnim + 32;

                    //エネミーが上側にいる時
                    if (player[j].pos.y > enemy[i].pos.y)
                    {
                        player[j].pos.y += 20;
                    }
                    else
                    {
                        player[j].pos.y -= 20;
                    }
                }
            }
            else if (enemy[i].texNo == OTHER_SIDE && enemy[i].patternAnim == 4 && enemy[i].countAnim >= enemy[i].atk_delay)
            {
                if (HitCheckBB(player[j].pos, player[j].w * 0.2, player[j].h * 0.3,
                    enemy[i].pos, enemy[i].w * 0.5, enemy[i].h * 0.1))
                {
                    player[j].hp -= 20;
                    enemy[i].atk_delay = enemy[i].countAnim + 32;

                    //エネミーが右側にいる時
                    if (player[j].pos.x < enemy[i].pos.x)
                    {
                        player[j].pos.x -= 20;
                    }
                    else
                    {
                        player[j].pos.x += 20;
                    }
                }
            }
            else if (enemy[i].texNo == OTHER_DEFAULT && enemy[i].countAnim >= enemy[i].atk_delay)
            {
                if (HitCheckBB(player[j].pos, player[j].w * 0.2, player[j].h * 0.3,
                    enemy[i].pos, enemy[i].w * 0.2, enemy[i].h * 0.3))
                {
                    player[j].hp -= 10;
                    enemy[i].atk_delay = enemy[i].countAnim + 30;

                    //エネミーが上側にいる時
                    if (player[j].pos.y > enemy[i].pos.y)
                    {
                        player[j].pos.y += 20;
                    }
                    //エネミーが下側にいる時
                    else if (player[j].pos.y < enemy[i].pos.y)
                    {
                        player[j].pos.y -= 20;
                    }
                    //エネミーが右側にいる時
                    else if (player[j].pos.x < enemy[i].pos.x)
                    {
                        player[j].pos.x -= 20;
                    }
                    //エネミーが左側にいる時
                    else if (player[j].pos.x > enemy[i].pos.x)
                    {
                        player[j].pos.x += 20;
                    }
                }
            }
            else if (enemy[i].texNo == OTHER_ROLLING && enemy[i].patternAnim == 4 && enemy[i].countAnim >= enemy[i].atk_delay)
            {
                if (HitCheckBB(player[j].pos, player[j].w * 0.2, player[j].h * 0.3,
                    enemy[i].pos, enemy[i].w * 0.45, enemy[i].h * 0.45))
                {
                    player[j].hp -= 10;
                    enemy[i].atk_delay = enemy[i].countAnim + 30;
                }
            }
        }
    }

    bool ans = false;

    //エネミーと攻撃の衝突処理
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (enemy[i].use == false)continue;

        for (int j = 0; j < PLAYER_MAX; j++)
        {
            if ((player[j].dir == UP    && player[j].texNo  != 17) ||      //攻撃中じゃなかったら処理をとばす
                (player[j].dir == DOWN  && player[j].texNo  != 18) ||
                (player[j].dir == LEFT  && player[j].texNo  != 19) ||
                (player[j].dir == RIGHT && player[j].texNo  != 20)) 
                continue;


            if (player[j].dir == RIGHT)
            {
                ans = HitCheckBB(D3DXVECTOR3(player[j].pos.x + player[j].w + player[j].atk_w, player[j].pos.y, 0), player[j].atk_w, player[j].atk_h, enemy[i].pos, enemy[i].w * 0.3, enemy[i].h * 0.3);
            }
            else if (player[j].dir == LEFT)
            {
                ans = HitCheckBB(D3DXVECTOR3(player[j].pos.x - player[j].w - player[j].atk_w, player[j].pos.y, 0), player[j].atk_w, player[j].atk_h, enemy[i].pos, enemy[i].w * 0.3, enemy[i].h * 0.3);
            }
            else if (player[j].dir == UP)
            {
                ans = HitCheckBB(D3DXVECTOR3(player[j].pos.x, player[j].pos.y - player[j].h - player[j].atk_h, 0), player[j].atk_w, player[j].atk_h, enemy[i].pos, enemy[i].w * 0.3, enemy[i].h * 0.3);
            }
            else if (player[j].dir == DOWN)
            {
                ans = HitCheckBB(D3DXVECTOR3(player[j].pos.x, player[j].pos.y + player[j].h + player[j].atk_h, 0), player[j].atk_w, player[j].atk_h, enemy[i].pos, enemy[i].w * 0.3, enemy[i].h * 0.3);
            }

            if (ans == true)
            {
                if (player[j].atk_delay <= player[j].countAnim)
                {
                    enemy[i].hp        -= player[j].attack;
                    player[j].atk_delay = player[j].countAnim + ATACK_DELAY;
                }
                if (enemy[i].hp <= 0)
                {
                    enemy[i].use = false;
                }
            }
        }
        UpdateEffect(enemy[i].pos, i, ans);
    }

    // エネミーが全部死亡したら状態遷移
    int enemy_count = 0;
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (enemy[i].use == false) continue;
        enemy_count++;
    }

    // エネミーが０匹？
    if (enemy_count == 0 && g_Mode != MODE_TUTORIAL)
    {
        SetFade(FADE_OUT, MODE_RESULT);
    }

    // プレイヤーが全部死亡したら状態遷移
    int player_count = 0;
    for (int i = 0; i < PLAYER_MAX; i++)
    {
        if (player[i].use == false) continue;
        player_count++;
    }

    // プレイヤーが０匹？
    if (player_count == 0 && g_Mode == MODE_GAME)
    {
        SetFade(FADE_OUT, MODE_TUTORIAL);
    }
    else if (player_count == 0 && g_Mode == MODE_TUTORIAL)
    {
        SetFade(FADE_OUT, MODE_GAME);
    }

}

bool HitCheckBB(D3DXVECTOR3 m_pos, float m_hw, float m_hh, 
                D3DXVECTOR3 y_pos, float y_hw, float y_hh)
{
    if ((m_pos.x + m_hw >= y_pos.x - y_hw) &&    //mの右側 > yの左側
        (m_pos.x - m_hw <= y_pos.x + y_hw) &&    //mの左側 < yの右側
        (m_pos.y + m_hh >= y_pos.y - y_hh) &&    //mの下側 > yの上側
        (m_pos.y - m_hh <= y_pos.y + y_hh)   )   //mの上側 < yの下側
    {
        return true;                             //当たった場合
    }
    return false;                                //外れた場合
}

//=============================================================================
// モードの設定
//=============================================================================
void SetMode(MODE mode)
{
    // モードを変える前に全部メモリを解放しちゃう
    // タイトル画面の終了処理
    UninitTitle();

    //チュートリアル画面の終了処理
    UninitTutorial();

    // ゲーム画面の終了処理
    UninitGame();

    // リザルト画面の終了処理
    UninitResult();


    g_Mode = mode;

    switch (g_Mode)
    {
    case MODE_TITLE:
    // タイトル画面の初期化
        InitTitle();
        break;

    case MODE_TUTORIAL:     
    // チュートリアル画面の初期化
        InitTutorial();
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

