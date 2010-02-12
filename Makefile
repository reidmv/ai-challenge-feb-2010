#
# Quick-n-dirty all-purpose makefile
#

CC          = g++
CFLAGS      = -g -Wall
LDFLAGS     =
COMPILE     = $(CC) $(CFLAGS) -c
LD          = $(CC) 
 
EXECUTABLE  = MyTronBot
SOURCES     = MyTronBot.cc map.cpp astar.cpp loc.cpp astarnode.cpp floodfill.cpp bot.cpp
OBJECTS     = $(SOURCES:.cpp=.o) 

all : $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@ 

.cpp.o :
	$(COMPILE) $< -o $@

clean :
	rm *.o $(EXECUTABLE)

# end of makefile
