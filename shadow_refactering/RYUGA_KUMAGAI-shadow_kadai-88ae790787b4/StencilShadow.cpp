//=============================================================================
//
// モデルの処理 [model.cpp]
// Author : 
//
//=============================================================================
#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "main.h"
#include "StencilShadow.h"
#include "camera.h"
#include "renderer.h"
#include "main.h"
#include "player.h"
#include "sprite.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define MODEL_FILENAME		"data/MODEL/miku_01.obj"
#define	VALUE_MOVE_MODEL	(0.50f)					// 移動速度
#define	RATE_MOVE_MODEL		(0.20f)					// 移動慣性係数
#define	VALUE_ROTATE_MODEL	(PI * 0.05f)			// 回転速度
#define	RATE_ROTATE_MODEL	(0.20f)					// 回転慣性係数
#define	SCALE_MODEL			(10.0f)					// 回転慣性係数


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct CBUFF_PER_MESH//メッシュ単位で渡す情報
{
    D3DXMATRIX mW;//ワールド行列
    D3DXMATRIX mWVP;//ワールドから射影までの変換行列
};
struct CBUFF_PER_MATERIAL//マテリアル単位で渡す情報
{
    D3DXVECTOR4 vAmbient;//アンビエント光
    D3DXVECTOR4 vDiffuse;//ディフューズ色
    D3DXVECTOR4 vSpecular;//鏡面反射
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
ID3D11DeviceContext* ImmediateContext;
ID3D11Device* D3DDevice;
ID3D11DepthStencilState* m_pDSState;
ID3D11DepthStencilState* m_pDSState2;
ID3D11BlendState* m_pBlendState;
ID3D11Buffer* m_pVertexBuffer;//画面を蔽うポリゴン用
ID3D11Buffer* VertexBuffer;
ID3D11Buffer* m_pCBuffPerMaterial;
DWORD id[2][2];
int NumVertices = 0;
static STENCIL_SHADOW g_shadow[MAX_STENCIL_SHADOW];

bool InitStencilShadow(char *FileName, STENCIL_SHADOW *shadow)
{
    LoadObj(FileName, &shadow->model);

    {
        //ボリュームメッシュ用のバーテックスバッファーを作成
        D3D11_BUFFER_DESC bd;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(VERTEX_3D) * 100000;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        //D3D11_SUBRESOURCE_DATA sd;
        //ZeroMemory(&sd, sizeof(sd));
        //sd.pSysMem = shadow->model.VertexArray;

        GetDevice()->CreateBuffer(&bd, /*&sd*/NULL, &shadow->dx_model.VertexBuffer);
    }

    // インデックスバッファ生成
    {
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(unsigned short) * shadow->model.IndexNum;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = shadow->model.IndexArray;

        GetDevice()->CreateBuffer(&bd, &sd, &shadow->dx_model.IndexBuffer);
    }

    // サブセット設定
    {
        shadow->dx_model.SubsetArray = new DX11_SUBSET[shadow->model.SubsetNum];
        shadow->dx_model.SubsetNum = shadow->model.SubsetNum;

        for (unsigned short i = 0; i < shadow->model.SubsetNum; i++)
        {
            shadow->dx_model.SubsetArray[i].StartIndex = shadow->model.SubsetArray[i].StartIndex;
            shadow->dx_model.SubsetArray[i].IndexNum = shadow->model.SubsetArray[i].IndexNum;

            shadow->dx_model.SubsetArray[i].Material.Material = shadow->model.SubsetArray[i].Material.Material;
            shadow->dx_model.SubsetArray[i].Material.Material.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

            D3DX11CreateShaderResourceViewFromFile(GetDevice(),
                                                   shadow->model.SubsetArray[i].Material.TextureName,
                                                   NULL,
                                                   NULL,
                                                   &shadow->dx_model.SubsetArray[i].Material.Texture,
                                                   NULL);
        }
    }
    shadow->bUse = true;

    return true;
}

bool InitStencilShadow(void)
{
    ImmediateContext = GetDeviceContext();
    D3DDevice = GetDevice();


    // ステンシル書き込み時の挙動を定義するレンダリングステートのオブジェクトを作成
    D3D11_DEPTH_STENCIL_DESC dc;
    //深度テスト
    dc.DepthEnable = true;//既存ポリゴンをはさんでいる判断に必要
    dc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;//ただし自分自身でZテストしないために書き込みはゼロ
    dc.DepthFunc = D3D11_COMPARISON_LESS;
    //ステンシルテスト
    dc.StencilEnable = true;
    dc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    dc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    //表面をどうするか
    dc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
    dc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
    dc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    //裏面をどうするか
    dc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
    dc.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR;
    dc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    D3DDevice->CreateDepthStencilState(&dc, &m_pDSState);

    // ステンシル利用時の挙動を定義するレンダリングステートのオブジェクトを作成
    dc.DepthEnable = false;
    dc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dc.DepthFunc = D3D11_COMPARISON_LESS;
    //ステンシルテスト
    dc.StencilEnable = true;
    dc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    dc.StencilWriteMask = 0x00;
    //表面をどうするか
    dc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
    dc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
    //裏面をどうするか
    dc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
    dc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dc.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;

    D3DDevice->CreateDepthStencilState(&dc, &m_pDSState2);

    //アルファブレンド用ブレンドステート作成
    D3D11_BLEND_DESC bld;
    ZeroMemory(&bld, sizeof(D3D11_BLEND_DESC));
    bld.IndependentBlendEnable = false;
    bld.AlphaToCoverageEnable = false;
    bld.RenderTarget[0].BlendEnable = true;
    bld.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bld.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bld.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bld.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bld.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bld.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bld.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    if (FAILED(D3DDevice->CreateBlendState(&bld, &m_pBlendState)))
    {
        return E_FAIL;
    }

    UINT mask = 0xffffffff;
    ImmediateContext->OMSetBlendState(m_pBlendState, NULL, mask);

    //画面前面のポリゴン用　バーテックスバッファーを作成
    VERTEX_3D VertexData[] =
    {
        D3DXVECTOR3(-SCREEN_WIDTH, -SCREEN_HEIGHT, 0), D3DXVECTOR3(0,0,0), D3DXCOLOR(0,0,0,1), D3DXVECTOR2(0,0),
        D3DXVECTOR3( SCREEN_WIDTH, -SCREEN_HEIGHT, 0), D3DXVECTOR3(0,0,0), D3DXCOLOR(0,0,0,1), D3DXVECTOR2(0,0),
        D3DXVECTOR3(-SCREEN_WIDTH,  SCREEN_HEIGHT, 0), D3DXVECTOR3(0,0,0), D3DXCOLOR(0,0,0,1), D3DXVECTOR2(0,0),
        D3DXVECTOR3( SCREEN_WIDTH,  SCREEN_HEIGHT, 0), D3DXVECTOR3(0,0,0), D3DXCOLOR(0,0,0,1), D3DXVECTOR2(0,0),

    };
    D3D11_BUFFER_DESC bd;

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_3D) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = VertexData;
    D3DDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);


    return true;
}

