#include <d3dx9.h>

struct MESHOBJECT
{
	//vertexbuffer를 직접 만들어서 사용하는 경우는 없을 것이므로 삭제
	//LPDIRECT3DVERTEXBUFFER9 VertexBuffer = nullptr;

	LPD3DXMESH importedMesh = nullptr;
	D3DMATERIAL9* MeshMarterials = nullptr;
	LPDIRECT3DTEXTURE9* MeshTexture = nullptr;

	DWORD NumMaterials = 0L;
};