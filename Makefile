CC=g++ -std=c++11
CFLAGS= -fopenmp -O3

INC=-I./include
SRC=./src
OBJ=bin

SRCFILES=$(addprefix $(OBJ)/, $(subst .c,.o, $(subst .cpp,.o, $(subst $(SRC)/,,$(wildcard $(SRC)/*)))))

all:
	make clean
	mkdir $(OBJ)
	make proxy

proxy: $(SRCFILES)
	mkdir -p $(OBJ)
	$(CC) -o tsp $(SRCFILES) $(INC) $(CFLAGS)

$(OBJ)/%.o: $(SRC)/%.cpp
	mkdir -p $(OBJ)
	$(CC) $(DEBUG) -c $< -o $@ $(INC) $(CFLAGS)

clean:
	rm -rf $(OBJ)
	rm -f tsp
