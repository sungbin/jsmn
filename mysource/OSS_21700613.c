#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

/*my practice*/
/**
 void printtoken(const char *JSON_STRING, jsmntok_t *t, int count){
    int i=0;
    int z;
    for(i=0; i<count; i++){
    printf("index: [%2d] start&end{%d, %d} size <%d> type '%d' parent <%d>\n%.*s\n", i, t[i].start, t[i].end, t[i].size, t[i].type, t[i].parent,t[i].end-t[i].start,
          JSON_STRING + t[i].start);
       }
 }
 **/

 typedef struct NameTokenInfo *pNameTokenInfo;
 typedef struct NameTokenInfo {
   int tokindex;
   int objectindex;
 } NameTokenInfo;

typedef struct {
  char model[20]; // 모델명
  char maker[20]; // 제조사
  int year; //제조년도
  char gastype[20]; //연료 타입
} car_t;

int makecarlist(char *JSON_STRING, jsmntok_t *t, int tokcount,  car_t *  p[])
{
	int i;
	int j =0;
	int k =0, y=0;
	int *objectStartList;
	int objectStartParent =0;
	objectStartList = (int*) malloc(sizeof(int));
	int *ObjectSubject;
	ObjectSubject = (int*) malloc(sizeof(int));
	pNameTokenInfo nt;
	nt = (pNameTokenInfo)malloc(sizeof(NameTokenInfo));
	for(i = 0;i<tokcount;i++)
	{
		if(t[i].parent == 0&& t[i].type ==3&&t[i].size==1)
		{
			k++;
			ObjectSubject = (int*) realloc(ObjectSubject,sizeof(int)*k);
			ObjectSubject[k-1] = i;
			objectStartParent = i+1;
			continue;
		}
		if(t[i].parent == objectStartParent && t[i].type == 1)
		{
				j++;
				objectStartList = (int*) realloc(objectStartList,sizeof(int)*j);
				objectStartList[j-1] = i;
		}
		if(t[i].parent == (objectStartList[j-1]))
		{
			y++;
			nt = (pNameTokenInfo)realloc(nt,sizeof(NameTokenInfo)*y);
			(nt[y-1]).tokindex = i+1;
			i+=7; //한 오프젝트의 인덱스가 8개여서, 7를 더해준다.
		}
	}
	for(i=0;i<j;i++)
	{
		car_t* ttemp;
		ttemp = (car_t*)malloc(sizeof(car_t));
		nt[i].objectindex = objectStartList[i];
		int tok = nt[i].tokindex;
		int PaObject = nt[i].objectindex;

		strncpy(ttemp->model,t[tok].start+ JSON_STRING ,t[tok].end-t[tok].start); //모델명

		strncpy(ttemp->maker,t[tok+2].start+ JSON_STRING ,t[tok+2].end-t[tok+2].start); //제조사
    strncpy(ttemp->gastype,t[tok+6].start+ JSON_STRING ,t[tok+6].end-t[tok+6].start); //연료타입

		char* year = JSON_STRING + t[tok+4].start; //제조년도
		int pp;
		sscanf(year,"%d",&pp);
		ttemp->year = pp;

		p[i] = ttemp;
	}
	return j;
}

void printcarlist(car_t *  p[], int pcount)
{
	printf("\n\n\n****************************************************************************************\n");
	printf("번호\t모델명\t\t\t제조사\t\t제조년도\t\t연료타입\n");
	printf("****************************************************************************************\n");

	int i;
	for(i=0;i<pcount;i++)
	{
		printf("%d\t",i+1);

		printf("%-20s\t",p[i]->model); //모델명
		printf("%-13s\t",p[i]->maker); //제조사
		printf("%-11d\t",p[i]->year); //년도
		printf("%-11s\n",p[i]->gastype); //연료타입
	}
}

char *readJSONFile()	{
	char *New_JSONSTRING;
	int count=0;
  char filename[15] = "cars.json";
	New_JSONSTRING = (char*)malloc(sizeof(char)*1);
	FILE *f;
	f = fopen(filename,"r");
  if((f = fopen(filename, "r")) == NULL) // 파일 열기
  {
   printf("%s 파일이 존재하지 않습니다.\n",filename);
   return NULL;
  }
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


  car_t* carlist[5];
  int car_count = makecarlist(JSON_STRING, &t[0],r,carlist);

  printcarlist(carlist,car_count);

	return EXIT_SUCCESS;
}
