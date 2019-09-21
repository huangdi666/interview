#pragma once
#include <WinSock2.h>
class CSokethd
{
protected:
	SOCKET m_hSocket;//���ĳ�Ա����
public:
	CSokethd(void);
	virtual ~CSokethd(void);
	BOOL Creat(UINT nSocketPort = 0,int nSockeType = SOCK_STREAM,LPCTSTR lpszSocketAddress = NULL);
	int SendTo(const void* lpBuf,int nBufLen,UINT nHostPort,LPCTSTR lpszSocketAddress);
	int ReceiverFrom(void* lpBuf,int nBufLen,char *rSocketAddress,UINT& rSocketPort);//�����ܽ������ݣ����ܽ���IP�Ͷ˿���Ϣ
	int Receive(void* lpBuf, int nBufLen,int nFlags = 0)
	{
		return recv(m_hSocket,(LPTSTR)lpBuf,nBufLen,nFlags);
	}
	BOOL Listen(int n = 5)
	{
		return !listen(m_hSocket,n);
	}
	BOOL Connect(LPCTSTR,UINT);
	BOOL Accept(CSokethd &,LPSTR,UINT*);
	int Send(const void* lpBuf,int nBufLen,int nFlags =0)
	{
		return send(m_hSocket,(LPCTSTR)lpBuf,nBufLen,nFlags);
	}
	void Close()
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}

};

