FLAGS = -Iinclude -Ithird-party

default: build

build: main.o hashmap.o json_value.o json_value_parse.o json_object.o json_object_parse.o json_array.o json_array_parse.o json_parse.o
	gcc -o out main.o hashmap.o json_value.o json_value_parse.o json_object.o json_object_parse.o json_array.o json_array_parse.o json_parse.o -Wall 

main.o: main.c third-party/hashmap.h
	gcc -c main.c $(FLAGS)

hashmap.o: third-party/hashmap.c third-party/hashmap.h
	gcc -c third-party/hashmap.c $(FLAGS)

json_value.o: json_value.c include/json_value.h
	gcc -c json_value.c $(FLAGS)

json_value_parse.o: json_value_parse.c include/json_value_parse.h
	gcc -c json_value_parse.c $(FLAGS)

json_object.o: json_object.c include/json_object.h third-party/hashmap.h
	gcc -c json_object.c $(FLAGS)

json_object_parse.o: json_object_parse.c include/json_object_parse.h
	gcc -c json_object_parse.c $(FLAGS)

json_array.o: json_array.c include/json_array.h
	gcc -c json_array.c $(FLAGS)

json_array_parse.o: json_array_parse.c include/json_array_parse.h
	gcc -c json_array_parse.c $(FLAGS)

json_parse.o: json_parse.c include/json_parse.h
	gcc -c json_parse.c $(FLAGS)

clear:
	del *.o
	del out.exe