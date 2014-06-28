// 다음 ifdef 블록은 DLL에서 내보내기하는 작업을 쉽게 해 주는 매크로를 만드는 
// 표준 방식입니다. 이 DLL에 들어 있는 파일은 모두 명령줄에 정의된 _EXPORTS 기호로
// 컴파일되며, 다른 프로젝트에서는 이 기호를 정의할 수 없습니다.
// 이렇게 하면 소스 파일에 이 파일이 들어 있는 다른 모든 프로젝트에서는 
// YAMANGDXDLL_API 함수를 DLL에서 가져오는 것으로 보고, 이 DLL은
// 이 DLL은 해당 매크로로 정의된 기호가 내보내지는 것으로 봅니다.
#ifdef YAMANGDXDLL_EXPORTS
#define YAMANGDXDLL_API __declspec(dllexport)
#else
#define YAMANGDXDLL_API __declspec(dllimport)
#endif

#include "MeshObject.h"
struct CUSTOMVERTEX;

extern "C"{
	//파일 이름 넣어 mesh object 받아오기
	YAMANGDXDLL_API HRESULT InitCharacterModel( HWND hWnd, LPCTSTR fileName, MESHOBJECT* inputVal );

	//Main rendering MeshObject를 넣어 render 진행
	YAMANGDXDLL_API void Rendering( MESHOBJECT* inputVal );
	YAMANGDXDLL_API void SetEffect( bool isEffect = true );

	//rendering 후 디바이스 endScene
	YAMANGDXDLL_API void PostRendering();

	//각 object 해제시 사용
	YAMANGDXDLL_API void MeshObjectCleanUp( MESHOBJECT* inputVal );

	YAMANGDXDLL_API void SetWorldAreaMatrix( D3DXMATRIXA16* matrix );
	
	YAMANGDXDLL_API void SetCameraMatrix( D3DXMATRIXA16* matrix );
	YAMANGDXDLL_API void SetCameraView(float x = 0.f, float y = 0.f, float z = 0.f);
	YAMANGDXDLL_API void SetAspectRatio( long width, long height );

	//////////////////////////////////////////////////////////////////////////
	// Shader
	//////////////////////////////////////////////////////////////////////////

	YAMANGDXDLL_API HRESULT ShaderCreate( int size );
	YAMANGDXDLL_API HRESULT ShaderImport( LPCTSTR effectFile, int id );

	//////////////////////////////////////////////////////////////////////////
	// Billboard
	//////////////////////////////////////////////////////////////////////////

	YAMANGDXDLL_API void DrawBillboardByTexture( int id, char flag );
	YAMANGDXDLL_API HRESULT MeshTextureCreateBySize( INT size );
	YAMANGDXDLL_API HRESULT MeshTextureImport( LPCTSTR meshTexture, int id );

	//////////////////////////////////////////////////////////////////////////
	// height map 출력을 위한 함수
	//////////////////////////////////////////////////////////////////////////

	YAMANGDXDLL_API HRESULT HeightMapTextureImport( HWND hWnd, LPCTSTR heightMap, LPCTSTR mapTexture );
	YAMANGDXDLL_API HRESULT MapToolTextureImport( HWND hWnd, LPCTSTR toolTexture );

	YAMANGDXDLL_API HRESULT InitHeightMap( LPCTSTR fileName, float pixelSize );
	
	YAMANGDXDLL_API void InitGroundMesh( int row, int col );
	YAMANGDXDLL_API void CreateRawGround( int row, int col, float pixelSize , bool makeFile = false);

	YAMANGDXDLL_API void GetHeightMapSizeForQuadTree( DWORD* width, DWORD* height );
	YAMANGDXDLL_API void GetHeightMapForQuadTree( CUSTOMVERTEX** heightMap );
	YAMANGDXDLL_API HRESULT PreRenderHeightWithMapQuadTree( LPDWORD* index );
	YAMANGDXDLL_API void RenderHeightMapWithQuadTree( int tris, bool isWire = false );

	YAMANGDXDLL_API void ToolViewSetting( int width, int height );
	YAMANGDXDLL_API void RenderHeightMap();
	
	YAMANGDXDLL_API void HeightMapCleanup( );
	
	//////////////////////////////////////////////////////////////////////////
	// Picking Ray를 위한 함수
	//////////////////////////////////////////////////////////////////////////
	YAMANGDXDLL_API void CalcPickingRay( int mouseX, int mouseY );
	YAMANGDXDLL_API HRESULT TransPickedTriangle( float* pickedX, float* pickedZ, int* PickedTriPointA = nullptr, int* PickedTriPointB = nullptr, int* PickedTriPointC = nullptr );
	YAMANGDXDLL_API HRESULT TransPickedTriangleQuadTree( float* pickedX, float* pickedZ, int* PickedTriPointA = nullptr, int* PickedTriPointB = nullptr, int* PickedTriPointC = nullptr );

	YAMANGDXDLL_API void MapToolPickingEvent( int modeSelector, int PickedTriPointA, int PickedTriPointB, int PickedTriPointC );
	
	//////////////////////////////////////////////////////////////////////////
	//Text Render 를 위한 함수
	//////////////////////////////////////////////////////////////////////////
	YAMANGDXDLL_API void RenderText( LPCWSTR text, float left, float top, int RGB_R = 255, int RGB_G = 255, int RGB_B = 255, float right = 0, float bottom = 0 );

	//////////////////////////////////////////////////////////////////////////
	// D3D Cursor Set을 위한 함수
	//////////////////////////////////////////////////////////////////////////
	YAMANGDXDLL_API HRESULT InitCursor( int textureSize, int cursorPosX = 0, int cursorPosY = 0 );
	YAMANGDXDLL_API HRESULT CreateCursorImage( LPCWSTR cursorImagePath = L"cursor_default.png", int cursorType = 0 );
	YAMANGDXDLL_API HRESULT ChangeCursorImage( int cursorType = 0 );
	YAMANGDXDLL_API HRESULT RenderCursor();
	YAMANGDXDLL_API void CursorCleanUp();
	YAMANGDXDLL_API void SetCursorPosition( int PosX = 0, int PosY = 0 );

	//////////////////////////////////////////////////////////////////////////
	// ScreenShot
	//////////////////////////////////////////////////////////////////////////
	YAMANGDXDLL_API void TakeScreenShot();

	//////////////////////////////////////////////////////////////////////////
	//  UI
	//////////////////////////////////////////////////////////////////////////
	YAMANGDXDLL_API HRESULT InitUISprite( int textureSize );
	YAMANGDXDLL_API HRESULT CreateSpriteImage( LPCWSTR ImagePath = L"UI_default.png", INT typeNum = 0 );
	YAMANGDXDLL_API HRESULT RenderSprite( INT typeNum, D3DXVECTOR3 pos );
	YAMANGDXDLL_API void UICleanUp();
}
