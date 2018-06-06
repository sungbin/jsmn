#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"
#include "../myproduct.h"

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}


/** JSMN Example15(3) **/
int makeProduct(char *JSON_STRING, jsmntok_t *t, int tokcount,  product_t *  p[])
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
		if(t[i].parent == objectStartParent && t[i].type == 1) //parent 수정필요.
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
			i+=11;
		}
	}
	for(i=0;i<j;i++)
	{
		product_t* ttemp;
		ttemp = (product_t*)malloc(sizeof(product_t));
		nt[i].objectindex = objectStartList[i];
		int tok = nt[i].tokindex;
		int PaObject = nt[i].objectindex;

		char temp[20]="";

		strncpy(temp,t[tok].start+ JSON_STRING ,t[tok].end-t[tok].start); //제조사
		if(strcmp(temp,"apple")==0)
			ttemp->company = 0;
		else if(strcmp(temp,"samsung")==0)
			ttemp->company = 1;

		strncpy(ttemp->name,t[tok+2].start+ JSON_STRING ,t[tok+2].end-t[tok+2].start); //제품명
		strncpy(ttemp->shopping_mall,t[tok+8].start+ JSON_STRING ,t[tok+8].end-t[tok+8].start); //구매처
		strncpy(ttemp->status,t[tok+10].start+ JSON_STRING ,t[tok+10].end-t[tok+10].start); //상태

		char* price = JSON_STRING + t[tok+4].start;
		char* numOfProduct = JSON_STRING + t[tok+6].start;
		int pp;
		int nn;
		sscanf(price,"%d",&pp);
		sscanf(numOfProduct,"%d",&nn);
		ttemp->price = pp;
		ttemp->count = nn;

		p[i] = ttemp;

	}
	return j;
}

void printProduct(product_t *  p[], int pcount)
{
	printf("\n\n\n********************************************\n");
	printf("번호\t제품명\t\t\t제조사\t\t가격\t\t개수\t\t구매처\t\t상태\t\t총가격\n");
	printf("********************************************\n");

	int i;
	for(i=0;i<pcount;i++)
	{
		printf("%d\t",i+1);
		printf("%-20s\t",p[i]->name); //제품명

		if(p[i]->company==0)
			printf("%-10s\t","APPLE");
		else if(p[i]->company==1)
				printf("%-10s\t","SAMSUNG");

		printf("%-13d\t",p[i]->price); //가격
		printf("%-11d\t",p[i]->count); //개수
		printf("%-11s\t",p[i]->shopping_mall); //구매처
		printf("%-13s\t",p[i]->status); //상태

		printf("%d\n",p[i]->price*p[i]->count); // 총가격
	}
}

/** JSMN Example13(2): 5번 **/
int pullTokindex(const char *JSON_STRING, jsmntok_t *t, int tokcount, pNameTokenInfo nameTokIndex,
	int objectNo, char* name, int nameTokenCount)
{
	int i;
	for (i =0; i<nameTokenCount;i++)
	{
		if(nameTokIndex[i].objectindex == objectNo)
		{
			if(jsoneq(JSON_STRING, &t[nameTokIndex[i].tokindex-1], name) == 0)
				return nameTokIndex[i].tokindex;
		}
	}
	return -1;
}

