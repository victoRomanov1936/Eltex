#include <stdio.h>
#include "include/function.h"

int main(void) {
  int first_num, second_num;
  int menu_item = 0;
  int result = 0;
  while (menu_item != 5) {
    menu(&menu_item, &first_num, &second_num);
    switch(menu_item) {
      case 1:
        printf("%d\n",addition(first_num,second_num));
      	break;
      case 2:
      	printf("%d\n",substraction(first_num,second_num));
      	break;
      case 3:
        printf("%d\n",multiplication(first_num,second_num));
      	break;
      case 4:
      	if(second_num!=0) {
          printf("%d\n",division(first_num,second_num));
        } else {
          printf("Error");
        }
      	break;
      case 5:
      	break;
    }
  }
  
  return 0;
 }
