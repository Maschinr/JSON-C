FLAGS = -Iinclude
TEST_FLAGS = -Iinclude -Ithird-party

default: test build

build: main.o json.o
	gcc -o out main.o json.o -Wall 

main.o: main.c include/json.h
	gcc -c main.c $(FLAGS)

json.o: json.c include/json.h
	gcc -c json.c $(FLAGS)

# Test Code
TestJson.o: test/TestJson.c 
	gcc -c test/TestJson.c $(TEST_FLAGS)

unity.o: third-party/unity.c third-party/unity.h
	gcc -c third-party/unity.c $(TEST_FLAGS)

test: TestJson.o json.o unity.o 
	gcc -o test json.o TestJson.o unity.o -Wall 
	./test

clear:
	del *.o
	del out.exe