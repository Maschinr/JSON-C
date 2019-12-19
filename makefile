FLAGS = -Iinclude -Ithird-party

default: test build

build: main.o hashmap.o json.o
	gcc -o out main.o hashmap.o json.o -Wall 

main.o: main.c third-party/hashmap.h
	gcc -c main.c $(FLAGS)

hashmap.o: third-party/hashmap.c third-party/hashmap.h
	gcc -c third-party/hashmap.c $(FLAGS)

json.o: json.c include/json.h third-party/hashmap.h
	gcc -c json.c $(FLAGS)

# Test Code
TestJson.o: test/TestJson.c 
	gcc -c test/TestJson.c $(FLAGS)

unity.o: third-party/unity.c third-party/unity.h
	gcc -c third-party/unity.c $(FLAGS)

test: TestJson.o json.o unity.o hashmap.o
	gcc -o json_test json.o TestJson.o unity.o hashmap.o -Wall 
	./json_test

clear:
	del *.o
	del out.exe
	del json_test.exe