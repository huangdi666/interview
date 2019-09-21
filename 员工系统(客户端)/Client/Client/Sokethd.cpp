#include "Sokethd.h"
#pragma comment (lib,"ws2_32.lib")

CSokethd::CSokethd(void)
{
	WSAData wd;
	WSAStartup(0x0202,&wd); //构建时启动(更方便)
	m_hSocket = INVALID_SOCKET;//初始化
}


CSokethd::~CSokethd(void)
{
	Close();
}

BOOL CSokethd::Creat(UINT nSocketPort,int nSocketType,LPCTSTR lpszSocketAddress)
{//socket&bind

	m_hSocket = socket(AF_INET,nSocketType,0);//创建一个套接字(句柄)
	if(m_hSocket == INVALID_SOCKET)
		return FALSE;
	sockaddr_in sa = {AF_INET,htons(nSocketPort)};
	if(lpszSocketAddress)
		sa.sin_addr.s_addr = inet_addr(lpszSocketAddress);
	return !bind(m_hSocket,(sockaddr *)&sa,sizeof(sa));
}

int CSokethd::SendTo(const void* lpBuf,int nBufLen,UINT nHostPort,LPCTSTR lpszSocketAddress)
{
	sockaddr_in sa = {AF_INET,htons(nHostPort)};
	if(lpszSocketAddress)
		sa.sin_addr.s_addr = inet_addr(lpszSocketAddress);
	sendto(m_hSocket,(LPCSTR)lpBuf,nBufLen,0,(sockaddr*)&sa,sizeof(sa));
	return 0;
}
int CSokethd::ReceiverFrom(void* lpBuf,int nBufLen,char *rSocketAddress,UINT& rSocketPort)
{
	sockaddr_in sa = {AF_INET};
	int nLen = sizeof(sa);
	int nRet = recvfrom(m_hSocket,(char*)lpBuf,nBufLen,0,(sockaddr*)&sa,(int*)&rSocketPort);
	if(nRet>0)
	{
		rSocketPort = htons(sa.sin_port);
		strcpy(rSocketAddress,inet_ntoa(sa.sin_addr));
	}
	return 0;
}
BOOL CSokethd::Connect(LPCTSTR lpszHostAddress,UINT nHostPort)
{
	sockaddr_in sa = {AF_INET,htons(nHostPort)};
	sa.sin_addr.S_un.S_addr = inet_addr(lpszHostAddress);
	return !connect(m_hSocket,(sockaddr*)&sa,sizeof(sa));
}
BOOL CSokethd::Accept(CSokethd &socka,LPSTR szIP,UINT *nPort)
{
	sockaddr_in sa = {AF_INET};
	int nlen = sizeof(sa);
	socka.m_hSocket = accept(this->m_hSocket,(sockaddr*)&sa,&nlen);
	if(INVALID_SOCKET == socka.m_hSocket)
		return FALSE;
	if(szIP)
		strcpy(szIP,inet_ntoa(sa.sin_addr));
	if(nPort)
		*nPort = htons(sa.sin_port);
	return TRUE;

}

