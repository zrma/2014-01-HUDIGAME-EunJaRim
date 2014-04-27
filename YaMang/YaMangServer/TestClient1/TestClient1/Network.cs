using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;

namespace TestClient1
{
    public static class Network
    {
        public static Socket socket;

        public const String host = "127.0.0.1";
        public const int port = 9001;

        public static bool ConnectServer()
        {
            IPAddress serverIP = IPAddress.Parse(host);
            IPEndPoint serverEndPoint = new IPEndPoint(serverIP, port);
            socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            try
            {
                socket.Connect(serverEndPoint);
                return true;
            }
            catch (Exception)
            {
                return false;
            }
            
            
        }

        public static unsafe bool SendLoginRequest(int playerID)
        {
            PacketStruct.LoginRequest loginRequest = new PacketStruct.LoginRequest();
            loginRequest.m_Size = (short)sizeof(PacketStruct.LoginRequest);
            loginRequest.m_Type = (short)PacketTypes.PKT_CS_LOGIN;
            loginRequest.m_PlayerId = playerID;
            byte[] sendData = RawSerializer<PacketStruct.LoginRequest>.RawSerialize(loginRequest);
            socket.Send(sendData, 0, sendData.Length, SocketFlags.None);

            // receive를 넣어서 결과 처리해줄려했는데 귀찮다. 일단 그냥 송신만
            return true;
        }

        public static unsafe void SendGameOverRequest(int playerID, bool isWon )
        {
            PacketStruct.GameOverRequest request = new PacketStruct.GameOverRequest();
            request.m_Size = (short)sizeof(PacketStruct.GameOverRequest);
            request.m_Type = (short)PacketTypes.PKT_CS_GAMEOVER;
            request.m_PlayerId = playerID;
            request.m_IsWon = isWon;

            byte[] sendData = RawSerializer<PacketStruct.GameOverRequest>.RawSerialize(request);

            socket.Send(sendData, 0, sendData.Length, SocketFlags.None);
        }


        public static unsafe void SendRoomCreateRequest(int playerID)
        {
            PacketStruct.RoomCreateRequest request = new PacketStruct.RoomCreateRequest();
            request.m_Size = (short)sizeof(PacketStruct.RoomCreateRequest);
            request.m_Type = (short)PacketTypes.PKT_CS_ROOM_CREATE;
            request.m_PlayerId = playerID;

            byte[] sendData = RawSerializer<PacketStruct.RoomCreateRequest>.RawSerialize(request);

            socket.Send(sendData, 0, sendData.Length, SocketFlags.None);
        }

        public static unsafe void SendEnterRoomRequest(int roomNumber)
        {
            PacketStruct.EnterRoomRequest request = new PacketStruct.EnterRoomRequest();
            request.m_Size = (short)sizeof(PacketStruct.EnterRoomRequest);
            request.m_Type = (short)PacketTypes.PKT_CS_ENTER_ROOM;
            request.m_RoomNumber = roomNumber;

            byte[] sendData = RawSerializer<PacketStruct.EnterRoomRequest>.RawSerialize(request);

            socket.Send(sendData, 0, sendData.Length, SocketFlags.None);
        }

        public static unsafe void SendLeaveRoomRequest(int roomNumber)
        {
            PacketStruct.LeaveRoomRequest request = new PacketStruct.LeaveRoomRequest();
            request.m_Size = (short)sizeof(PacketStruct.LeaveRoomRequest);
            request.m_Type = (short)PacketTypes.PKT_CS_LEAVE_ROOM;
            request.m_RoomNumber = roomNumber;

            byte[] sendData = RawSerializer<PacketStruct.LeaveRoomRequest>.RawSerialize(request);

            socket.Send(sendData, 0, sendData.Length, SocketFlags.None);
        }

