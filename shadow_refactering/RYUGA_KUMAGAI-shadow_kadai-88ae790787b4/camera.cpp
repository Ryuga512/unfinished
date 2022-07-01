//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "StencilShadow.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	POS_X_CAM		(0.0f)			// カメラの初期位置(X座標)
#define	POS_Y_CAM		(50.0f)			// カメラの初期位置(Y座標)
#define	POS_Z_CAM		(-140.0f)		// カメラの初期位置(Z座標)

#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(10000.0f)										// ビュー平面のFarZ値

#define	VALUE_MOVE_CAMERA	(2.0f)										// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)							// カメラの回転量
#define ZOOM_CAMERA         (30.0f)                                     // ズーム時のカメラ
#define TPS_CAMERA          (100.0f)
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAMERA			g_Camera;		// カメラデータ
//カメラは姿勢行列に保存。そのフレームのカメラの変化はその時のカメラの姿勢行列からスタート
static D3DXMATRIX PoseMat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
float Yaw = 0, Pitch = 0, Roll = 0;
//フレームごとに足しこむ。重要なのは”１成分だけ”足しこむこと
D3DXMATRIX DeltaMat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);//姿勢の微小変化分

// カメラの注視点の線形移動用の移動座標テーブル
static D3DXVECTOR3	g_MoveTbl_at[] = {
    D3DXVECTOR3(0.0f, 7.0f, 0.0f),
    D3DXVECTOR3(-200.0f, 7.0f, 0.0f),
    D3DXVECTOR3(-200.0f, 7.0f, 200.0f),
};


// カメラの注視点の線形移動用の移動スピードテーブル
static float	g_MoveSpd_at[] = {
    0.01f,
    0.1f,
    0.005f,
};

// カメラの視点の線形移動用の移動座標テーブル
static D3DXVECTOR3	g_MoveTbl_pos[] = {
    D3DXVECTOR3(0.0f, 50.0f, 0.0f),
    D3DXVECTOR3(-200.0f, 50.0f, 50.0f),
    D3DXVECTOR3(-200.0f, 50.0f, 100.0f),
};


// カメラの視点の線形移動用の移動スピードテーブル
static float	g_MoveSpd_pos[] = {
    0.01f,
    0.1f,
    0.005f,
};


//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
    //カメラの視点
	g_Camera.pos = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);

    //カメラの注視点
	g_Camera.at = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    //上方向
	g_Camera.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

    //回転角度
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//// 視点と注視点の距離を計算
	//float vx, vz;
	//vx = g_Camera.pos.x - g_Camera.at.x;
	//vz = g_Camera.pos.z - g_Camera.at.z;
	g_Camera.len = TPS_CAMERA;

    // イベントカメラのフラグを初期化
    g_Camera.event_camera = false;

    // カメラの注視点用の時間をクリア
    g_Camera.time_at = 0.0f;

    //カメラの視点用の時間をクリア
    g_Camera.time_pos = 0.0f;
}


//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{

}


