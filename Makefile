#
# Quick-n-dirty all-purpose makefile
#

CC          = g++
CFLAGS      = -g -Wall -O2
LDFLAGS     =
COMPILE     = $(CC) $(CFLAGS) -c
LD          = $(CC) 
 
EXECUTABLE  = MyTronBot
SOURCES     = MyTronBot.cc map.cpp astar.cpp loc.cpp astarnode.cpp longestpath.cpp bot.cpp
HEADERS     = map.h astar.h loc.h astarnode.h longestpath.h bot.h ai.h
OBJECTS     = $(SOURCES:.cpp=.o) 

all : $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS) $(HEADERS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@ 

map.cpp.o : map.cpp map.h ai.h
	$(COMPILE) $< -o $@

astar.cpp.o : astar.cpp astar.h ai.h
	$(COMPILE) $< -o $@
	
astarnode.cpp.o : astarnode.cpp astarnode.h ai.h
	$(COMPILE) $< -o $@
	
floodfill.cpp.o : floodfill.cpp floodfill.h ai.h
	$(COMPILE) $< -o $@
	
bot.cpp.o : bot.cpp bot.h ai.h
	$(COMPILE) $< -o $@
	
.cpp.o : 
	$(COMPILE) $< -o $@

clean :
	rm *.o $(EXECUTABLE)

# end of makefile
