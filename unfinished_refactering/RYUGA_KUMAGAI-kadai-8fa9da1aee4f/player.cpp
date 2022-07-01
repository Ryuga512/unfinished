//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#include "player.h"
#include "sprite.h"
#include "input.h"
#include "attack.h"
#include "sound.h"
#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// (200)			// キャラサイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// (200)			// 
#define TEXTURE_MAX					(21)    // テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(8)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(8)		// アニメーションの切り替わるWait値

#define PLAYER_SPEED				(5.0f)	// プレイヤーの移動速度
#define ATTACK_HEIGHT               (15)
#define ATTACK_WIDTH                (15)

#define STEP                        (100)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {	// Texファイルの場所
    //チュートリアル中のプレイヤー
	"data/TEXTURE/tutorial_player/player_bdefault.png", 
    "data/TEXTURE/tutorial_player/player_fdefault.png", 
    "data/TEXTURE/tutorial_player/player_ldefault.png",
    "data/TEXTURE/tutorial_player/player_rdefault.png",
    "data/TEXTURE/tutorial_player/player_bwalk.png",
    "data/TEXTURE/tutorial_player/player_fwalk.png",
    "data/TEXTURE/tutorial_player/player_lwalk.png",
    "data/TEXTURE/tutorial_player/player_rwalk.png",
    "data/TEXTURE/tutorial_player/attack_side.png",

    //ゲーム中のプレイヤー
    "data/TEXTURE/game_player/player_bdefault.png",
    "data/TEXTURE/game_player/player_fdefault.png",
    "data/TEXTURE/game_player/player_ldefault.png",
    "data/TEXTURE/game_player/player_rdefault.png",
    "data/TEXTURE/game_player/player_bwalk.png",
    "data/TEXTURE/game_player/player_fwalk.png",
    "data/TEXTURE/game_player/player_lwalk.png",
    "data/TEXTURE/game_player/player_rwalk.png",
    //攻撃
    "data/TEXTURE/game_player/player_bdefault.png",
    "data/TEXTURE/game_player/player_fdefault.png",
    "data/TEXTURE/game_player/player_ldefault.png",
    "data/TEXTURE/game_player/player_rdefault.png",

};

