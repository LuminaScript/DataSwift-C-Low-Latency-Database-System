/*
 * @Author: YizhenZhang
 * @Date: 2023-08-23 10:52:20
 * @FilePath: /project/tools1/c/procctl.cpp
 * @Description: 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[])
{

        if(argc < 3)
        {
                printf("Usage:\n");
                printf("    ./procctl <timetvl> <program> [argv1] [argv2] ... [argvN]\n\n");

                printf("Description:\n");
                printf("    This program serves as a scheduler for service programs and shell scripts. It periodically launches specified programs or scripts based on a given time interval.\n\n");

                printf("Arguments:\n");
                printf("    timetvl:  The execution interval, specified in seconds. After the scheduled program completes, it will be re-executed by procctl after this interval elapses.\n");
                printf("    program:  The full path of the program or shell script to be scheduled.\n");
                printf("    argvs:    Optional arguments for the scheduled program or script.\n\n");

                printf("Example:\n");
                printf("    sudo ./procctl 5 /usr/bin/tar zcvf /tmp/tmp.tgz /usr/include\n\n");

                printf("Notes:\n");
                printf("    1. This program is resistant to standard 'kill' commands but can be forcibly terminated using 'kill -9'.\n");
                printf("    2. Ensure you have the necessary permissions to execute the scheduled program or script.\n\n");

                return -1;
        }

        // close signal interruption & IO
        for(int ii = 0; ii < 64; ii++)
        {
                signal(ii, SIG_IGN);
                close(ii);
        }

        if(fork() != 0) exit(0);

        signal(SIGCHLD, SIG_DFL);

        char *pargv[argc];
        for(int ii = 2; ii < argc; ii++)
        {
                pargv[ii - 2] = argv[ii];
        }
        pargv[argc - 2] = NULL;

        while(true)
        {
                if(fork() == 0)
                {
                        execv(argv[2], pargv);
                        exit(0);//prevent execv failure & infinite loop
                }
                else
                {
                        int status;
                        wait(&status);
                        sleep(atoi(argv[1]));
                }
        }
        return 0;
}