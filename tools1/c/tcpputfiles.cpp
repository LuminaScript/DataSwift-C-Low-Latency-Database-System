/*
 * Program Name @tcpputfiles.cpp，
*/
#include "_public.h"

CTcpClient TcpClient;

bool srv000();    // 心跳。
bool srv001();    // 登录业务。
 
int main(int argc,char *argv[])
{
  if (argc!=3)
  {
    printf("Using:./tcpputfiles ip port\nExample:./tcpputfiles 127.0.0.1 5005\n\n"); return -1;
  }

  // 向服务端发起连接请求。
  if (TcpClient.ConnectToServer(argv[1],atoi(argv[2]))==false)
  {
    printf("TcpClient.ConnectToServer(%s,%s) failed.\n",argv[1],argv[2]); return -1;
  }

  // 登录业务。
  if (srv001()==false) { printf("srv001() failed.\n"); return -1; }

  for (int ii=3;ii<5;ii++)
  {
    if (srv000()==false) break;

    sleep(ii);
  }

  return 0;
}

// 心跳。 
bool srv000()    
{
  char buffer[1024];
 
  SPRINTF(buffer,sizeof(buffer),"<srvcode>0</srvcode>");
  printf("发送：%s\n",buffer);
  if (TcpClient.Write(buffer)==false) return false; // 向服务端发送请求报文。

  memset(buffer,0,sizeof(buffer));
  if (TcpClient.Read(buffer)==false) return false; // 接收服务端的回应报文。
  printf("接收：%s\n",buffer);

  return true;
}

// 登录业务。 
bool srv001()    
{
  char buffer[1024];
 
  SPRINTF(buffer,sizeof(buffer),"<srvcode>1</srvcode><tel>1392220000</tel><password>123456</password>");
  printf("发送：%s\n",buffer);
  if (TcpClient.Write(buffer)==false) return false; // 向服务端发送请求报文。

  memset(buffer,0,sizeof(buffer));
  if (TcpClient.Read(buffer)==false) return false; // 接收服务端的回应报文。
  printf("接收：%s\n",buffer);

  // 解析服务端返回的xml。
  int iretcode=-1;
  GetXMLBuffer(buffer,"retcode",&iretcode);
  if (iretcode!=0) { printf("登录失败。\n"); return false; }

  printf("登录成功。\n"); 

  return true;
}




 


