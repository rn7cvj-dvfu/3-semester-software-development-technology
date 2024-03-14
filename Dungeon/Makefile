CFLAGS+= -std=gnu++11 -Wall -pedantic -O3 -ggdb -Iinclude -I c:/MinGW/include/ncurses
OBJECTS= spell.o main.o level.o game.o character.o enemy.o geometry.o tile.o \
item.o gui.o scrollable_menu.o treasure.o equippable.o \
enemy_type.o projectile.o projectile_modifier.o healing_potion.o scroll_fire.o \
spell_factory.o bomb.o spawner.o mutable_block.o
LIBS= -lncurses
CXX ?= g++

main: ${OBJECTS}
	${CXX} ${CFLAGS} -o $@ ${OBJECTS} ${LIBS}

#Objects with a header file
%.o: src/%.cpp include/*.hpp
	${CXX} ${CFLAGS} -c -o $@ $<

clean:
	-rm -f $(OBJECTS) main

install: main
	cp -v main /usr/bin/

.PHONY: clean
