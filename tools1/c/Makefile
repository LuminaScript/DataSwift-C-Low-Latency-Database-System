# 开发框架头文件路径。
PUBINCL = -I/home/yizhen/Desktop/project/public

# 开发框架cpp文件名，这里直接包含进来，没有采用链接库，是为了方便调试。
PUBCPP = /home/yizhen/Desktop/project/public/_public.cpp

CFLAGS = -g

all: procctl checkproc book10

procctl:procctl.cpp
	g++ -o procctl procctl.cpp
	cp procctl ../bin/.

checkproc:checkproc.cpp
	g++ $(CFLAGS) -o checkproc checkproc.cpp $(PUBINCL) $(PUBCPP) -lm -lc

book10: book10.cpp
	g++ $(CFLAGS) -o book10 book10.cpp $(PUBINCL) $(PUBCPP) -lm -lc

clean:
	rm -f procctl checkproc book10