#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

void jsonNameList(char *jsonstr, jsmntok_t *t, int tokcount, int *nameTokIndex)
{
	int i,j=1;
	for(i = 1; i < tokcount; i++)
	{
		//printf("Tok(%d) - Toke(%d): %d-%d\"%.*s\"\n",i,i-1,t[i-1].end-t[i].start,t[i+1].start-t[i].end,t[i].end-t[i].start, jsonstr + t[i].start);
		if(i-1,t[i-1].end-t[i].start==-6||t[i+1].start-t[i].end==4)
		{
			nameTokIndex[j-1] = i;
			j++;
		}
	}
}

void printNameList(char *jsonstr, jsmntok_t *t, int *nameTokIndex)
{
	printf("***** Name List *****\n");
	int i =1,j=0;
	while(nameTokIndex[j]!=0)
	{
		i=nameTokIndex[j];
		printf("NAME[%d]: %.*s\n",j+1,t[i].end-t[i].start, jsonstr + t[i].start);
		j++;
	}
}

void selectNameList(char *jsonstr, jsmntok_t *t, int *nameTokIndex)
{
	int input,i,j=0,z;
	int maxindex;
	while(nameTokIndex[j] !=0)
	{
		j++;
	}
	maxindex = j-1;
	int maxNameToken = nameTokIndex[maxindex];
	do{
		printf("Select Name's no (exit:0) >> ");
		scanf("%d",&input);
		i=nameTokIndex[input-1];
		printf("NAME[%d]: %.*s\n",input,t[i].end-t[i].start, jsonstr + t[i].start);

		for(z=maxNameToken;z>nameTokIndex[input-1];z--)
			if(nameTokIndex[input]>z)
				printf("%.*s\n",t[z].end-t[z].start, jsonstr + t[z].start);

	}while(input!=0);
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
	int nameTokIndex[100]={0};
	jsonNameList(JSON_STRING,&t[0],r,&nameTokIndex[0]);

	printNameList(JSON_STRING,&t[0],&nameTokIndex[0]);
	selectNameList(JSON_STRING, &t[0], &nameTokIndex[0]);

	return EXIT_SUCCESS;
}
