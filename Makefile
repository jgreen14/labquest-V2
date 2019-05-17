all: labquest

labquest: labquest.o io.o player.o creature.o agent.o level.o room.o tiles.o items.o coord.o list.o node.o
	g++ -enable-auto-import  -g labquest.o io.o creature.o player.o agent.o level.o room.o tiles.o items.o coord.o list.o node.o -o labquest -lncurses 

node.o: node.cpp
	g++ -g -c node.cpp

list.o: list.cpp
	g++ -g -c list.cpp

coord.o: coord.cpp
	g++ -g -c coord.cpp

items.o: items.cpp
	g++ -g -c items.cpp

tiles.o: tiles.cpp
	g++ -g -c tiles.cpp

room.o: room.cpp
	g++ -g -c room.cpp

level.o: level.cpp
	g++ -g -c level.cpp

agent.o: agent.cpp
	g++ -g -c agent.cpp

player.o: player.cpp
	g++ -g -c player.cpp

creature.o: creature.cpp
	g++ -g -c creature.cpp

io.o: io.cpp
	g++ -g -c io.cpp

labquest.o: labquest.cpp
	g++ -g -c labquest.cpp

clean:
	-rm -f labquest labquest.exe node.o list.o labquest.o items.o coord.o tiles.o room.o level.o agent.o player.o creature.o io.o
