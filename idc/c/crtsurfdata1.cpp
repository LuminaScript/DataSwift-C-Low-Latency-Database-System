/*
 * @Author: YizhenZhang yz4401@columbia.edu
 * @Date: 2023-08-16 01:54:51
 */

#include "_public.h"
CLogFile logfile(10);

int main(int argc, char *argv[]){
 //inifile outpath logfile
 if(argc != 4){
  printf("Using: sudo ./crtsurfdata1 inifile outpath logfile\n");
  printf("Example: sudo ./crtsurfdata1 /project/idc1/ini/stcode.ini /tmp/surfdata /log/idc/crtsurfdata1.log\n\n");
  printf("inifile: National meteorological station parameter filename.\n");
  printf("outpath: Directory where the national meteorological station data files are stored.\n");
  printf("logfile: Log filename of this program's execution.\n");

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