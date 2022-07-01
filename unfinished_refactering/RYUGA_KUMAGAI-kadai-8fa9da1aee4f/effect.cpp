//=============================================================================
//
// エネミー処理 [effect.cpp]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#include "enemy.h"
#include "sprite.h"
#include "effect.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// (200)			// キャラサイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// (200)			// 
#define TEXTURE_MAX					(EFFECT_MAX)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(5)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(2)		// アニメーションの切り替わるWait値

#define effect_SPEED					(2.0f)	// 移動速度


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {	// Texファイルの場所
    "data/TEXTURE/effect/jaki.png",		
};

EFFECT effect[EFFECT_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(void)
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
    for (int i = 0; i < EFFECT_MAX; i++)
    {
        effect[i].use         = true;
        effect[i].w           = 200;
        effect[i].h           = 200;
        effect[i].pos         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        effect[i].rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        effect[i].texNo       = i;			  // 読みこんだ画像の順番で表示する
        effect[i].countAnim   = 0;
        effect[i].patternAnim = 0;		      // アニメのパターン番号
        effect[i].alpha       = 0.0f;
    }

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
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
void UpdateEffect(D3DXVECTOR3 PosArg, int i, bool ans)
{
        effect[i].use = ans;
        if (effect[i].use == true)
        {						// Yes
                // アニメーション (Wait値) 
            effect[i].countAnim++;
            if ((effect[i].countAnim % ANIM_WAIT) == 0)
            {
                // パターンの切り替え
                effect[i].patternAnim = (effect[i].patternAnim + 1) % ANIM_PATTERN_NUM;
            }
            effect[i].pos   = PosArg;
            if (effect[i].countAnim == 64)
            {
                effect[i].alpha = 0.0f;
                effect[i].countAnim = 0;
                effect[i].patternAnim = 0;
            }
            else
            {
                effect[i].alpha = 1.0f;
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
void DrawEffect(void)
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

    for (int i = 0; i < EFFECT_MAX; i++)
    {
        if (effect[i].use == true)		// プレイヤーが使われている？
        {						// Yes
            // テクスチャ設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[effect[i].texNo]);

            // プレイヤーの位置やテクスチャー座標を反映
            float px = effect[i].pos.x;    	// プレイヤーの表示位置X
            float py = effect[i].pos.y;    	// プレイヤーの表示位置Y
            float pw = effect[i].w;		// プレイヤーの表示幅
            float ph = effect[i].h;		// プレイヤーの表示高さ

            float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
            float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
            float tx = tw * effect[i].patternAnim;	// テクスチャの左上X座標
            float ty = th * (effect[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X);	// テクスチャの左上Y座標
            float ro = 0.0f;
            // １枚のポリゴンの頂点とテクスチャ座標を設定
            SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
                D3DXCOLOR(1.0f, 1.0f, 1.0f, effect[i].alpha), ro);


            // ポリゴン描画
            GetDeviceContext()->Draw(4, 0);
        }
    }
}


//=============================================================================
// エネミー構造体のアドレスを取得
//=============================================================================
EFFECT *GetEffect_p(void)
{
    return &effect[0];
}