int jsonNameList(const char *JSON_STRING, jsmntok_t *t, int tokcount, pNameTokenInfo nameTokIndex)
{
		int i;
		int j =0;
	  int k =0, y=0;
	  int *objectStartList;
	  int objectStartParent =0;
	  objectStartList = (int*) malloc(sizeof(int));
	  int *ObjectSubject;
	  ObjectSubject = (int*) malloc(sizeof(int));


	  nameTokIndex = (pNameTokenInfo)malloc(sizeof(NameTokenInfo));

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
	    if(t[i].parent == objectStartParent && t[i].type == 1) //parent 수정필요.
	    {
	        j++;
	        objectStartList = (int*) realloc(objectStartList,sizeof(int)*j);
	        objectStartList[j-1] = i;
	    }
	    if(t[i].parent == (objectStartList[j-1]))
	    {
	      y++;
				if(y!=1)
	      	nameTokIndex = (pNameTokenInfo)realloc(nameTokIndex,sizeof(NameTokenInfo)*y);
				nameTokIndex[y-1].tokindex = i+1;
				nameTokIndex[y-1].objectindex =objectStartList[j-1];
				printf("index = %d ",y-1);
				printf("p:%d ,%d** %.*s **\n",nameTokIndex[y-1].objectindex,nameTokIndex[y-1].tokindex,
				t[nameTokIndex[y-1].tokindex].end-t[nameTokIndex[y-1].tokindex].start,
		          JSON_STRING + t[nameTokIndex[y-1].tokindex].start);
	    }
	  }
		return y;


}

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

 // void PrintListOfSpecificProduct(const char *JSON_STRING, jsmntok_t *t, int count)
 // {
 //   int i;
 // 	 int j =0;
 //   int k =0, y=0;
 //   int *objectStartList;
 //   int objectStartParent =0;
 //   objectStartList = (int*) malloc(sizeof(int));
 //   int *ObjectSubject;
 //   ObjectSubject = (int*) malloc(sizeof(int));
 //   pNameTokenInfo nt;
 //   nt = (pNameTokenInfo)malloc(sizeof(NameTokenInfo));
 // 	for(i = 0;i<count;i++)
 // 	{
 //     if(t[i].parent == 0&& t[i].type ==3&&t[i].size==1)
 //     {
 //       k++;
 //       ObjectSubject = (int*) realloc(ObjectSubject,sizeof(int)*k);
 //       ObjectSubject[k-1] = i;
 //       objectStartParent = i+1;
 //       continue;
 //     }
 //     if(t[i].parent == objectStartParent && t[i].type == 1) //parent 수정필요.
 //     {
 //         j++;
 //         objectStartList = (int*) realloc(objectStartList,sizeof(int)*j);
 //         objectStartList[j-1] = i;
 //     }
 //     if(t[i].parent == (objectStartList[j-1]))
 //     {
 //       y++;
 //       nt = (pNameTokenInfo)realloc(nt,sizeof(NameTokenInfo)*y);
 // 			(nt[y-1]).tokindex = i+1;
 // 			i+=7;
 //     }
 //   }
 // }

