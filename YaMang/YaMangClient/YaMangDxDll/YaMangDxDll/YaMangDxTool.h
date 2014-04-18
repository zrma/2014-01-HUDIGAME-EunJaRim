#ifdef YAMANGDXDLL_EXPORTS
#define YAMANGDXDLL_API __declspec(dllexport)
#else
#define YAMANGDXDLL_API __declspec(dllimport)
#endif

#include "MeshObject.h"

extern "C"{}