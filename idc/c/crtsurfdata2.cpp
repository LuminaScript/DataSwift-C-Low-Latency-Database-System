/*
 * @Author: YizhenZhang yz4401@columbia.edu
 * @Date: 2023-08-16 01:54:51
 * @Program: crtsurfdata2.cpp
 */

#include "_public.h"
/*
省   站号  站名 纬度   经度  海拔高度
安徽,58015,砀山,34.27,116.2,44.2
*/
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



CLogFile logfile(10);

int main(int argc, char *argv[]){
 //inifile outpath logfile
 if(argc != 4){
  printf("Using: sudo ./crtsurfdata1 inifile outpath logfile\n");
  printf("Example: sudo ./crtsurfdata2 /home/yizhen/Desktop/project/idc/ini/stcode.ini /tmp/surfdata /log/idc/crtsurfdata2.log\n\n");
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
        /*
        for(int i = 0; i < vstcode.size(); i++)
                logfile.Write("provname=%s,obtid=%s,obtname=%s,lat=%.2f,lon=%.2f,height=%.2f\n", vstcode[i].provname,vstcode[i].obtid, vstcode[i].obtname, vstcode[i].lat, vstcode[i].lon, vstcode[i].height);

        */
        return true;
}