/*
 * @Author: YizhenZhang
 * @Date: 2023-08-25 16:17:42
 */
#include "_public.h"

// 程序退出和信号2、15的处理函数。
void EXIT(int sig);

int main(int argc,char *argv[])
{
  // 程序的帮助。
  if (argc != 4)
  {
    printf("\n");
    printf("Using:./deletefiles pathname matchstr timeout\n\n");

    printf("Example:sudo ./deletefiles /log/idc \"*.log.20*\" 0.02\n");
    printf("        sudo ./deletefiles /tmp/idc/surfdata \"*.xml,*.json\" 0.01\n");
    printf("        sudo ./procctl 300 /project/tools1/bin/deletefiles /log/idc \"*.log.20*\" 0.02\n");
    printf("        sudo ./procctl 300 /project/tools1/bin/deletefiles /tmp/idc/surfdata \"*.xml,*.json\" 0.01\n\n");


    return -1;
  }

  // 关闭全部的信号和输入输出。
  // 设置信号,在shell状态下可用 "kill + 进程号" 正常终止些进程。
  // 但请不要用 "kill -9 +进程号" 强行终止。
  CloseIOAndSignal(true); signal(SIGINT,EXIT);  signal(SIGTERM,EXIT);

  // 获取文件超时的时间点。
  char strTimeOut[21];
  LocalTime(strTimeOut,"yyyy-mm-dd hh24:mi:ss",0-(int)(atof(argv[3])*24*60*60));

  CDir Dir;
  // 打开目录，CDir.OpenDir()
  if (Dir.OpenDir(argv[1],argv[2],10000,true)==false)
  {
    printf("Dir.OpenDir(%s) failed.\n",argv[1]); return -1;
  }



  // 遍历目录中的文件名。
  while (true)
  {
    // 得到一个文件的信息，CDir.ReadDir()
    if (Dir.ReadDir()==false) break;
  
    // 与超时的时间点比较，如果更早，就需要压缩
    if ( (strcmp(Dir.m_ModifyTime,strTimeOut)<0))
    {
      if (REMOVE(Dir.m_FullFileName)==0) 
        printf("REMOVE %s ok.\n",Dir.m_FullFileName);
      else
        printf("REMOVE %s failed.\n",Dir.m_FullFileName);
    }
  }

  return 0;
}

void EXIT(int sig)
{
  printf("程序退出,sig=%d\n\n",sig);

  exit(0);
}