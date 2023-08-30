# 开发框架头文件路径。
PUBINCL = -I/home/yizhen/Desktop/project/public

# 开发框架cpp文件名，这里直接包含进来，没有采用链接库，是为了方便调试。
PUBCPP = /home/yizhen/Desktop/project/public/_public.cpp

CFLAGS = -g

all: procctl checkproc book10 gzipfiles deletefiles ftpclient tcpputfiles fileserver

procctl:procctl.cpp
	g++ -o procctl procctl.cpp
	cp procctl ../bin/.

checkproc:checkproc.cpp
	g++ $(CFLAGS) -o checkproc checkproc.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp checkproc ../bin/.

book10: book10.cpp
	g++ $(CFLAGS) -o book10 book10.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp book10 ../bin/.

gzipfiles:gzipfiles.cpp
	g++ $(CFLAGS) -o gzipfiles gzipfiles.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp gzipfiles ../bin/.

deletefiles: deletefiles.cpp
	g++ $(CFLAGS) -o deletefiles deletefiles.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp deletefiles ../bin/.

ftpclient: ftpclient.cpp
	g++ $(CFLAGS) -o ftpclient ftpclient.cpp /home/yizhen/Desktop/project/public/_ftp.cpp $(PUBCPP) $(PUBINCL) -lftp -lm -lc

tcpputfiles: tcpputfiles.cpp
	g++ $(CFLAGS) -o tcpputfiles tcpputfiles.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp tcpputfiles ../bin/.

fileserver: fileserver.cpp
	g++ $(CFLAGS) -o fileserver fileserver.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp fileserver ../bin/.
clean:
	rm -f procctl checkproc book10 gzipfiles deletefiles