PLAYER player[PLAYER_MAX];		// プレイヤー配列
int mode;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
    mode = GetMode();

	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(
			GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// プレイヤーの初期化
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		player[i].use         = true;
		player[i].w           = 100;
		player[i].h           = 200;
		player[i].pos         = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.5 + 200, 0.0f);
		player[i].rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        if (mode == MODE_GAME)
        {
            player[i].texNo = 9;
            player[i].dir   = UP;
        }
        else if(mode == MODE_TUTORIAL)
        {
            player[i].texNo = 0;        
            player[i].dir   = DOWN;
        }
		player[i].countAnim   = 0;
		player[i].patternAnim = 0;		
        player[i].attack      = 100;
        player[i].hp          = 100;
        player[i].pre_pos     = D3DXVECTOR3(-100, 0, 0);
        player[i].pre_tx      = 0;
        player[i].pre_texNo   = 0;
        for (int j = 0; j < 4; j++)
        {
            player[i].hit[j]  = false;
        }
        player[i].atk_w       = 15;
        player[i].atk_h       = 15;
        player[i].atk_delay   = ATACK_DELAY;
        player[i].in_attack   = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	// 頂点情報を解放
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	// テクスチャー情報を解放
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}



}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
        if (player[i].hp <= 0)
        {
            player[i].use = false;
        }
                // プレイヤーが使われている？
		if (player[i].use == true)		    
		{						           
		        // アニメーション (Wait値) 
			player[i].countAnim++;
			if ((player[i].countAnim % ANIM_WAIT) == 0)
			{
                if (player[i].texNo == 8 && player[i].patternAnim == 7)
                {
                    player[i].hp -= 25;
                }

                // パターンの切り替え
                player[i].patternAnim = (player[i].patternAnim + 1) % ANIM_PATTERN_NUM;

                //攻撃状態の時、64フレーム(mainに記載)待ってデフォルト状態にする
                if (mode           == MODE_GAME&&
                   ((player[i].dir == UP       && player[i].texNo == 17) ||
                    (player[i].dir == DOWN     && player[i].texNo == 18) ||
                    (player[i].dir == LEFT     && player[i].texNo == 19) ||
                    (player[i].dir == RIGHT    && player[i].texNo == 20))&&
                     player[i].patternAnim == 0)
                {
                    switch (player[i].dir)
                    {
                    case UP:
                        player[i].texNo = 9;
                        break;
                    case DOWN:
                        player[i].texNo = 10;
                        break;
                    case LEFT:
                        player[i].texNo = 11;
                        break;
                    case RIGHT:
                        player[i].texNo = 12;
                        break;
                    }
                }
			}

            CollisionCheckPlayer(i);

            //攻撃の時以外移動などできる
            if (((player[i].dir    == UP    && player[i].texNo != 17) ||
                 (player[i].dir    == DOWN  && player[i].texNo != 18) ||
                 (player[i].dir    == LEFT  && player[i].texNo != 19) ||
                 (player[i].dir    == RIGHT && player[i].texNo != 20))&&
                  player[i].use    == true  && player[i].texNo != 8)
            {
                // キー入力で移動 
                if (GetKeyboardPress(DIK_DOWN))
                {
                    player[i].dir   = DOWN;
                    if (!player[i].hit[DOWN])
                    {
                        player[i].pos.y += PLAYER_SPEED;
                        if (mode == MODE_GAME)
                        {
                            player[i].texNo = 14;
                        }
                        else
                        {
                            player[i].texNo = 5;
                        }
                    }
                }
                else if (GetKeyboardPress(DIK_UP))
                {
                    player[i].dir = UP;
                    if (!player[i].hit[UP])
                    {
                        player[i].pos.y -= PLAYER_SPEED;
                        if (mode == MODE_GAME)
                        {
                            player[i].texNo = 13;
                        }
                        else
                        {
                            player[i].texNo = 4;
                        }
                    }

                }
                else if (GetKeyboardPress(DIK_RIGHT))
                {
                    player[i].dir = RIGHT;
                    if (!player[i].hit[RIGHT])
                    {
                        player[i].pos.x += PLAYER_SPEED;

                        if (mode == MODE_GAME)
                        {
                            player[i].texNo = 16;
                        }
                        else
                        {
                            player[i].texNo = 7;
                        }
                    }
                }
                else if (GetKeyboardPress(DIK_LEFT))
                {
                    player[i].dir = LEFT;
                    if (!player[i].hit[LEFT])
                    {
                        player[i].pos.x -= PLAYER_SPEED;

                        if (mode == MODE_GAME)
                        {
                            player[i].texNo = 15;
                        }
                        else
                        {
                            player[i].texNo = 6;
                        }
                    }
                }
                else if (player[i].dir == DOWN)
                {
                    if (mode == MODE_GAME)
                    {
                        player[i].texNo = 10;
                    }
                    else
                    {
                        player[i].texNo = 1;
                    }
                }
                else if (player[i].dir == UP)
                {
                    if (mode == MODE_GAME)
                    {
                        player[i].texNo = 9;
                    }
                    else
                    {
                        player[i].texNo = 0;
                    }
                }
                else if (player[i].dir == RIGHT)
                {
                    if (mode == MODE_GAME)
                    {
                        player[i].texNo = 12;
                    }
                    else
                    {
                        player[i].texNo = 3;
                    }
                }
                else if (player[i].dir == LEFT)
                {
                    if (mode == MODE_GAME)
                    {
                        player[i].texNo = 11;
                    }
                    else
                    {
                        player[i].texNo = 2;
                    }
                }

                if (mode == MODE_GAME && GetKeyboardTrigger(DIK_SPACE))
                {
                    player[i].patternAnim = 0;

                    switch (player[i].dir)
                    {
                    case UP:
                        player[i].texNo = 17;
                        break;
                    case DOWN:
                        player[i].texNo = 18;
                        break;
                    case LEFT:
                        player[i].texNo = 19;
                        break;
                    case RIGHT:
                        player[i].texNo = 20;
                        break;
                    }
                }
                else if (mode == MODE_TUTORIAL && GetKeyboardTrigger(DIK_SPACE) &&
                    player[i].pos.x == 860 && player[i].pos.y >= 200 && player[i].pos.y <= 280)
                {
                    player[i].patternAnim = 0;
                    player[i].texNo = 8;
                }

                // ゲームパッドで移動処理
                if (IsButtonPressed(0, BUTTON_DOWN))
                {
                    player[i].pos.y += PLAYER_SPEED;
                }
                else if (IsButtonPressed(0, BUTTON_UP))
                {
                    player[i].pos.y -= PLAYER_SPEED;
                }

                if (IsButtonPressed(0, BUTTON_RIGHT))
                {
                    player[i].pos.x += PLAYER_SPEED;
                }
                else if (IsButtonPressed(0, BUTTON_LEFT))
                {
                    player[i].pos.x -= PLAYER_SPEED;
                }
            }

            //いつでも発動できる位置
            if (mode == MODE_GAME && GetKeyboardTrigger(DIK_Z))
            {
                if (player[i].pre_pos.x == -100)
                {
                    player[i].pre_pos = player[i].pos;
                }
                else
                {
                    player[i].pos = player[i].pre_pos;
                    player[i].pre_pos = D3DXVECTOR3(-100, 0, 0);
                }

            }
		}

	}


#ifdef _DEBUG	// デバッグ情報を表示する
	char *str = GetDebugStr();
	sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", player[0].pos.x, player[0].pos.y);
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定（材質情報、Tex情報）
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	for (int i = 0; i < PLAYER_MAX; i++)
	{
        if (player[i].use == true)		// プレイヤーが使われている？
        {
            // テクスチャ設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[player[i].texNo]);
        }
        else
        {
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);
        }
			// プレイヤーの位置やテクスチャー座標を反映
			float px = player[i].pos.x;					// プレイヤーの表示位置X
			float py = player[i].pos.y;					// プレイヤーの表示位置Y
			float pw = player[i].w;						// プレイヤーの表示幅
			float ph = player[i].h;						// プレイヤーの表示高さ

			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
			float tx = tw * player[i].patternAnim;		// テクスチャの左上X座標
			float ty = 0.0f;							// テクスチャの左上Y座標
            float ro = 0.0f;
			// １枚のポリゴンの頂点とテクスチャ座標を設定
            SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
                                    D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ro);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

            if (player[i].pre_pos.x != -100)
            {
                GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[player[i].pre_texNo]);
                SetSpriteColorRotation(g_VertexBuffer, player[i].pre_pos.x, player[i].pre_pos.y, pw, ph, player[i].pre_tx, ty, tw, th,
                    D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), ro);
                GetDeviceContext()->Draw(4, 0);
            }
            else
            {
                player[i].pre_tx    = tx;
                player[i].pre_texNo = player[i].texNo;
            }
	}
}

//=============================================================================
// プレイヤーの座標を取得
//=============================================================================
PLAYER *GetPlayer_p(void)
{
	return &player[0];
}


