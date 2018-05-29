#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

/*my practice*/
 void printtoken(const char *JSON_STRING, jsmntok_t *t, int count){
    int i=0;
    int z;
    for(i=0; i<count; i++){
    printf("index: [%2d] start&end{%d, %d} size <%d> type '%d' parent <%d>\n%.*s\n", i, t[i].start, t[i].end, t[i].size, t[i].type, t[i].parent,t[i].end-t[i].start,
          JSON_STRING + t[i].start);
       }
 }

void firstDataOfObjectAndSelectObject(const char *JSON_STRING, jsmntok_t *t, int count)
{
	int i;
	int j =0, y=0;
  int z;
  int *objectStartList;
  objectStartList = (int*) malloc(sizeof(int));
  int *NameList;
  NameList = (int*) malloc(sizeof(int));
  printf("****** Name List ******\n");
	for(i = 0;i<count;i++)
	{
    if(t[i].parent == -1 && t[i].type == 1)
    {
        j++;
        objectStartList = (int*) realloc(objectStartList,sizeof(int)*j);
        objectStartList[j-1] = i+1;
    }
		if(t[i].parent == 0)
		{
			// while(1)
			// {
			// 	i++;
			// 	if((t+i)->type == JSMN_STRING && (t+i)-> size == 1)
			// 		break;
			// }
      y++;
      NameList = (int*) realloc(NameList,sizeof(int)*j);
      NameList[j-1] = i+1;
			printf("[NAME %d] %.*s\n",y,t[i].end-t[i].start,
	          JSON_STRING + t[i].start);
		}
	}
  //printf("@@@@@@@@ %d @@@@@@@@",j);
  printf("\n\n\n****** Object List ******\n");
  for(i=0;i<j;i++)
  {
    printf("[Name %d] %.*s\n",i+1,t[i+2].end-t[i+2].start,
          JSON_STRING + t[i+2].start);
  }

  int selectedNum;
  while(1)
    {
      printf("원하는 번호 입력(Exit:0) :");
      scanf("%d",&selectedNum);
      if(selectedNum==0)
        break;
      else if(selectedNum<0||selectedNum>j)
        printf("그 번호의 object는 존재하지 않습니다.\n");
      else if(selectedNum>0&&selectedNum<j)
      {
        printf("name : %.*s\n",t[objectStartList[selectedNum-1]+1].end-t[objectStartList[selectedNum-1]+1].start, JSON_STRING + t[objectStartList[selectedNum-1]+1].start);
        for(z=objectStartList[selectedNum-1]+2;z<objectStartList[selectedNum]-1;z++)
        {
          if(t[z].type==3||t[z].type==2)
          {
            if(t[z].size>=1 &&t[z].parent == 0)
            {
              printf("\t[%.*s]    ",t[z].end-t[z].start, JSON_STRING + t[z].start);
              printf("%.*s\n",t[z+1].end-t[z+1].start, JSON_STRING + t[z+1].start);
              z++;
            }
          }
        }
      }
      else if(selectedNum == j)
      {
        printf("name : %.*s\n",t[objectStartList[selectedNum-1]+1].end-t[objectStartList[selectedNum-1]+1].start, JSON_STRING + t[objectStartList[selectedNum-1]+1].start);
        for(z=objectStartList[selectedNum-1]+2;z<count;z++)
        {
          if(t[z].type==3||t[z].type==2)
          {
            if(t[z].size>=1 &&t[z].parent == 0)
            {
              printf("\t[%.*s]    ",t[z].end-t[z].start, JSON_STRING + t[z].start);
              printf("%.*s\n",t[z+1].end-t[z+1].start, JSON_STRING + t[z+1].start);
              z++;
            }
          }
        }
      }
  }
}

void jsonNameList(char *jsonstr, jsmntok_t *t, int tokcount, int *nameTokIndex)
{
	printf("***** Object List *****\n");
	int i,j=1;
	for(i = 0; i < tokcount; i++)
	{
		if((t+i)->type == JSMN_STRING && (t+i)-> size == 1)
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
		printf("NAME[%d]: %.*s",j+1,t[i].end-t[i].start, jsonstr + t[i].start);
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
	f = fopen("data2.json","r");
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
	// if (r < 1 || t[0].type != JSMN_OBJECT) {
	// 	printf("Object expected\n");
	// 	return 1;
	// }
	//int nameTokIndex[100]={0};
	//jsonNameList(JSON_STRING,&t[0],r,&nameTokIndex[0]);

	//printNameList(JSON_STRING,&t[0],&nameTokIndex[0]);
	//selectNameList(JSON_STRING, &t[0], &nameTokIndex[0]);

	//printtoken(JSON_STRING,&t[0], r);

	firstDataOfObjectAndSelectObject(JSON_STRING,&t[0], r);
	return EXIT_SUCCESS;
}
