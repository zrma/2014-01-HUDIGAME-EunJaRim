using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace TestClient1
{

    enum PacketTypes : short
    {
        PKT_NONE = 0,

        PKT_CS_LOGIN = 1,
        PKT_SC_LOGIN = 2,

        PKT_CS_CHAT = 3,
        PKT_SC_CHAT = 4,

        PKT_CS_GAMEOVER = 5,
        PKT_SC_GAMEOVER = 6,

        PKT_CS_ROOM_CREATE = 7,
        PKT_SC_ROOM_CREATE = 8,

        PKT_CS_ENTER_ROOM = 9,
        PKT_SC_ENTER_ROOM = 10,

        PKT_CS_LEAVE_ROOM = 11,
        PKT_SC_LEAVE_ROOM = 12,

        PKT_CS_GENERATE_CORPS = 100,
        PKT_SC_GENERATE_CORPS = 101,

        PKT_CS_MOVE_CORPS = 102,
        PKT_SC_MOVE_CORPS = 103,
        PKT_CS_STOP_CORPS = 104,
        PKT_SC_STOP_CORPS = 105,

        PKT_CS_CORPS_CHANGE_FORMATION = 200,
        PKT_SC_CORPS_CHANGE_FORMATION = 201,

        PKT_MAX = 1024
    };


    enum UnitType: int
    {
	UNIT_NONE = 0,

	UNIT_ARROW = 10,
	UNIT_GUARD = 20,
	UNIT_KNIGHT = 30,
	UNIT_PIKE = 40,
	UNIT_SWORD = 50,

	UNIT_MAX = 255
    };

    enum FormationType: int
    {
	FORMATION_NONE = 0,

	FORMATION_DEFENSE = 10,
	FORMATION_DESTROY = 20,
	FORMATION_RUSH = 30,

	FORMATION_MAX = 255
    };

    public static class PacketStruct
    {
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
        public unsafe struct LoginRequest
        {
            public short m_Size;
            public short m_Type;
            public int m_PlayerId;
        }


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
        public unsafe struct GameOverRequest
        {
            public short m_Size;
            public short m_Type;
            public int m_PlayerId;
            [MarshalAs(UnmanagedType.I1)]
            public bool m_IsWon;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
        public unsafe struct RoomCreateRequest
        {
            public short m_Size;
            public short m_Type;
            public int m_PlayerId;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
        public unsafe struct EnterRoomRequest
        {
            public short m_Size;
            public short m_Type;
            public int m_RoomNumber;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
        public unsafe struct LeaveRoomRequest
        {
            public short m_Size;
            public short m_Type;
            public int m_RoomNumber;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
        public unsafe struct GenerateCorpsRequest
        {
            public short m_Size;
            public short m_Type;
            public int m_UnitType;
            public float m_EyeX;
            public float m_EyeY;
            public float m_EyeZ;
            public float m_LookX;
            public float m_LookY;
            public float m_LookZ;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
        public unsafe struct MoveCorpsRequest
        {
            public short m_Size;
            public short m_Type;
            public int m_PlayerID;
            public int m_CorpsID;
            public float m_EyeX;
            public float m_EyeY;
            public float m_EyeZ;
            public float m_LookX;
            public float m_LookY;
            public float m_LookZ;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
        public unsafe struct StopCorpsRequest
        {
            public short m_Size;
            public short m_Type;
            public int m_PlayerID;
            public int m_CorpsID;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
        public unsafe struct ChangeCorpsFormationRequest
        {
            public short m_Size;
            public short m_Type;
            public int m_CorpsID;
            public int m_FormationType;
        }
    }
}
