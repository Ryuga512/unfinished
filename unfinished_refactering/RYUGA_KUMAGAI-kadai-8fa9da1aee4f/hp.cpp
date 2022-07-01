//=============================================================================
//
// エネミー処理 [hp.cpp]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#include "hp.h"
#include "sprite.h"
#include "game.h"
#include "enemy.h"
#include "player.h"
#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// (200)			// キャラサイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// (200)			// 
#define TEXTURE_MAX					(HP_MAX)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(1)		    // アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		    // アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(4)		    // アニメーションの切り替わるWait値



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {	// Texファイルの場所
    "data/TEXTURE/fade_black.png",
    "data/TEXTURE/HP.png",
    "data/TEXTURE/blood.png",
};

HP hp[HP_MAX];
int m;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitHp(void)
{
    m = GetMode();
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


    // オブジェクトの初期化
    for (int i = 0; i < HP_MAX; i++)
    {
        hp[i].use = true;

        hp[i].texNo = i;				// 読みこんだ画像の順番で表示する

        hp[i].countAnim = 0;
        hp[i].patternAnim = 0;		    // アニメのパターン番号
    }

    if (m == MODE_TUTORIAL)
    {
        hp[0].use = false;
        hp[1].use = false;
        hp[2].use = true;
    }

    hp[0].w   = SCREEN_WIDTH - 200;
    hp[0].h   = 20;
    hp[0].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 15, 0.0f);
    hp[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


    hp[1].w   = SCREEN_WIDTH - 200;
    hp[1].h   = 10;
    hp[1].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 15, 0.0f);
    hp[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    hp[2].w   = SCREEN_WIDTH;
    hp[2].h   = SCREEN_HEIGHT;
    hp[2].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
    hp[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHp(void)
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
void UpdateHp(void)
{
    BossHpBar();

#ifdef _DEBUG	// デバッグ情報を表示する
    //char *str = GetDebugStr();
    //sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHp(void)
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

    for (int i = 0; i < HP_MAX; i++)
    {
        if (hp[i].use == true)		// オブジェクトが使われている？
        {						// Yes
            // テクスチャ設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[hp[i].texNo]);

            // オブジェクトの位置やテクスチャー座標を反映
            float px = hp[i].pos.x;	// hpの表示位置X
            float py = hp[i].pos.y;	// hpの表示位置Y
            float pw = hp[i].w;		// hpの表示幅
            float ph = hp[i].h;		// hpの表示高さ
            float ro = hp[i].rot.z;

            float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
            float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
            float tx = tw * hp[i].patternAnim;	        // テクスチャの左上X座標
            float ty = 0.0f;	                        // テクスチャの左上Y座標

            if (i < 2)
            {
                // １枚のポリゴンの頂点とテクスチャ座標を設定
                SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ro);
            }
            else
            {
                PLAYER *player = GetPlayer_p();
                SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th, D3DXCOLOR(1.0f, 1.0f, 1.0f, (100 - player[0].hp) / 100.0f), ro);
            }
            // ポリゴン描画
            GetDeviceContext()->Draw(4, 0);
        }
    }
}


//=============================================================================
// オブジェクト構造体のアドレスを取得
//=============================================================================
HP *GetHp_p(void)
{
    return &hp[0];
}


void BossHpBar(void)
{
    ENEMY *enemy = GetEnemy_p();

    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (enemy[i].boss)
        {
            float width  = hp[0].w * enemy[i].hp / BOSS_HP_MAX;
            hp[1].w      = width;
            hp[1].pos.x  = hp[0].pos.x - (BOSS_HP_MAX - enemy[i].hp) * 0.76f * (500.0f / BOSS_HP_MAX);
        }
    }
}

