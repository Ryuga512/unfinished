

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer WorldBuffer : register( b0 )
{
	matrix World;
}

cbuffer ViewBuffer : register( b1 )
{
	matrix View;
}

cbuffer ProjectionBuffer : register( b2 )
{
	matrix Projection;
}

// マテリアルバッファ
struct MATERIAL
{
	float4		Ambient;//環境光
	float4		Diffuse;//拡散反射光
	float4		Specular;//鏡面反射光
	float4		Emission;//放射光
	float		Shininess;//光沢
	float3		Dummy;//16bit境界用
};

cbuffer MaterialBuffer : register( b3 )
{
	MATERIAL	Material;
}

// ライト用バッファ
struct LIGHT
{
    float4      Direction;
    float4      Position;

    float4      Attenuation;

    matrix      View;
    matrix      Projection;

	float4		Diffuse;
	float4		Ambient;
	int			Type;
	int			Dummy[3];
};

cbuffer LightBuffer : register( b4 )
{
	LIGHT		Light;
}

struct CAMERA
{
    float4 Eye;
};
//=============================================================================
// 頂点シェーダ
//=============================================================================



void VertexShaderPolygon( in  float4 inPosition		: POSITION0,
						  in  float4 inNormal		: NORMAL0,
						  in  float4 inDiffuse		: COLOR0,
						  in  float2 inTexCoord		: TEXCOORD0,

						  out float4 outPosition	: SV_POSITION,
						  out float4 outNormal		: NORMAL0,
						  out float2 outTexCoord	: TEXCOORD0,
						  out float4 outDiffuse		: COLOR0 )
{

    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    outNormal = inNormal;
    outTexCoord = inTexCoord;
    outPosition = mul(inPosition, wvp);


    //float4 ZCalcTex;
    //wvp = mul(World, Light.View);
    //wvp = mul(wvp, Light.Projection);
    //ZCalcTex = mul(wvp, inPosition);


    if (Light.Type == 0)
    {
        outDiffuse = inDiffuse * Material.Diffuse;
    }
    else
    {
        float4 dir;

        dir = Light.Position - inPosition;

        float len = length(dir);
        float4 worldNormal;
        inNormal.w = 0;

        worldNormal = mul(inNormal, World);
        worldNormal = normalize(worldNormal);
        //拡散
        float light = dot(dir, worldNormal);
        //減衰
        float colA = saturate(1.0f / (Light.Attenuation.x + Light.Attenuation.y * len + Light.Attenuation * len * len));

        outDiffuse = inDiffuse * Material.Diffuse * light * Light.Diffuse;
        //outDiffuse += inDiffuse * Material.Ambient * Light.Ambient;
        outDiffuse.a = inDiffuse.a * Material.Diffuse.a * colA;
    }


}


//void VertexShadow(in  float4 inPosition  : POSITION0,
//                  in  float4 inNormal    : NORMAL0,
//                  in  float4 inDiffuse   : COLOR0,
//                  in  float2 inTexCoord  : TEXCOORD0,
//
//                  out float4 outPosition : SV_POSITION,
//                  out float4 outNormal   : NORMAL0,
//                  out float2 outTexCoord : TEXCOORD0,
//                  out float4 outDiffuse  : COLOR0)
//{
//    matrix wvp;
//    wvp = mul(World, View);
//    wvp = mul(wvp, Projection);
//
//    outNormal = inNormal;
//    outTexCoord = inTexCoord;
//
//    float4 dir;
//
//    dir = Light.Position - inPosition;
//
//    float LN = dot(inNormal, dir);
//    float scale = (0 < LN) ? 0.0f : 5.0f;
//
//    inPosition -= dir * scale;
//
//    outPosition = mul(inPosition, wvp);
//
//    outDiffuse = (0.0f, 0.0f, 0.0f, 0.0f);
//}


//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture : register( t0 );      //テクスチャー
SamplerState	g_SamplerState : register( s0 ); //サンプラー


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void PixelShaderPolygon( in  float4 inPosition		: POSITION0,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float4 inDiffuse		: COLOR0,

						 out float4 outDiffuse		: SV_Target )
{

    outDiffuse = g_Texture.Sample( g_SamplerState, inTexCoord );

	outDiffuse *= inDiffuse;

}
