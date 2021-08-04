SRCS := $(wildcard *.cpp) 
OBJS := $(foreach wrd,$(SRCS),$(patsubst %.cpp,%.o,$(wrd)))
LIBS := libs/
EXEC =
EXEC := $(if $(EXEC),$(EXEC),main)

all: $(EXEC) clean 
	./main && convert ray.ppm ray.jpg 
$(EXEC): $(OBJS)
	$(info Compiling..)
	g++ -o $(EXEC) $(OBJS)

%.o: %.cpp
	$(info Linking..)	
	g++ -c $< -I$(LIBS) -o $@ -O2

.PHONY: clean
clean:
	rm -f *.o
