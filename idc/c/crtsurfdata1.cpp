/*
 * @Author: YizhenZhang yz4401@columbia.edu
 * @Date: 2023-08-16 01:54:51
 */

#include "_public.h"
CLogFile logfile(10);

int main(int argc, char *argv[]){
 //inifile outpath logfile
 if(argc != 4){
  printf("Using: ./crtsurfdata1 inifile outpath logfile\n");
  printf("Example:/project/idc1/bin/crtsurfdata1 /project/idc1/ini/stcode.ini /tmp/surfdata /log/idc/crtsurfdata1.log\n\n");
  printf("inifile  全国气象站点参数文件名。\n");
  printf("outpath  全国气象站点数据文件存放的目录。\n");
  printf("logfile  本程序运行的日志文件名。\n");
  return -1;
 } 

 if(logfile.Open(argv[3]) == false)
 {
  printf("logfile.Open(%s) failed. \n", argv[3]);
  return -1;
 }
 logfile.Write("crtsurfdata1 starting ...\n");
 /*
 TO_DO
 */


 logfile.Write("crtsurfdata1 stopped ...\n");


 return 0;
}