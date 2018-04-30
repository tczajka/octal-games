GTEST_DIR := googletest/googletest
CPPFLAGS := -MMD -MP -I$(GTEST_DIR)/include
CXXFLAGS := -O2 -std=c++11 -Wall -Wextra -pthread
LDFLAGS := -lpthread
SOURCES := $(shell find src -name '*.cc')
OBJECTS := $(SOURCES:src/%.cc=obj/%.o)
DEPENDENCIES := $(OBJECTS:%.o=%.d)
TEST_OBJECTS := $(filter %_test.o, $(OBJECTS))
EXEC_OBJECTS := $(filter %_exec.o, $(OBJECTS))
REGULAR_OBJECTS := $(filter-out %_test.o %_exec.o, $(OBJECTS))
BINARIES := $(EXEC_OBJECTS:obj/%_exec.o=bin/%)

.PHONY: all
all: bin/tests $(BINARIES)

.PHONY: clean
clean:
	rm -fr bin obj

superclean:
	rm -fr bin obj obj-gtest

bin:
	mkdir -p bin

obj/%.o: src/%.cc
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ -Isrc

bin/tests: $(TEST_OBJECTS) $(REGULAR_OBJECTS) obj-gtest/gtest_main.a | bin
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

bin/%: obj/%_exec.o $(REGULAR_OBJECTS) | bin
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Googletest

GTEST_HEADERS := $(GTEST_DIR)/include/gtest/*.h \
                 $(GTEST_DIR)/include/gtest/internal/*.h

GTEST_SRCS_ := $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

obj-gtest:
	mkdir -p obj-gtest

obj-gtest/gtest-all.o : $(GTEST_SRCS_) | obj-gtest
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@

obj-gtest/gtest_main.o : $(GTEST_SRCS_) | obj-gtest
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@

obj-gtest/gtest.a : obj-gtest/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

obj-gtest/gtest_main.a : obj-gtest/gtest-all.o obj-gtest/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Dependencies

-include $(DEPENDENCIES)
