#ifndef __NET_H__
#define __NET_H__

//ws2_32.lib
#ifdef WIN32
#include <WinSock2.h>
#else
// in linux, android, ios
#endif

// 
class Net
{
public:
	static SOCKET _server;
	static SOCKET _connet;

	static int _isConnected;
	static int _isRecvComplete;

	static char* _recvData;

	static bool Listen(short port = 9999);
	static bool isConnected();

	static bool Connect(const char* ip, short port = 9999);
	static int Send(const char* buffer, int len);

	// 接收数据的接口
	static bool RecvStart();
	static bool isRecvComplete();
	static char* RecvData(int& len);

	//
	static DWORD WINAPI AcceptThreadFunc(void* arg);
	static DWORD WINAPI RecvThreadFunc(void* arg);
};

#endif // __NET_H__


