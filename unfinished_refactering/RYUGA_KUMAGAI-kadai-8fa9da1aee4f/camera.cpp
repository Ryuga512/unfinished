//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"


D3DXMATRIX				g_CameraViewMatrix;
D3DXMATRIX				g_CameraInvViewMatrix;
D3DXMATRIX				g_CameraProjectionMatrix;

D3DXVECTOR3				g_CameraPosition;
D3DXVECTOR3				g_CameraTarget;

D3DXVECTOR3				g_CameraRotation;



D3DXMATRIX GetCameraViewMatrix()
{
	return g_CameraViewMatrix;
}

D3DXMATRIX GetCameraInvViewMatrix()
{
	return g_CameraInvViewMatrix;
}

D3DXMATRIX GetCameraProjectionMatrix()
{
	return g_CameraProjectionMatrix;
}

D3DXVECTOR3 GetCameraPosition()
{
	return g_CameraPosition;
}

//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
	g_CameraPosition = D3DXVECTOR3( 0.0f, 50.0f, -100.0f );
	g_CameraTarget   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_CameraRotation = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitCamera(void)
{


}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateCamera(void)
{

	PLAYER *player = GetPlayer_p();	// playerの中にプレイヤー配列０番のアドレスが入る

	g_CameraTarget.x = player[0].pos.x * 0.5f;
	g_CameraTarget.z = player[0].pos.z * 0.5f;

	g_CameraPosition.x = g_CameraTarget.x + sinf( g_CameraRotation.y ) * 100.0f;
	g_CameraPosition.z = g_CameraTarget.z - cosf( g_CameraRotation.y ) * 100.0f;

}


//=============================================================================
// 描画処理
//=============================================================================
void SetCamera(void)
{

	// ビューマトリックス設定
	D3DXMatrixLookAtLH( &g_CameraViewMatrix, &g_CameraPosition, &g_CameraTarget, &D3DXVECTOR3(0.0f, 1.0f, 0.0f) );

	SetViewMatrix( &g_CameraViewMatrix );


	float det;
	D3DXMatrixInverse(&g_CameraInvViewMatrix, &det, &g_CameraViewMatrix);


	// プロジェクションマトリックス設定
	D3DXMatrixPerspectiveFovLH( &g_CameraProjectionMatrix, 1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 1000.0f);

	SetProjectionMatrix( &g_CameraProjectionMatrix );

}



