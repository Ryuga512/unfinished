//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#include "enemy.h"
#include "sprite.h"
#include "player.h"
#include <time.h>
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// (200)			// キャラサイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// (200)			// 
#define TEXTURE_MAX					(4)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(4)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			((TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y))	// アニメーションパターン数
#define ANIM_WAIT					(8)    // アニメーションの切り替わるWait値

#define ENEMY_SPEED					(2.0f)	// 移動速度

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void BossMotion(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {	// Texファイルの場所
	"data/TEXTURE/boss/other_default.png",
    "data/TEXTURE/boss/other_front_attack.png",
    "data/TEXTURE/boss/other_side_attack.png",
    "data/TEXTURE/boss/other_rolling_attack.png",
};

ENEMY enemy[ENEMY_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
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


	// エネミーの初期化
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i].use         = true;
		enemy[i].w           = 400;
		enemy[i].h           = 400;
		enemy[i].rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy[i].texNo       = OTHER_DEFAULT;

		enemy[i].countAnim   = 0;
		enemy[i].patternAnim = 0;		        // アニメのパターン番号
		enemy[i].left        = true;			// 左へ移動させる
        enemy[i].atk_delay   = 0;
        enemy[i].r           = 1.0f;
        enemy[i].g           = 1.0f;
        enemy[i].b           = 1.0f;
	}

    for (int i = 1; i < ENEMY_MAX; i++)
    {
        enemy[i].boss = false;
        enemy[i].hp   = 10.0f;
    }
    enemy[0].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5 - 120, 0.0f);
    enemy[0].boss = true;
    enemy[0].hp   = BOSS_HP_MAX;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
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
void UpdateEnemy(void)
{
    PLAYER *player = GetPlayer_p();
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy[i].use == true)
		{		

			// アニメーション (Wait値) 
			enemy[i].countAnim++;
			if ((enemy[i].countAnim % ANIM_WAIT) == 0)
			{
				// パターンの切り替え
				enemy[i].patternAnim = (enemy[i].patternAnim + 1) % (ANIM_PATTERN_NUM);

                if (enemy[i].patternAnim == 0 && enemy[i].texNo > OTHER_DEFAULT)
                {
                    enemy[i].texNo = OTHER_DEFAULT;
                }
                else if (enemy[i].patternAnim == 0)
                {
                    BossMotion();
                }
			}

            if (enemy[i].texNo == OTHER_ROLLING)
            {
                if (enemy[i].p_pos.x > enemy[i].pos.x)
                {
                    enemy[i].pos.x += 1;
                }
                else if (enemy[i].p_pos.x < enemy[i].pos.x)
                {
                    enemy[i].pos.x -= 1;
                }

                if (enemy[i].p_pos.y > enemy[i].pos.y)
                {
                    enemy[i].pos.y += 1;
                }
                else if (enemy[i].p_pos.y < enemy[i].pos.y)
                {
                    enemy[i].pos.y -= 1;
                }
            }
		}
	}


#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
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

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy[i].use == true)		                    // プレイヤーが使われている？
		{						// Yes
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[enemy[i].texNo]);

			// プレイヤーの位置やテクスチャー座標を反映
			float px = enemy[i].pos.x;	                    // プレイヤーの表示位置X
			float py = enemy[i].pos.y;	                    // プレイヤーの表示位置Y
			float pw = enemy[i].w;		                    // プレイヤーの表示幅
			float ph = enemy[i].h;		                    // プレイヤーの表示高さ

			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	                        // テクスチャの幅
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	                        // テクスチャの高さ
			float tx = tw * enemy[i].patternAnim;	                            // テクスチャの左上X座標
			float ty = th * (enemy[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X);	// テクスチャの左上Y座標
            // １枚のポリゴンの頂点とテクスチャ座標を設定
            SetSpriteColor(0, g_VertexBuffer, px, py, pw, ph,tx, ty, tw, th, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

            // ポリゴン描画
			GetDeviceContext()->Draw(4, 0);                 
		}
	}
}


//=============================================================================
// エネミー構造体のアドレスを取得
//=============================================================================
ENEMY *GetEnemy_p(void)
{
	return &enemy[0];
}

void BossMotion(void)
{
    PLAYER *player = GetPlayer_p();
    srand((unsigned int)time(NULL));
    int r = rand() % 2;
    for (int i = 0; i < PLAYER_MAX; i++)
    {
        for (int j = 0; j < ENEMY_MAX; j++)
        {
            if (enemy[j].boss)
            {
                if (r == 0)
                {
                    enemy[i].p_pos = player[i].pos;
                    enemy[j].texNo = OTHER_ROLLING;
                }
                //正面の距離300ぐらいにいるとき
                else if (player[i].pos.x >= enemy[j].pos.x - enemy[j].w * 0.3f &&
                    player[i].pos.x <= enemy[j].pos.x + enemy[j].w * 0.3f &&
                    player[i].pos.y  - enemy[j].pos.y <= 300              &&
                    player[i].pos.y  - enemy[j].pos.y >= 0)
                {
                    enemy[j].texNo = OTHER_FRONT;
                }
                else if (player[i].pos.y >= enemy[j].pos.y - enemy[j].h * 0.5f &&
                         player[i].pos.y <= enemy[j].pos.y + enemy[j].h * 0.5f &&
                         player[i].pos.x  - enemy[j].pos.x <= 300              &&
                         player[i].pos.x  - enemy[j].pos.x >= 0)
                {
                    enemy[j].texNo = OTHER_SIDE;
                }
                else if (player[i].pos.y >= enemy[j].pos.y - enemy[j].h * 0.5f &&
                         player[i].pos.y <= enemy[j].pos.y + enemy[j].h * 0.5f &&
                         player[i].pos.x - enemy[j].pos.x >= -300 &&
                         player[i].pos.x - enemy[j].pos.x <= 0)
                {
                    enemy[j].texNo = OTHER_SIDE;
                }
                else
                {
                    enemy[j].texNo = OTHER_DEFAULT;
                }
            }
        }
    }
    return;
}
