#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

void jsonNameList(char *jsonstr, jsmntok_t *t, int tokcount)
{
	int i,j=1;
	printf("***** Name List *****\n");
	for(i = 1; i < tokcount; i++)
	{
		//printf("Tok(%d) - Toke(%d): %d-%d\"%.*s\"\n",i,i-1,t[i-1].end-t[i].start,t[i+1].start-t[i].end,t[i].end-t[i].start, jsonstr + t[i].start);
		if(i-1,t[i-1].end-t[i].start==-6||t[i+1].start-t[i].end==4)
		{
			printf("NAME[%d]: %.*s\n",j,t[i].end-t[i].start, jsonstr + t[i].start);
			j++;
		}
	}


}

char *readJSONFile()	{
	char *New_JSONSTRING;
	int count=0;
	New_JSONSTRING = (char*)malloc(sizeof(char)*1);
	strcpy(New_JSONSTRING,"");
	FILE *f;
	f = fopen("data.json","r");
	char *temp;
	temp = (char*)malloc(sizeof(char)*1);

	while(1){
	fgets(temp,sizeof(temp),f);
		if(feof(f))
		{
			break;
		}
		count+=strlen(temp)+1;
		New_JSONSTRING=realloc(New_JSONSTRING,sizeof(char)*count);
		strcat(New_JSONSTRING,temp);
	}
	fclose(f);
	return New_JSONSTRING;
}

/*static const char *JSON_STRING =
	"{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  "
	"\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";*/

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int main() {
	char *JSON_STRING = readJSONFile();

	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}
	jsonNameList(JSON_STRING,&t[0],r);

	return EXIT_SUCCESS;
}
