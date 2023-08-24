/*
 * @Author: YizhenZhang yz4401@columbia.edu
 * @Date: 2023-08-23 16:13:24
 * @LastEditors: YizhenZhang yz4401@columbia.edu
 * @LastEditTime: 2023-08-24 14:13:41
 * @FilePath: /project/tools1/c/checkprofile.c
 * @Description: 
 */
#include "_public.h"

CLogFile logfile;

int main(int argc, char *argv[])
{
        //程序帮助文档
        if(argc != 2)
        {
                printf("\n");
                printf("Usage: sudo ./checkproc logfilename\n");
                printf("Example: sudo /project/tools1/bin/procctl 10 /project/tools1/bin/checkproc /tmp/log/checkproc.log\n\n");

                printf("This program is used to check whether a background service has timed out. If it has, it will terminate it.\n");
                printf("Note:\n");
                printf("1) This program should be launched by procctl, with a recommended run cycle of 10 seconds.\n");
                printf("2) To avoid accidental termination by regular users, this program should be run as the root user.\n\n\n");


                return 0;
        }

        // 忽略全部信号和IO，不希望程序被干扰
        CloseIOAndSignal(true);
        //打开日志文件
        if(logfile.Open(argv[1], "a+") == false)
        {
               printf("logfile.Open(%s) failed.\n", argv[1]);
               return -1;
        }

        int shmid = 0;

        //创建/获取共享内存
        if ((shmid = shmget((key_t)SHMKEYP, MAXNUMP * sizeof(struct st_procinfo), 0666 | IPC_CREAT)) == -1)
        {
                logfile.Write("Failed to create/get Shared Memory (%x). \n", SHMKEYP);
                return false;
        }

        //将共享内存链接到当前进程的地址空间
        struct st_procinfo *shm = (struct st_procinfo *) shmat(shmid, 0, 0);

        //遍历共享内存中全部的记录
        for(int ii = 0; ii < MAXNUMP; ii++)
        {
                //如果记录的pid == 0， 表示空记录，continue
                if (shm[ii].pid == 0) continue;

                //如果记录的pid 非0，表示是服务程序的心跳记录
                // logfile.Write("i=%d, pid=%d,pname=%s,timeout=%d,atime=%d\n", \
                ii, shm[ii].pid, shm[ii].pname, shm[ii].timeout, shm[ii].atime);
                
                //向进程发送信号0，判断是否存在，否则删除 continue
                int iret = kill(shm[ii].pid, 0); // return -1 of process non-exits

                if(iret == -1){
                        logfile.Write("进程pid=%d(%s)已经不存在。\n",(shm+ii)->pid,(shm+ii)->pname);
                        memset(shm+ii,0,sizeof(struct st_procinfo)); // 从共享内存中删除该记录。
                        continue;
                }

                
                time_t now = time(0);
                if (now-shm[ii].atime < shm[ii].timeout) continue;

                //如果已超时
                logfile.Write("进程pid=%d(%s)已超时。\n",(shm+ii)->pid,(shm+ii)->pname);

                //发送信号15， 尝试正常终止进程
                kill(shm[ii].pid, 15);

                for(int i = 0; i < 5; i++){
                        sleep(1);
                        iret = kill(shm[ii].pid, 0);
                        if(iret == -1) break;
                }

                
                if(iret == -1)
                        logfile.Write("进程pid=%d(%s)已超时。\n",(shm+ii)->pid,(shm+ii)->pname);
                else{
                        //如果进程仍存在，就发送信号9，强行终止
                        kill(shm[ii].pid, 9);
                        logfile.Write("进程pid=%d(%s)已强行终止。\n",(shm+ii)->pid,(shm+ii)->pname);

                }

                //从共享内存中删除已超时的进程的心跳记录
                memset(shm + ii, 0, sizeof(struct st_procinfo));
        }

        // 把共享内存从当前进程中分离。
        shmdt(shm);
        
        return 0;
}