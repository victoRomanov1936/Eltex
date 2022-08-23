#include <stdio.h>
#include "include/struct.h"

int main(void) {
  struct SubscriberList array_of_subscriber[MAX_SIZE_STRUCT];
  CleanStructur(array_of_subscriber); 
  int menu_item = 1;
  while(menu_item != 5){
    menu(&menu_item);
    switch (menu_item) {
      case 1:
        AddSubscriber(array_of_subscriber);
	break;
      case 2:
        SearchSubscriber(array_of_subscriber);
	break;
      case 3:
        DeleteSubscriber(array_of_subscriber,SearchSubscriber(array_of_subscriber));
	break;
      case 4:
        DisplaySubscriber(array_of_subscriber);
	break;
      case 5:
          printf("End\n");
	  break;
      default:
	printf("Error\n");
    }
  }

  return 0;
}
