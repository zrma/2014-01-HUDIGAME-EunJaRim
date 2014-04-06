using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace CharacterTool
{
    class YamangDll
    {
        ////////////////////////////////////////////////////////////
        // C++ 타입을 common language로 C#과 호환되도록 하기 - 
        // http://msdn.microsoft.com/en-us/library/Aa712982
        /// ////////////////////////////////////////////////////////

        [DllImport("yaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl )]
        public static extern Int32 InitD3D( IntPtr hWnd );

        [DllImport("yaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void D3DCleanUp();

        [DllImport("yaMangDxDll.dll",  CharSet = CharSet.Unicode , CallingConvention = CallingConvention.Cdecl)]
        public static extern Int32 InitGeometry( IntPtr hWnd, [MarshalAs(UnmanagedType.LPWStr)] String fileName, ref IntPtr inputVal);

        [DllImport("yaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void PreRendering(float x, float y, float z);

        [DllImport("yaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Rendering(ref IntPtr inputVal);

        [DllImport("yaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void PostRendering();

        [DllImport("yaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MeshObjectCleanUp( IntPtr inputVal );

        //DLL 내부 MESHOBJECT 정의
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct MESHOBJECT
        {
            IntPtr importedMesh;
            IntPtr MeshMarterials;
            IntPtr MeshTexture;

            UInt32 NumMaterials;
        };

    }
}
