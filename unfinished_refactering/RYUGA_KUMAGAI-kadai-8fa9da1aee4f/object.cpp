//=============================================================================
//
// エネミー処理 [object.cpp]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#include "object.h"
#include "sprite.h"
#include "game.h"
#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// (200)			// キャラサイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// (200)			// 
#define TEXTURE_MAX					(OBJECT_MAX)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(1)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(4)		// アニメーションの切り替わるWait値

#define object_SPEED			    (2.0f)	// 移動速度


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {	// Texファイルの場所
    "data/TEXTURE/wall/saike.jpeg",
    "data/TEXTURE/wall/saike.jpeg",
    "data/TEXTURE/wall/saike.jpeg",
    "data/TEXTURE/wall/saike.jpeg",

    "data/TEXTURE/wall/kirtain.png",
    "data/TEXTURE/wall/kirtain.png",
    "data/TEXTURE/wall/kirtain.png",
    "data/TEXTURE/wall/kirtain.png",

    "data/TEXTURE/yubi.png",

};

OBJECT object[OBJECT_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitObject(void)
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


    // オブジェクトの初期化
    for (int i = 0; i < OBJECT_MAX; i++)
    {
        object[i].use = true;
        object[i].texNo = i;				// 読みこんだ画像の順番で表示する

        object[i].countAnim = 0;
        object[i].patternAnim = 0;		    // アニメのパターン番号

    }

    object[0].w   = 100;
    object[0].h   = SCREEN_WIDTH;
    object[0].pos = D3DXVECTOR3(0.0f,SCREEN_HEIGHT * 0.5, 0.0f);
    object[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    object[4].w   = 100;
    object[4].h   = SCREEN_WIDTH;
    object[4].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT * 0.5, 0.0f);
    object[4].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);



    object[1].w   = 100;
    object[1].h   = SCREEN_WIDTH;
    object[1].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT * 0.5, 0.0f);
    object[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    object[5].w   = 100;
    object[5].h   = SCREEN_WIDTH;
    object[5].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT * 0.5, 0.0f);
    object[5].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);



    object[2].w   = SCREEN_WIDTH;
    object[2].h   = 100;
    object[2].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT, 0.0f);
    object[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    object[6].w   = SCREEN_WIDTH;
    object[6].h   = 100;
    object[6].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT, 0.0f);
    object[6].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);



    object[3].w   = SCREEN_WIDTH;
    object[3].h   = 100;
    object[3].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5, 0.0f, 0.0f);
    object[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    object[7].w   = SCREEN_WIDTH;
    object[7].h   = 100;
    object[7].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5, 0.0f, 0.0f);
    object[7].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitObject(void)
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
void UpdateObject(void)
{

#ifdef _DEBUG	// デバッグ情報を表示する
    //char *str = GetDebugStr();
    //sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawObject(void)
{
    int n = GetMode();

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

    for (int i = 0; i < OBJECT_MAX; i++)
    {
        if (object[i].use == true)		// オブジェクトが使われている？
        {						// Yes
            // テクスチャ設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[object[i].texNo]);

            // オブジェクトの位置やテクスチャー座標を反映
            float px = object[i].pos.x;	// オブジェクトの表示位置X
            float py = object[i].pos.y;	// オブジェクトの表示位置Y
            float pw = object[i].w;		// オブジェクトの表示幅
            float ph = object[i].h;		// オブジェクトの表示高さ
            float ro = object[i].rot.z;

            float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
            float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
            float tx = tw * object[i].patternAnim;	    // テクスチャの左上X座標
            float ty = 0.0f;	                        // テクスチャの左上Y座標

            // １枚のポリゴンの頂点とテクスチャ座標を設定
            // チュートリアルとゲーム本編で分けている
            if ((n == MODE_TUTORIAL && 4 <= i) ||
                (n == MODE_GAME     && i < 4)   )
            {
                SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ro);

                // ポリゴン描画
                GetDeviceContext()->Draw(4, 0);
            }
        }
    }

    if (n == MODE_TUTORIAL)
    {
        // テクスチャ設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[8]);

        // １枚のポリゴンの頂点とテクスチャ座標を設定
        SetSprite(g_VertexBuffer, 900, 100, 200, 200, 0, 0, 1, 1);

        // ポリゴン描画
        GetDeviceContext()->Draw(4, 0);
    }

}


//=============================================================================
// オブジェクト構造体のアドレスを取得
//=============================================================================
OBJECT *GetObject_p(void)
{
    return &object[0];
}
