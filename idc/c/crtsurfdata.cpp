/*
 * @Author: YizhenZhang yz4401@columbia.edu
 * @Date: 2023-08-23 01:54:51
 * @Program: crtsurfdata.cpp
 */

//task 1 增加生成历史数据文件的功能，为压缩文件和清理文件模块准备历史数据文件。ok

//task 2 增加信号处理函数，处理2和15的信号。 ok

//task 3 解决调用exit函数退出时局部对象没有调用析构两数的问题。

//task 4 把心跳信息写入共享内存。

#include "_public.h"
CPActive PActive; //进程心跳

struct st_stcode
{
        char provname[31]; // state
        char obtid[11]; // station number
        char obtname[31]; // station name
        double lat; // latitude
        double lon; // longtitude
        double height; // height of station
};


// vector to hold climate station data
vector<struct st_stcode> vstcode;

// function to load vector
bool LoadSTCode(const char *inifile);

// Structure for minute-by-minute observation data from national meteorological stations
struct st_surfdata
{
  char obtid[11];      // Station code.
  char ddatetime[21];  // Data time: format yyyymmddhh24miss
  int  t;              // Temperature: in 0.1°C units.
  int  p;              // Atmospheric pressure: in 0.1 hPa units.
  int  u;              // Relative humidity: values between 0-100.
  int  wd;             // Wind direction: values between 0-360.
  int  wf;             // Wind speed: in 0.1 m/s units.
  int  r;              // Rainfall: in 0.1mm units.
  int  vis;            // Visibility: in 0.1 meter units.
};

vector<struct st_surfdata> vsurfdata;

char strddatetime[21]; 

// 模拟生成全国气象站点的容器
void CrtSurfData();

CFile File;

// transfer climate data from vector into file
bool CrtSurfFile(const char *outpath, const char *datafmt);

CLogFile logfile(10);

void EXIT(int sig); // 信号处理函数

int main(int argc, char *argv[]){
 //inifile outpath logfile
 if(argc != 5 && argc != 6){
  printf("Using: sudo ./crtsurfdata inifile outpath logfile datafmt [datatime(optional)]\n");
  printf("Example: sudo ./crtsurfdata /home/yizhen/Desktop/project/idc/ini/stcode.ini /tmp/idc/surfdata /log/idc/crtsurfdata.log xml, json, csv 2021071023000\n\n");
  printf("inifile: National meteorological station parameter filename.\n");
  printf("outpath: Directory where the national meteorological station data files are stored.\n");
  printf("logfile: Log filename of this program's execution.\n");

  return -1;
 } 

 //不能放在打开日志的后面，否则会关闭日志的IO
 CloseIOAndSignal(true);signal(SIGINT, EXIT);signal(SIGTERM,EXIT);


 if(logfile.Open(argv[3]) == false)
 {
  printf("logfile.Open(%s) failed. \n", argv[3]);
  return -1;
 }
 logfile.Write("crtsurfdata starting ...\n");

 PActive.AddPInfo(20,"crtsurfdata" ); // do not need to update hearbeat beacause the program runs too short

if (LoadSTCode(argv[1]) == false)
        return -1;
memset(strddatetime, 0, sizeof(strddatetime));
if (argc == 5)
        LocalTime(strddatetime, "yyyymmddhh24miss");
else
        STRCPY(strddatetime, sizeof(strddatetime), argv[5]);

CrtSurfData();

if(strstr(argv[4], "xml") != 0) CrtSurfFile(argv[2], "xml");

if(strstr(argv[4], "json") != 0) CrtSurfFile(argv[2], "json");

if(strstr(argv[4], "csv") != 0) CrtSurfFile(argv[2], "csv");

 logfile.Write("crtsurfdata stopped ...\n");


 return 0;
}

bool LoadSTCode(const char *inifile)
{
        // open station data file
        if (File.Open(inifile, "r") == false)
        {
                ::std::cout << "Opened failed\n";
                logfile.Write("File.Open(%s) failed.\n", inifile);
                return false;
        }

        char strBuffer[301];

        CCmdStr CmdStr;

        struct st_stcode stcode;

        while(true)
        {
                if(File.Fgets(strBuffer, 300, true) == false) break;

                logfile.Write("=%s=\n", strBuffer);

                // split what we've read
                CmdStr.SplitToCmd(strBuffer, ",",true);
                
                if(CmdStr.CmdCount() != 6) continue;

                CmdStr.GetValue(0, stcode.provname, 30);
                CmdStr.GetValue(1, stcode.obtid, 30);
                CmdStr.GetValue(2, stcode.obtname, 10);
                CmdStr.GetValue(3, &stcode.lat);
                CmdStr.GetValue(4, &stcode.lon);
                CmdStr.GetValue(5, &stcode.height);

                vstcode.push_back(stcode);
        }
        return true;
}

