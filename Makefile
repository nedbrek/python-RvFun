OBJ := pymain.o
LIB := rvfun.so
CXXFLAGS += -std=c++11 -MP -MMD -Wall -g -O3 -fPIC
CXXFLAGS += $(shell pkg-config --cflags python3)
CXXFLAGS += -I../RvFun
LIBS := -lboost_python3 $(shell pkg-config --libs python3)

DEP  := $(addprefix dep/,$(OBJ:.o=.d))
OBJS := $(addprefix obj/,$(OBJ))

CXXBUILD = $(CXX) $(CXXFLAGS) -MF $(patsubst %.cpp,dep/%.d,$<) -c -o $@ $<

.PHONY: all
all: dep obj $(LIB)

-include $(DEP)

$(OBJS): obj/%.o: %.cpp
	@$(CXXBUILD)

obj:
	@mkdir $@

dep:
	@mkdir $@

$(LIB): $(OBJS) ../RvFun/rvfun.a
	g++ -shared -g -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm -f $(OBJS) $(LIB)

