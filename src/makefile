#C++向けで、以下の前提で使用可能なmakefile
#
#・同一フォルダにヘッダ・ソースと実行ファイルが格納されている
#・実行ファイルはフォルダ名と同一
#・main関数を持つソースはフォルダ名.cpp
#・ソースの拡張子はcpp

#変数定義
###################################################

#ターゲット
#作成する実行ファイル
#2個上のフォルダ名と同一
TARGET := $(shell basename `dirname \`pwd\``)

#g++のコンパイルのフラグ
#CXXFLAGS := -Wall -lsqlite3 -L/usr/lib/x86_64-linux-gnu -lglut -lGLU -lGL -I/usr/include
#デバッグ用のオプション
CXXFLAGS := -g -O0 -Wall -lsqlite3 -L/usr/lib/x86_64-linux-gnu -lglut -lGLU -lGL -I/usr/include

#ソースファイル
#フォルダ直下の.cppファイル
SRCS := $(shell ls *.cpp)

#オブジェクトファイル
#.cppを.oに置換している
OBJS := $(SRCS:.cpp=.o)

#依存関係ファイル
DEPS := $(SRCS:.cpp=.d)

#コマンド
###################################################

#全てコンパイルする
#先頭に書いたコマンドはmakeコマンドでオプション無しで実行される
.PHONY: all
all: $(TARGET)

.PHONY: run
run: $(TARGET)
	./$(TARGET)

#実行ファイルの作成
#オプションは最後に指定する
#先頭に書くとコンパイルが通らない
#<ターゲット>: ターゲットに依存するファイル
#$@ : ターゲット名
#$< : 最初の依存ファイル名(":"の右側の最初(一番左)の項目)
#$^ : 依存ファイル全体
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

#オブジェクトファイルの作成
#"-MMD"オプションによって依存関係ファイル(*.d)も同時に作成する
%.o: %.cpp
	$(CXX) -c -MMD $< $(CXXFLAGS)

#実行ファイル・中間ファイルの削除
#".PHONY"はターゲットではなくコマンドであることを明示するためのコマンド
.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

#依存関係ファイルをインクルードする
#先頭に”-”を書いているので依存関係ファイルがなくてもエラーとならない
-include $(DEPS)