//模拟生成全国气象站点分钟观测数据，存放在vsurfdata容器中
void CrtSurfData()
{
        srand(time(0));

        char strddatetime[21];
        memset(strddatetime, 0, sizeof(strddatetime));
        LocalTime(strddatetime, "yyyymmddhh24miss");
        struct st_surfdata stsurfdata;

        for(int i = 0; i < vstcode.size(); i++)
        {
                memset(&stsurfdata, 0, sizeof(struct st_surfdata));
                strncpy(stsurfdata.obtid, vstcode[i].obtid, 10);
                strncpy(stsurfdata.ddatetime, strddatetime, 14);
                stsurfdata.t = rand() % 351;          // Temperature: in 0.1°C units.
                stsurfdata.p = rand() % 265 + 10000;  // Atmospheric pressure: in 0.1 hPa units.
                stsurfdata.u = rand() % 100 + 1;      // Relative humidity: values between 0-100.
                stsurfdata.wd = rand() % 360;         // Wind direction: values between 0-360.
                stsurfdata.wf = rand() % 150;         // Wind speed: in 0.1 m/s units.
                stsurfdata.r = rand() % 16;           // Rainfall: in 0.1mm units.
                stsurfdata.vis = rand() % 5001 + 100000; // Visibility: in 0.1 meter units.
    

                vsurfdata.push_back(stsurfdata);
        }
}

bool CrtSurfFile(const char *outpath, const char *datafmt)
{
        /*
        temporary file - file content conflict with read-write
        */
       CFile File;
        // generate file name
        char strFileName[301];
        snprintf(strFileName, sizeof(strFileName), "%s/SURF_ZH_%s_%d.%s", outpath, strddatetime, getpid(), datafmt);

        // open file
        if(File.OpenForRename(strFileName, "w") == false)
        {
                logfile.Write("File.OpenForRename(%s) failed.\n", strFileName);
                return false;
        }

        // write first-line title
        if (strcmp(datafmt, "csv") == 0){
                File.Fprintf("站点代码,数据时间,气温,气压,相对湿度,风向,风速,降雨量,能见度\n");
        }
        



        // traverse vsurfdata container
        for(int ii = 0; ii < vsurfdata.size(); ii++)
        {
                if(strcmp(datafmt, "csv") == 0) 
                {
                        File.Fprintf("%s,%s,%.1f,%.1f,%d,%d,%.1f,%.1f,%.1f\n", \
                        vsurfdata[ii].obtid,vsurfdata[ii].ddatetime,vsurfdata[ii].t/10.0,vsurfdata[ii].p/10.0, \
                        vsurfdata[ii].u,vsurfdata[ii].wd,vsurfdata[ii].wf/10.0,vsurfdata[ii].r/10.0,vsurfdata[ii].vis/10.0);
                }
                
                if (strcmp(datafmt,"xml")==0)
                {
                        File.Fprintf("<obtid>%s</obtid><ddatetime>%s</ddatetime><t>%.1f</t><p>%.1f</p>"\
                        "<u>%d</u><wd>%d</wd><wf>%.1f</wf><r>%.1f</r><vis>%.1f</vis><endl/>\n",\
                        vsurfdata[ii].obtid,vsurfdata[ii].ddatetime,vsurfdata[ii].t/10.0,vsurfdata[ii].p/10.0,\
                        vsurfdata[ii].u,vsurfdata[ii].wd,vsurfdata[ii].wf/10.0,vsurfdata[ii].r/10.0,vsurfdata[ii].vis/10.0);
                }

                if (strcmp(datafmt,"json")==0)
                {
                        File.Fprintf("{\"obtid\":\"%s\",\"ddatetime\":\"%s\",\"t\":\"%.1f\",\"p\":\"%.1f\","\
                                        "\"u\":\"%d\",\"wd\":\"%d\",\"wf\":\"%.1f\",\"r\":\"%.1f\",\"vis\":\"%.1f\"}",\
                                vsurfdata[ii].obtid,vsurfdata[ii].ddatetime,vsurfdata[ii].t/10.0,vsurfdata[ii].p/10.0,\
                                vsurfdata[ii].u,vsurfdata[ii].wd,vsurfdata[ii].wf/10.0,vsurfdata[ii].r/10.0,vsurfdata[ii].vis/10.0);
                        if (ii<vsurfdata.size()-1) File.Fprintf(",\n");
                        else File.Fprintf("\n");
                }


        }

        if (strcmp(datafmt,"xml")==0) File.Fprintf("<data>\n");
        if (strcmp(datafmt,"json")==0) File.Fprintf("{\"data\":[\n");

        sleep(10);
        // close file
        File.CloseAndRename();

        UTime(strFileName, strddatetime); // change file's time attribute

        // write success log
        logfile.Write("Successfully generated data file %s, data time %s, record data %d. \n", strFileName, strddatetime, vsurfdata.size());

        return true;
}

//程序退出和信号2、15信号处理代码
void EXIT(int sig){
        logfile.Write("程序退出, sig = %d\n\n", sig);
        exit(0);
}