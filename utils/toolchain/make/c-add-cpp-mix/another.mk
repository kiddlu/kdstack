#  
#  
  
BIN = foyerserver.exe  
CC = gcc  
CPP = g++  

INCS =   
LIBS =   
SUBDIRS =  

DEFINC = -I"./../../base/" -I"./../common" -I"./../../lib/lxnet/" -I"./../../lib/tinyxml/src/"  

CSRCS = $(wildcard  ./*.c ./../../base/log.c ./../../base/corsslib.c ./../../base/idmgr.c ./../../base/pool.c)  
CPPSRCS = $(wildcard ./*.cpp ./../common/backcommand.cpp ./../common/connector.cpp)  

COBJS := $(CSRCS:.c=.o)  
CPPOBJS := $(CPPSRCS:.cpp=.o)  

MAKEDEPEND = gcc -MM -MT  
CFLAGS =  
#CFLAGS += -Wall -ansi -DWIN32 -DNDEBUG -O2  
CPPFLAGS =  
#CPPFLAGS += -Wall -DWIN32 -DNDEBUG -O2  

CDEF = $(CSRCS:.c=.d)  
CPPDEF = $(CPPSRCS:.cpp=.d)  

PLATS = win32-debug win32-release linux-debug linux-release  
none:  
    @echo "Please choose a platform:"  
    @echo " $(PLATS)"  
win32-debug:  
    $(MAKE) all INCS=-I"c:/mingw/include" LIBS="-L"c:/mingw/lib" -L"./../../lib/lxnet" -llxnet -lws2_32 -L"./../../lib/tinyxml" -ltinyxml" CFLAGS="-Wall -DWIN32 -DDEBUG -g" CPPFLAGS="-Wall -DWIN32 -DDEBUG -g"  
win32-release:  
    $(MAKE) all INCS=-I"c:/mingw/include" LIBS="-L"c:/mingw/lib" -L"./../../lib/lxnet" -llxnet -lws2_32 -L"./../../lib/tinyxml" -ltinyxml" CFLAGS="-Wall -DWIN32 -DNDEBUG -O2" CPPFLAGS="-Wall -DWIN32 -DNDEBUG -O2"  
linux-debug:  
    $(MAKE) all INCS=-I"/usr/include" LIBS="-L"/usr/lib" -L"./../../lib/lxnet" -llxnet -lpthread -L"./../../lib/tinyxml" -ltinyxml" CFLAGS="-Wall -DDEBUG -g" CPPFLAGS="-Wall -DDEBUG -g"  
linux-release:  
    $(MAKE) all INCS=-I"/usr/include" LIBS="-L"/usr/lib" -L"./../../lib/lxnet" -llxnet -lpthread -L"./../../lib/tinyxml" -ltinyxml" CFLAGS="-Wall -DNDEBUG -O2" CPPFLAGS="-Wall -DNDEBUG -O2"  

all:$(BIN)  

$(CDEF) : %.d : %.c  
    $(MAKEDEPEND) $(<:.c=.o) $< $(DEFINC) > $@  
$(CPPDEF) : %.d : %.cpp  
    $(MAKEDEPEND) $(<:.cpp=.o) $< $(DEFINC) > $@  

depend:
    -rm $(CDEF)  
    -rm $(CPPDEF)  
    $(MAKE) $(CDEF)  
    $(MAKE) $(CPPDEF)  
$(COBJS) : %.o: %.c  
    $(CC) -c $< -o $@ $(INCS) $(DEFINC) $(CFLAGS)  
$(CPPOBJS) : %.o: %.cpp  
    $(CPP) -c $< -o $@ $(INCS) $(DEFINC) $(CPPFLAGS)  
$(BIN) : $(COBJS) $(CPPOBJS)  
    $(CPP) -o $(BIN) $(COBJS) $(CPPOBJS) $(LIBS)  
    -rm $(COBJS) $(CPPOBJS)  
  
.PHONY:clean 
clean:            
    -rm $(BIN) $(COBJS) $(CPPOBJS)  
