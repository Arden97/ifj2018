#include <stdlib.h>
#include <stdbool.h>
#include "strlib.h"

string_t * strInit(size_t size){
	string_t *str = (string_t *)malloc(sizeof(string_t));
	if(!str) exit(99);

  str->string = (char *)malloc(sizeof(char)*size + 1);
	if(!(str->string)) exit(99);

	str->capacity = size;
	str->length = 0;
	return str;
}

bool extendStr(string_t *str, size_t new_size){
    if( !(str->string =(char *)(realloc(str->string, sizeof(char)*new_size + 1, str->capacity + 1))) )
		exit(99);
    str->capacity=new_size;
    return true;
}