//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
    PLAYER *player = GetPlayer();


    ////Yaw成分
    //if (GetKeyState(VK_LEFT) & 0x80)
    //{
    //    D3DXMatrixRotationAxis(&DeltaMat, &D3DXVECTOR3(PoseMat._21, PoseMat._22, PoseMat._23), -0.01);
    //}
    //else if (GetKeyState(VK_RIGHT) & 0x80)
    //{
    //    D3DXMatrixRotationAxis(&DeltaMat, &D3DXVECTOR3(PoseMat._21, PoseMat._22, PoseMat._23), 0.01);
    //}
    ////Pitch成分
    //else if (GetKeyState(VK_UP) & 0x80)
    //{
    //    D3DXMatrixRotationAxis(&DeltaMat, &D3DXVECTOR3(PoseMat._11, PoseMat._12, PoseMat._13), -0.01);
    //}
    //else if (GetKeyState(VK_DOWN) & 0x80)
    //{
    //    D3DXMatrixRotationAxis(&DeltaMat, &D3DXVECTOR3(PoseMat._11, PoseMat._12, PoseMat._13), 0.01);
    //}
    ////Roll成分
    //else if (GetKeyState('Z') & 0x80)
    //{
    //    D3DXMatrixRotationAxis(&DeltaMat, &D3DXVECTOR3(PoseMat._31, PoseMat._32, PoseMat._33), -0.01);
    //}
    //else if (GetKeyState('X') & 0x80)
    //{
    //    D3DXMatrixRotationAxis(&DeltaMat, &D3DXVECTOR3(PoseMat._31, PoseMat._32, PoseMat._33), 0.01);
    //}

    if (GetKeyboardPress(DIK_Q) || IsButtonPressed(0, BUTTON_A))
    {// 注視点旋回「左」
        g_Camera.rot.y -= VALUE_ROTATE_CAMERA;
        if (g_Camera.rot.y < -D3DX_PI)
        {
            g_Camera.rot.y += D3DX_PI * 2.0f;
        }

        g_Camera.at.x = g_Camera.pos.x + sinf(g_Camera.rot.y) * g_Camera.len;
        g_Camera.at.z = g_Camera.pos.z + cosf(g_Camera.rot.y) * g_Camera.len;
    }

    if (GetKeyboardPress(DIK_E) || IsButtonPressed(0, BUTTON_C))
    {// 注視点旋回「右」
        g_Camera.rot.y += VALUE_ROTATE_CAMERA;
        if (g_Camera.rot.y > D3DX_PI)
        {
            g_Camera.rot.y -= D3DX_PI * 2.0f;
        }

        g_Camera.at.x = g_Camera.pos.x + sinf(g_Camera.rot.y) * g_Camera.len;
        g_Camera.at.z = g_Camera.pos.z + cosf(g_Camera.rot.y) * g_Camera.len;
    }
    player->rot.y = g_Camera.rot.y;

    if (GetKeyboardPress(DIK_T) || IsButtonPressed(0, BUTTON_X))
    {// 注視点移動「上」
     // 仮にx軸を使う
        if (g_Camera.rot.x < 1.0f)
        {
            g_Camera.rot.x += VALUE_ROTATE_CAMERA;
        }

    }

    if (GetKeyboardPress(DIK_B) || IsButtonPressed(0, BUTTON_B))
    {// 注視点移動「下」
     // 仮にx軸を使う
        if (g_Camera.rot.x  > -1.0f)
        {
            g_Camera.rot.x -= VALUE_ROTATE_CAMERA;

        }
    }


    if (!g_Camera.event_camera)
    {
        //cam->at = g_Player.pos;
        //cam->pos = cam->at + D3DXVECTOR3(0.0f, 50.0f, -140.0f);
        //cam->pos.x = cam->at.x - sinf(cam->rot.y) * cam->len;
        //cam->pos.z = cam->at.z - cosf(cam->rot.y) * cam->len;
    }



    //右クリックでズーム、離すと戻る
    if (IsMouseRightPressed() || IsButtonPressed(0, BUTTON_L))
    {
        g_Camera.at = player->pos;
        g_Camera.at.y = player->pos.y + 10;
        g_Camera.len = ZOOM_CAMERA;        
        player->rot.y = g_Camera.rot.y + D3DX_PI;
        g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
        g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
        g_Camera.pos.y = player->pos.y + 10;
    }
    else
    {
        g_Camera.len = TPS_CAMERA;
        g_Camera.at = player->pos;
        g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
        g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
        g_Camera.pos.y = POS_Y_CAM;
    }

    g_Camera.at.y = g_Camera.pos.y + sinf(g_Camera.rot.x) * g_Camera.len;
    //if (GetKeyboardPress(DIK_M))
    //{// 離れる
    //    g_Camera.len += VALUE_MOVE_CAMERA;
    //    g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
    //    g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
    //}

    //// カメラを初期に戻す
    //if (GetKeyboardPress(DIK_R))
    //{
    //    UninitCamera();
    //    InitCamera();
    //}

    //if (GetKeyboardTrigger(DIK_9))
    //{
    //    g_Camera.event_camera = g_Camera.event_camera ? false : true;
    //}

    //if (g_Camera.event_camera)
    //{
    //    {
    //    // エネミー０番だけテーブルに従って座標移動（線形補間）
    //    int nowNo = (int)g_Camera.time_at;			// 整数分であるテーブル番号を取り出している
    //    int maxNo = (sizeof(g_MoveTbl_at) / sizeof(D3DXVECTOR3));       // 登録テーブル数を数えている
    //    int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
    //    D3DXVECTOR3	pos = g_MoveTbl_at[nextNo] - g_MoveTbl_at[nowNo];   // XYZ移動量を計算している
    //    float nowTime = g_Camera.time_at - nowNo;	// 時間部分である少数を取り出している
    //    pos *= nowTime;								// 現在の移動量を計算している

    //    // 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
    //    g_Camera.at = g_MoveTbl_at[nowNo] + pos;

    //    g_Camera.time_at += g_MoveSpd_at[nowNo];	// 時間を進めている
    //    if ((int)g_Camera.time_at >= maxNo)		    // 登録テーブル最後まで移動したか？
    //    {
    //        g_Camera.time_at -= maxNo;			    // ０番目にリセットしつつも小数部分を引き継いでいる
    //    }
    //}

    //    {
    //    // エネミー０番だけテーブルに従って座標移動（線形補間）
    //    int nowNo = (int)g_Camera.time_pos;			// 整数分であるテーブル番号を取り出している
    //    int maxNo = (sizeof(g_MoveTbl_pos) / sizeof(D3DXVECTOR3));       // 登録テーブル数を数えている
    //    int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
    //    D3DXVECTOR3	pos = g_MoveTbl_pos[nextNo] - g_MoveTbl_pos[nowNo];   // XYZ移動量を計算している
    //    float nowTime = g_Camera.time_pos - nowNo;	// 時間部分である少数を取り出している
    //    pos *= nowTime;								// 現在の移動量を計算している

    //    // 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
    //    g_Camera.pos = g_MoveTbl_pos[nowNo] + pos;

    //    g_Camera.time_pos += g_MoveSpd_pos[nowNo];	// 時間を進めている
    //    if ((int)g_Camera.time_pos >= maxNo)		    // 登録テーブル最後まで移動したか？
    //    {
    //        g_Camera.time_pos -= maxNo;			    // ０番目にリセットしつつも小数部分を引き継いでいる
    //    }
    //    }

    //}

}

