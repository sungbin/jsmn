#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** my def**/
typedef struct NameTokenInfo *pNameTokenInfo;
typedef struct NameTokenInfo {
  int tokindex;
  int objectindex;
} NameTokenInfo;

typedef enum {
  APPLE = 1, //애플
  SAMSUNG = 2 //삼성
} company_t;

typedef struct {
  company_t company; //제조사
  char name[20]; //제품명
  int price; //가격
  int count; //개수
  char shopping_mall[20]; //구매처
  char status[20]; //상태(new, used)
} product_t;