// void PrintListOfSpecificProduct(char *JSON_STRING, jsmntok_t *t, int count)
// {
//   int i;
// 	int j =0;
//   int k =0, y=0;
//   int *objectStartList;
//   int objectStartParent =0;
//   objectStartList = (int*) malloc(sizeof(int));
//   int *ObjectSubject;
//   ObjectSubject = (int*) malloc(sizeof(int));
//   pNameTokenInfo nt;
//   nt = (pNameTokenInfo)malloc(sizeof(NameTokenInfo));
// 	for(i = 0;i<count;i++)
// 	{
//     if(t[i].parent == 0&& t[i].type ==3&&t[i].size==1)
//     {
//       k++;
//       ObjectSubject = (int*) realloc(ObjectSubject,sizeof(int)*k);
//       ObjectSubject[k-1] = i;
//       objectStartParent = i+1;
//       continue;
//     }
//     if(t[i].parent == objectStartParent && t[i].type == 1) //parent 수정필요.
//     {
//         j++;
//         objectStartList = (int*) realloc(objectStartList,sizeof(int)*j);
//         objectStartList[j-1] = i;
//     }
//     if(t[i].parent == (objectStartList[j-1]))
//     {
//       y++;
//       nt = (pNameTokenInfo)realloc(nt,sizeof(NameTokenInfo)*y);
// 			(nt[y-1]).tokindex = i+1;
// 			i+=7;
//     }
//   }
//   printf("\n\n\n********************************************\n");
// 	printf("번호\t제품명\t제조사\t가격\t개수\t총가격\n");
// 	printf("********************************************\n");
// 	for(i=0;i<j;i++)
// 	{
// 		nt[i].objectindex = objectStartList[i];
// 		int tok = nt[i].tokindex;
// 		int PaObject = nt[i].objectindex;
// 		printf("%d\t",i+1); //번호
// 		printf("%.*s\t",t[tok+2].end-t[tok+2].start, JSON_STRING + t[tok+2].start); //제품명
// 		printf("%.*s\t",t[tok].end-t[tok].start, JSON_STRING + t[tok].start); //제조사
// 		printf("%.*s\t",t[tok+4].end-t[tok+4].start, JSON_STRING + t[tok+4].start); //가격
// 		printf("%.*s\t",t[tok+6].end-t[tok+6].start, JSON_STRING + t[tok+6].start); //개수
//
// 		char* price = JSON_STRING + t[tok+4].start;
// 		char* numOfProduct = JSON_STRING + t[tok+6].start;
// 		int pp;
// 		int nn;
// 		sscanf(price,"%d",&pp);
// 		sscanf(numOfProduct,"%d",&nn);
// 		printf("%d\n",pp*nn); // 총가격
// 	}
// }

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
  int objectStartParent =0;
  objectStartList = (int*) malloc(sizeof(int));
  int *NameList;
  NameList = (int*) malloc(sizeof(int));
  printf("****** Name List ******\n");
	for(i = 0;i<count;i++)
	{
    if(t[i].parent == 0&& t[i].type ==3&&t[i].size==1)
    {
      objectStartParent = i+1;
      continue;
    }
    if(t[i].parent == objectStartParent && t[i].type == 1) //parent 수정필요.
    {
        j++;
        objectStartList = (int*) realloc(objectStartList,sizeof(int)*j);
        objectStartList[j-1] = i;
    }
		if(t[i].parent == (objectStartList[j-1]))
		{
      y++;
      NameList = (int*) realloc(NameList,sizeof(int)*j);
      NameList[j-1] = i+1;
			printf("[NAME %d] %.*s\n",y,t[i].end-t[i].start,
	          JSON_STRING + t[i].start);
		}
	}
  printf("\n\n\n****** Object List ******\n");
  for(i=0;i<j;i++)
  {
    printf("[Name %d] %.*s\n",i+1,t[objectStartList[i]+2].end-t[objectStartList[i]+2].start,
          JSON_STRING + t[objectStartList[i]+2].start);
  }

  int selectedNum;
  int end = count;
  while(1)
    {
      printf("원하는 번호 입력(Exit:0) :");
      scanf("%d",&selectedNum);
      if(selectedNum==0)
        break;
      else if(selectedNum<0||selectedNum>j)
        printf("그 번호의 object는 존재하지 않습니다.\n");
      else if(selectedNum>0&&selectedNum<=j)
      {
        printf("%.*s : ",t[objectStartList[selectedNum-1]+1].end-t[objectStartList[selectedNum-1]+1].start, JSON_STRING + t[objectStartList[selectedNum-1]+1].start);
        printf("%.*s\n",t[objectStartList[selectedNum-1]+2].end-t[objectStartList[selectedNum-1]+2].start, JSON_STRING + t[objectStartList[selectedNum-1]+2].start);
        if(selectedNum<j)
            end = objectStartList[selectedNum]-1;
        for(z=objectStartList[selectedNum-1]+2;z<end;z++)
        {
          if(t[z].type==3||t[z].type==2)
          {
            if(t[z].size>=1 &&t[z].parent == objectStartList[selectedNum-1])
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
  char filename[15];
	New_JSONSTRING = (char*)malloc(sizeof(char)*1);
	strcpy(New_JSONSTRING,"");
	FILE *f;
  f = fopen("myproduct.json","r");
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

	product_t *namelist[20];
	int numOfProduct = makeProduct(JSON_STRING, &t[0], r, namelist);

	printProduct(namelist,numOfProduct);

	return EXIT_SUCCESS;
}