void DrawCamera(void)
{
    return;
}
//=============================================================================
// カメラの更新
//=============================================================================
void SetCamera(void) 
{
    ////微小変化を足しこむ
    //PoseMat *= DeltaMat;

    //D3DXVECTOR3 CamDir;//カメラが向く方向
    //CamDir = D3DXVECTOR3(0, 0, 1);//最初はZ軸方向を向くところからスタート
    //D3DXVec3TransformCoord(&CamDir, &CamDir, &PoseMat);

    //// ビュートランスフォーム　これが”カメラ”
    //D3DXVECTOR3 LookPos = g_Camera.pos + CamDir;//注視点　この点とカメラ位置からカメラの方向が決定される
    //D3DXVec3TransformCoord(&g_Camera.up, &g_Camera.up, &PoseMat);

	// ビューマトリックス設定
	D3DXMatrixLookAtLH(&g_Camera.mtxView, &g_Camera.pos, &g_Camera.at, &g_Camera.up);

    SetViewMatrix(&g_Camera.mtxView);

	float det;
	D3DXMatrixInverse(&g_Camera.mtxInvView, &det, &g_Camera.mtxView);


	// プロジェクションマトリックス設定
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection, 1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, VIEW_NEAR_Z, VIEW_FAR_Z);

	SetProjectionMatrix(&g_Camera.mtxProjection);
}


//=============================================================================
// カメラの取得
//=============================================================================
CAMERA *GetCamera(void) 
{
	return &g_Camera;
}

