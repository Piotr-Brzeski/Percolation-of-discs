all: bin/discs

OBJS=tmp/bins.o tmp/disc.o tmp/main.o tmp/position.o tmp/random.o tmp/system.o

OPTIONS=-flto -O3

bin/discs: tmp bin $(OBJS)
	g++ $(OPTIONS) -o bin/discs $(OBJS)

tmp:
	mkdir -p tmp

bin:
	mkdir -p bin

tmp/%.o: discs/%.cpp
	g++ -std=c++17 $(OPTIONS) -c $< -o $@

clean:
	rm -rf tmp bin

