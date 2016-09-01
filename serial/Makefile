# Makefile_header

# プログラム名とオブジェクトファイル名
PROGRAM = serial_test
OBJS    = serial_lib.o $(PROGRAM:%=%.o) 

# 定義済みマクロの再定義
CC = gcc
CXX = g++
CFLAGS = -Wall -O2 
# CFLAGS_DEBUG = -Wall -g

# サフィックスルール適用対象の拡張子の定義
.SUFFIXES: .cpp .o
# サフィックスルール
debug: CFLAGS += -g
debug: $(PROGRAM)

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

# プライマリターゲット
.PHONY: all
all: $(PROGRAM)

# プログラムの生成ルール

$(PROGRAM): $(OBJS)
	$(CXX) -o $(PROGRAM) $^


# ファイル削除用ターゲット
.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJS) depend.inc

# ヘッダファイルの依存関係
.PHONY: depend
depend: $(OBJS:.o=.cpp)
	-@ $(RM) depend.inc
	-@ for i in $^; do cpp -MM $$i | sed "s/\ [_a-zA-Z0-9][_a-zA-Z0-9]*\.cpp//g" >> depend.inc; done

run:
	./$(PROGRAM)
-include depend.inc