bool CreateStencilShadow(D3DXMATRIX world, LIGHT Light, STENCIL_SHADOW *shadow)
{
    D3D11_MAPPED_SUBRESOURCE SubR;
    D3DXVECTOR3 vertex0, vertex1, vertex2, vertex3;
    NumVertices = 0;

    D3DXVECTOR3 dir = D3DXVECTOR3(Light.Direction.x, Light.Direction.y, Light.Direction.z);

    D3DXMATRIX inv;
    D3DXMatrixInverse(&inv, NULL, &world);
    inv._41 = inv._42 = inv._43 = 0;

    D3DXVec3TransformCoord(&dir, &dir, &inv);

    int *edge = new int[shadow->model.VertexNum * 4];
    int Edges = 0;
    /**********************************************************************/
    //〜矩形引き伸ばし最適化後編〜
    /**********************************************************************/
    //全頂点について、引き延ばしを検討
    ImmediateContext->Map(shadow->dx_model.VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubR);
    VERTEX_3D* model = (VERTEX_3D*)SubR.pData;
    // メッシュ内頂点位置の最大と最小を検索する

    for (int i = 0; i < shadow->model.VertexNum - 3; i += 3)
    {
        //頂点を３つ使い、面の法線を求める
        vertex0 = shadow->model.VertexArray[i + 0].Position;
        vertex1 = shadow->model.VertexArray[i + 1].Position;
        vertex2 = shadow->model.VertexArray[i + 2].Position;


        D3DXVECTOR3 Normal;
        D3DXVECTOR3 cross0(vertex1 - vertex0);
        D3DXVECTOR3 cross1(vertex2 - vertex1);
        D3DXVec3Cross(&Normal, &cross0, &cross1);
        D3DXVec3Normalize(&Normal, &Normal);

        //ポリゴンの裏側なのかを判定&辺を抽出
        if (D3DXVec3Dot(&Normal, &dir) <= 0 && D3DXVec3Dot(&Normal, &dir) > -1.0)
        {
            edge[Edges + 0] = shadow->model.IndexArray[i + 0];
            edge[Edges + 1] = shadow->model.IndexArray[i + 1];

            edge[Edges + 2] = shadow->model.IndexArray[i + 1];
            edge[Edges + 3] = shadow->model.IndexArray[i + 2];

            edge[Edges + 4] = shadow->model.IndexArray[i + 2];
            edge[Edges + 5] = shadow->model.IndexArray[i + 0];

            Edges += 6;

        }
    }

    dir = -dir;
    for (DWORD i = 0; i < Edges - 6; i += 2)
    {
        vertex0 = shadow->model.VertexArray[edge[i + 0]].Position + shadow->model.VertexArray[edge[i + 0]].Normal * 0.05f;
        vertex1 = shadow->model.VertexArray[edge[i + 1]].Position + shadow->model.VertexArray[edge[i + 1]].Normal * 0.05f;
        vertex2 = vertex0 + dir * 10000;
        vertex3 = vertex1 + dir * 10000;

       model[NumVertices + 0].Position = vertex0;
       model[NumVertices + 1].Position = vertex1;
       model[NumVertices + 2].Position = vertex2;

       model[NumVertices + 3].Position = vertex1;
       model[NumVertices + 4].Position = vertex3;
       model[NumVertices + 5].Position = vertex2;

        NumVertices += 6;
    }
    D3DXComputeBoundingBox(&model->Position, shadow->model.VertexNum, sizeof(VERTEX_3D), &shadow->BBox.Min, &shadow->BBox.Max);

    ImmediateContext->Unmap(shadow->dx_model.VertexBuffer, 0);

    delete[] edge;
    /***************************************************/
    return true;
}


