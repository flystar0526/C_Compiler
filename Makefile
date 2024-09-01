CFLAGS=-std=c11 -g -static
OBJ = main.o tokenizer.o parser.o

# 編譯可執行文件 main，使用到 main.o 和 parser.o
main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

# 編譯 main.o
main.o: main.c tokenizer.h
	$(CC) $(CFLAGS) -c main.c

# 編譯 parser.o
tokenizer.o: tokenizer.c tokenizer.h
	$(CC) $(CFLAGS) -c tokenizer.c

# 編譯 parser.o
parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

# 測試命令，依賴於 main
test: main
	./test.sh

# 清理生成的文件
clean: 
	rm -f main *.o *~ tmp*

.PHONY: test clean
