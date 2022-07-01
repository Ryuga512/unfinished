//=============================================================================
//
// 弾処理 [Bullet.cpp]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(1)				// テクスチャの数

#define	BULLET_WIDTH	    (50.0f)			// 頂点サイズ
#define	BULLET_HEIGHT		(80.0f)			// 頂点サイズ

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBullet(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点バッファ
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static BULLET				g_aBullet[MAX_BULLET];	// 弾ワーク
static int					g_TexNo;			    // テクスチャ番号
static bool					g_bAlpaTest;		    // アルファテストON/OFF
static int					g_nAlpha;			    // アルファテストの閾値

static char *g_TextureName[] =
{
    "data/TEXTURE/bullet000.png",
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBullet(void)
{
    MakeVertexBullet();

    // テクスチャ生成
    for (int i = 0; i < TEXTURE_MAX; i++)
    {
        D3DX11CreateShaderResourceViewFromFile(GetDevice(),
            g_TextureName[i],
            NULL,
            NULL,
            &g_Texture[i],
            NULL);
    }

    g_TexNo = 0;

    // 弾ワークの初期化
    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        ZeroMemory(&g_aBullet[nCntBullet].material, sizeof(g_aBullet[nCntBullet].material));
        g_aBullet[nCntBullet].material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

        g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBullet[nCntBullet].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
        g_aBullet[nCntBullet].fWidth = BULLET_WIDTH;
        g_aBullet[nCntBullet].fHeight = BULLET_HEIGHT;
        g_aBullet[nCntBullet].bUse = false;
        g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBullet[nCntBullet].speed = 1.5f;
        g_aBullet[nCntBullet].radius = 3;
    }



    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{
    for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
    {
        if (g_Texture[nCntTex] != NULL)
        {// テクスチャの解放
            g_Texture[nCntTex]->Release();
            g_Texture[nCntTex] = NULL;
        }
    }

    if (g_VertexBuffer != NULL)
    {// 頂点バッファの解放
        g_VertexBuffer->Release();
        g_VertexBuffer = NULL;
    }
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        if (g_aBullet[nCntBullet].bUse)
        {
            //弾の移動処理
            g_aBullet[nCntBullet].pos.x += sinf(g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].speed;
            g_aBullet[nCntBullet].pos.z += cosf(g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].speed;
            g_aBullet[nCntBullet].pos.y += sinf(g_aBullet[nCntBullet].rot.x) * g_aBullet[nCntBullet].speed;

            // 影の位置設定
            SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, 0.1f, g_aBullet[nCntBullet].pos.z));
        
            if (g_aBullet[nCntBullet].pos.x < WALL_LEFT
                || g_aBullet[nCntBullet].pos.x > WALL_RIGHT
                || g_aBullet[nCntBullet].pos.z < WALL_DOWN
                || g_aBullet[nCntBullet].pos.z > WALL_UP
                || g_aBullet[nCntBullet].pos.y < (GROUND - 1)
                )
            {
                g_aBullet[nCntBullet].bUse = false;
                ReleaseShadow(g_aBullet[nCntBullet].nIdxShadow);
            }
        }
    }
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{

    // ライティングを無効
    SetLightEnable(false);

    D3DXMATRIX mtxView, mtxScale, mtxTranslate, mtxRot;

    // 頂点バッファ設定
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

    // プリミティブトポロジ設定
    GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        if (g_aBullet[nCntBullet].bUse)
        {
            // ワールドマトリックスの初期化
            D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

            // ビューマトリックスを取得
            CAMERA *cam = GetCamera();
            mtxView = cam->mtxView;

            // ポリゴンを正面に向ける

            // 逆行列を計算で求める
            D3DXMatrixInverse(&g_aBullet[nCntBullet].mtxWorld, NULL, &mtxView);
            g_aBullet[nCntBullet].mtxWorld._41 = 0.0f;
            g_aBullet[nCntBullet].mtxWorld._42 = 0.0f;
            g_aBullet[nCntBullet].mtxWorld._43 = 0.0f;

            // スケールを反映
                D3DXMatrixScaling(&mtxScale, g_aBullet[nCntBullet].scl.x,
                    g_aBullet[nCntBullet].scl.y,
                    g_aBullet[nCntBullet].scl.z);
            D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxScale);

            ////回転を反映
            //D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBullet[nCntBullet].rot.y, g_aBullet[nCntBullet].rot.x, g_aBullet[nCntBullet].rot.z);
            //D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxRot);

            // 移動を反映
            D3DXMatrixTranslation(&mtxTranslate, g_aBullet[nCntBullet].pos.x,
                g_aBullet[nCntBullet].pos.y,
                g_aBullet[nCntBullet].pos.z);
            D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTranslate);


            // ワールドマトリックスの設定
            SetWorldMatrix(&g_aBullet[nCntBullet].mtxWorld);

            // マテリアル設定
            SetMaterial(g_aBullet[nCntBullet].material);

            // テクスチャ設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[nCntBullet % TEXTURE_MAX]);

            // ポリゴンの描画
            GetDeviceContext()->Draw(4, 0);
        }
    }

    // ライティングを有効に
    SetLightEnable(true);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexBullet(void)
{
    // 頂点バッファ生成
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(VERTEX_3D) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

    // 頂点バッファに値をセットする
    D3D11_MAPPED_SUBRESOURCE msr;
    GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

    VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

    float fWidth = 20.0f;
    float fHeight = 20.0f;

    // 頂点座標の設定
    vertex[0].Position = D3DXVECTOR3(-fWidth / 2.0f, 0.1f, fHeight / 2.0f);
    vertex[1].Position = D3DXVECTOR3(fWidth  / 2.0f, 0.1f, fHeight / 2.0f);
    vertex[2].Position = D3DXVECTOR3(-fWidth / 2.0f, 0.1f, -fHeight / 2.0f);
    vertex[3].Position = D3DXVECTOR3(fWidth  / 2.0f, 0.1f, -fHeight / 2.0f);
    
    // 拡散光の設定
    vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    // テクスチャ座標の設定
    vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
    vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
    vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
    vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

    GetDeviceContext()->Unmap(g_VertexBuffer, 0);

    return S_OK;
}

//=============================================================================
// 弾のパラメータをセット
//=============================================================================
int SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    int nIdxBullet = -1;

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        if (!g_aBullet[nCntBullet].bUse)
        {
            //g_aBullet[nCntBullet].rot.x = ;
            g_aBullet[nCntBullet].pos = pos;
            g_aBullet[nCntBullet].rot = rot;
            g_aBullet[nCntBullet].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
            g_aBullet[nCntBullet].bUse = true;

            // 影の設定
            g_aBullet[nCntBullet].nIdxShadow = CreateShadow(g_aBullet[nCntBullet].pos, 0.5f, 0.5f);

            nIdxBullet = nCntBullet;

            break;
        }
    }

    return nIdxBullet;
}

BULLET *GetBullet(void)
{
    return g_aBullet;
}
