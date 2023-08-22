/*
 * @Author: YizhenZhang yz4401@columbia.edu
 * @Date: 2023-08-16 01:54:51
 * @Program: crtsurfdata4.cpp
 */

#include "_public.h"

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

// transfer climate data from vector into file
bool CrtSurfFile(const char *outpath, const char *datafmt);

CLogFile logfile(10);

int main(int argc, char *argv[]){
 //inifile outpath logfile
 if(argc != 5){
  printf("Using: sudo ./crtsurfdata4 inifile outpath logfile datafmt\n");
  printf("Example: sudo ./crtsurfdata4 /home/yizhen/Desktop/project/idc/ini/stcode.ini /tmp/idc/surfdata /log/idc/crtsurfdata4.log xml, json, csv\n\n");
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

if (LoadSTCode(argv[1]) == false)
        return -1;
CrtSurfData();

if(strstr(argv[4], "xml") != 0) CrtSurfFile(argv[2], "xml");

if(strstr(argv[4], "json") != 0) CrtSurfFile(argv[2], "json");
if(strstr(argv[4], "csv") != 0) CrtSurfFile(argv[2], "csv");

 logfile.Write("crtsurfdata1 stopped ...\n");


 return 0;
}

bool LoadSTCode(const char *inifile)
{
        CFile File;

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

        }
        // close file
        File.CloseAndRename();

        // write success log
        logfile.Write("Successfully generated data file %s, data time %s, record data %d. \n", strFileName, strddatetime, vsurfdata.size());

        return true;
}