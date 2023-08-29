/*
 * @Author: YizhenZhang yz4401@columbia.edu
 */


#include "../_public.h"
 
int main(int argc,char *argv[])
{
  if (argc!=2)
  {
    printf("Using:./demo08new port\nExample:./demo08new 5005\n\n"); return -1;
  }

  CTcpServer TcpServer;

  if (TcpServer.InitServer(atoi(argv[1])) == false){
    printf("TcpServer.InitServer(%s) failed.\n", argv[1]); return -1;
  }

  if (TcpServer.Accept() == false){
    printf("TcpServer.Accept() failed. \n");
    return -1;
  }

  printf("Client (%s) connected. \n", TcpServer.GetIP());
 
  char buffer[102400];

  // 第5步：与客户端通讯，接收客户端发过来的报文后，回复ok。
  while (1)
  {
    memset(buffer,0,sizeof(buffer));
    if (TcpServer.Read(buffer) == false) break;
    printf("Recieved: %s\n",buffer);

    strcpy(buffer,"ok");

    if(TcpServer.Write(buffer) == false) break;
    printf("Sent: %s\n",buffer);
  }
 
  // 第6步：关闭socket，释放资源。
}
