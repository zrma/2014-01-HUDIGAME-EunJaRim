// 변환행렬
float4x4 matW: WORLD;
float4x4 matV: VIEW;
float4x4 matP: PROJECTION;

// 텍스처
texture	tex0;

// 입력정점 형식
struct VS_INPUT
{
	float3 pos  : POSITION;
	float4 diff : COLOR0;
	float2 tex  : TEXCOORD0;
};

// 출력정점 형식
struct VS_OUTPUT
{
	float4 pos  : POSITION;
	float4 diff : COLOR0;
	float2 tex  : TEXCOORD0;
};

// VS라는 정점쉐이더 함수 선언
VS_OUTPUT VS( VS_INPUT In )
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	// matW와 matV행렬을 곱해서 WorldView행렬생성
	float4x4 WorldView = mul( matW, matV );

	float4 InP = float4( In.pos, 1 ) * ( sin( g_fTime ) + 5 ) / 3;
	float3 P = mul( float4( InP.xyz, 1 ), ( float4x3 )WorldView );  // view공간에서의 위치계산

	float3 P = mul(float4(In.pos, 1), (float4x3)WorldView);  // view공간에서의 위치계산

	Out.pos = mul( float4( P, 1 ), matP );	// 투영공간에서의 위치계산
	Out.diff = In.diff;					// 입력색깔을 출력색깔로
	Out.tex.x = In.tex.x;				// 텍스처의 x좌표는 그대로...
	Out.tex.y = 1.0f - In.tex.y;		// 텍스처의 y좌표를 뒤집는다

	return Out;
}