        public static unsafe void SendGenerateCorpsRequest(
              byte unitType
            , float eyeX
            , float eyeY
            , float eyeZ
            , float lookX
            , float lookY
            , float lookZ)
        {
            

            PacketStruct.GenerateCorpsRequest request = new PacketStruct.GenerateCorpsRequest();
            request.m_Size = (short)sizeof(PacketStruct.GenerateCorpsRequest);
            request.m_Type = (short)PacketTypes.PKT_CS_GENERATE_CORPS;
            request.m_UnitType = unitType;
            request.m_EyeX = eyeX;
            request.m_EyeY = eyeY;
            request.m_EyeZ = eyeZ;
            request.m_LookX = lookX;
            request.m_LookY = lookY;
            request.m_LookZ = lookZ;

            byte[] sendData = RawSerializer<PacketStruct.GenerateCorpsRequest>.RawSerialize(request);

            socket.Send(sendData, 0, sendData.Length, SocketFlags.None);
        }




        public static unsafe void SendMoveCorpsRequest(
        int corpsID
        , float eyeX
        , float eyeY
        , float eyeZ
        , float lookX
        , float lookY
        , float lookZ)
        {


            PacketStruct.MoveCorpsRequest request = new PacketStruct.MoveCorpsRequest();
            request.m_Size = (short)sizeof(PacketStruct.MoveCorpsRequest);
            request.m_Type = (short)PacketTypes.PKT_CS_MOVE_CORPS;
            request.m_CorpsID = corpsID;
            request.m_EyeX = eyeX;
            request.m_EyeY = eyeY;
            request.m_EyeZ = eyeZ;
            request.m_LookX = lookX;
            request.m_LookY = lookY;
            request.m_LookZ = lookZ;

            byte[] sendData = RawSerializer<PacketStruct.MoveCorpsRequest>.RawSerialize(request);

            socket.Send(sendData, 0, sendData.Length, SocketFlags.None);
        }



        public static unsafe void SendStopCorpsRequest(
        int corpsID)
        {
            PacketStruct.StopCorpsRequest request = new PacketStruct.StopCorpsRequest();
            request.m_Size = (short)sizeof(PacketStruct.StopCorpsRequest);
            request.m_Type = (short)PacketTypes.PKT_CS_STOP_CORPS;
            request.m_CorpsID = corpsID;

            byte[] sendData = RawSerializer<PacketStruct.StopCorpsRequest>.RawSerialize(request);

            socket.Send(sendData, 0, sendData.Length, SocketFlags.None);
        }


        public static unsafe void SendChangeCorpsFormationRequest(
            int corpsID
            , byte formationType)
        {
            PacketStruct.ChangeCorpsFormationRequest request = new PacketStruct.ChangeCorpsFormationRequest();
            request.m_Size = (short)sizeof(PacketStruct.ChangeCorpsFormationRequest);
            request.m_Type = (short)PacketTypes.PKT_CS_CORPS_CHANGE_FORMATION;
            request.m_CorpsID = corpsID;
            request.m_FormationType = formationType;

            byte[] sendData = RawSerializer<PacketStruct.ChangeCorpsFormationRequest>.RawSerialize(request);

            socket.Send(sendData, 0, sendData.Length, SocketFlags.None);
        }


        //////////////////////////////////////////////////////////////////////
    }










    public static class RawSerializer<T>
    {
        public static T RawDeserialize(byte[] rawData)
        {
            return RawDeserialize(rawData, 0);
        }

        public static T RawDeserialize(byte[] rawData, int position)
        {
            int rawsize = Marshal.SizeOf(typeof(T));
            if (rawsize > rawData.Length)
                return default(T);

            IntPtr buffer = Marshal.AllocHGlobal(rawsize);
            Marshal.Copy(rawData, position, buffer, rawsize);
            T obj = (T)Marshal.PtrToStructure(buffer, typeof(T));
            Marshal.FreeHGlobal(buffer);
            return obj;
        }

        public static byte[] RawSerialize(T item)
        {
            int rawSize = Marshal.SizeOf(typeof(T));
            IntPtr buffer = Marshal.AllocHGlobal(rawSize);
            Marshal.StructureToPtr(item, buffer, false);
            byte[] rawData = new byte[rawSize];
            Marshal.Copy(buffer, rawData, 0, rawSize);
            Marshal.FreeHGlobal(buffer);
            return rawData;
        }
    }
}
