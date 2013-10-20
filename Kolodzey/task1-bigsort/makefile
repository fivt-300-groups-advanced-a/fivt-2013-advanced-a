#тесты состоят из тестирующих функций и не содержат main
#эта штука автоматически добавляет все исходники, лежащие в папке tests
#и подключает к ним соответствующий файл из папки source

#для тестирования функций, вызывающих несколько блоков, перепишу шаблон…

# SYNOPSIS:
#
#   make [all]  - makes everything.
#   make TARGET - makes the given target. ex make tests/bubblesort_unittest

CXX = clang++

BIN_DIR   = bin
GTEST_DIR = gtest-1.7.0
TESTS_DIR = tests
BIGSORT_DIR = source

#названия тестов; цель all = построить тесты из всех исходников в tests/
TESTS = $(subst .cpp,,$(wildcard $(TESTS_DIR)/*.cpp))

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
#BIGSORT_DIR тут добавлена за тем же, что и GTEST_DIR
CPPFLAGS += -isystem $(GTEST_DIR)/include
CPPFLAGS += -isystem $(BIGSORT_DIR)


# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

all: $(TESTS)

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
$(BIN_DIR)/gtest-all.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc \
            -o $(BIN_DIR)/gtest-all.o

$(BIN_DIR)/gtest_main.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc \
            -o $(BIN_DIR)/gtest_main.o


$(BIN_DIR)/gtest.a: $(BIN_DIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(BIN_DIR)/gtest_main.a: $(BIN_DIR)/gtest-all.o $(BIN_DIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^


#сборка простой тестирующей функции, вызывающей только 1 блок кода
#вместо % подставьте название интересующего блока
$(BIN_DIR)/%.o: $(BIGSORT_DIR)/%.cpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/%_unittest.o: $(TESTS_DIR)/%_unittest.cpp \
	                     $(BIGSORT_DIR)/bigsort.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(TESTS_DIR)/%_unittest:  $(addprefix $(BIN_DIR)/,%.o %_unittest.o \
	                                              gtest_main.a)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

#сборка integration теста
$(BIN_DIR)/integration_test.o: $(TESTS_DIR)/integration_test.cpp \
	                     $(BIGSORT_DIR)/bigsort.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(TESTS_DIR)/integration_test: $(addprefix $(BIN_DIR)/,integration_test.o \
	                                              gtest_main.a)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@