//=============================================================================
// 終了処理
//=============================================================================
void UnloadStencilShadow(STENCIL_SHADOW *shadow)
{
    if (shadow->dx_model.VertexBuffer)	shadow->dx_model.VertexBuffer->Release();
    if (shadow->dx_model.IndexBuffer)		shadow->dx_model.IndexBuffer->Release();
    if (shadow->dx_model.SubsetArray)		delete[] shadow->dx_model.SubsetArray;

}


//=============================================================================
// 描画処理
//=============================================================================
void DrawStencilShadow(STENCIL_SHADOW* shadow, D3DXMATRIX mtxWorld)
{
    PLAYER *Player = GetPlayer();
    LIGHT *Light = GetLight();

    //現在のレンダリングステートを退避させておく（後で戻すため）
    ID3D11DepthStencilState* pCurrentDSState = NULL;
    UINT Num = 0;
    ImmediateContext->OMGetDepthStencilState(&pCurrentDSState, &Num);
    // レンダリングステートの変更
    ImmediateContext->OMSetDepthStencilState(m_pDSState, 1);

    //バーテックスバッファーをセット
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    shadow->mtxWorld = mtxWorld;
    // ワールドマトリックスの設定
    SetWorldMatrix(&shadow->mtxWorld);
    //SetMaterial(shadow->dx_model.SubsetArray->Material.Material);
    //DrawModel(Model);
    ImmediateContext->IASetVertexBuffers(0, 1, &shadow->dx_model.VertexBuffer, &stride, &offset);
    ImmediateContext->Draw(NumVertices, 0);

    // レンダリングステートを元に戻す
    ImmediateContext->OMSetDepthStencilState(pCurrentDSState, Num);

}
//=============================================================================
// 位置の設定
//=============================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
    g_shadow[nIdxShadow].pos = pos;
}

void ReleaseShadow(int nIdxShadow)
{
    if (nIdxShadow >= 0 && nIdxShadow < MAX_STENCIL_SHADOW)
    {
        g_shadow[nIdxShadow].bUse = false;
    }
}

void polygon(void)
{
    //現在のレンダリングステートを退避させておく（後で戻すため）
    ID3D11DepthStencilState* pCurrentDSState = NULL;
    UINT Num = 0;
    ImmediateContext->OMGetDepthStencilState(&pCurrentDSState, &Num);

    D3DXMATRIX mtxWorld, mtxView, mtxTranslate, mtxScl, mtxRot;
    // レンダリングステートの変更
    ImmediateContext->OMSetDepthStencilState(m_pDSState2, 0);
    // プリミティブトポロジ設定
    ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // ワールドマトリックスの初期化
    D3DXMatrixIdentity(&mtxWorld);


    // ビューマトリックスを取得
    CAMERA *cam = GetCamera();
    mtxView = cam->mtxView;
    PLAYER *player = GetPlayer();
    // ポリゴンを正面に向ける


       // 頂点バッファ設定
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    ImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

    // マトリクス設定
    SetWorldViewProjection2D();

    ImmediateContext->Draw(4, 0);


    // レンダリングステートを元に戻す
    ImmediateContext->OMSetDepthStencilState(pCurrentDSState, Num);
}

void Peshanko(DX11_MODEL *Model)
{
    PLAYER *Player = GetPlayer();


    D3DXMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

    // ワールドマトリックスの初期化
    D3DXMatrixIdentity(&mtxWorld);

    // スケールを反映
    D3DXMatrixScaling(&mtxScl, Player->scl.x, Player->scl.y, Player->scl.z);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

    // 回転を反映
    D3DXMatrixRotationYawPitchRoll(&mtxRot, Player->rot.y, Player->rot.x, Player->rot.z);

    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);


    //移動を反映
    D3DXMatrixTranslation(&mtxTranslate, Player->pos.x, Player->pos.y, Player->pos.z);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

    D3DXMATRIX matShadow = D3DXMATRIX(
        1, 0,     0, 0,
        0, 0,     0, 0,
        0, 0,     1, 0,
        0, 0.01f, 0, 1);

    D3DXMATRIX mat;
    D3DXMatrixMultiply(&mat, &mtxWorld, &matShadow);

    // ワールドマトリックスの設定
    SetWorldMatrix(&mat);
    
    DrawModel(Model);

    return;
}


STENCIL_SHADOW* GetStencilShadow(void)
{
    return g_shadow;
}
