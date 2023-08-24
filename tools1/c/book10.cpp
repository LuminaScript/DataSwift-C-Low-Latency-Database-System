/*
 * @Author: YizhenZhang yz4401@columbia.edu
 * @Date: 2023-08-23 14:41:10
 * @FilePath: /Desktop/project/tools1/c/book10.cpp
 * @Description: ...
 */
#include "_public.h"

void EXIT(int sig)
{
        // will not call deconstructor function for local variable
        printf("sig=%d\n", sig);
        if(sig == 2) exit(0);
}

CPActive PActive; //constructor

int main(int argc, char *argv[])
{
        if(argc != 3) 
        {
                printf("Using: ./book procname timeout\n");
                return 0;
        }
        signal(2, EXIT);
        signal(15, EXIT);
        PActive.AddPInfo(atoi(argv[2]), argv[1]); //初始化
        while (true)
        {
                //PActive.UptATime(); //更新共享内存中的心跳信息
                sleep(10);
        }

        return 0;

}


