#include <malloc.h>
#include <stdio.h>
#include "include/struct.h"

int main(void) {
  int size =0;
  struct SubscriberList *array_of_subscriber = malloc(size * sizeof(struct SubscriberList));
  if(array_of_subscriber!=NULL) {
  int menu_item = 0;
  while(menu_item != 5) {
    menu(&menu_item);
    switch (menu_item) {
      case 1:
        size++;
        AddSubscriber(array_of_subscriber, size);
	break;
      case 2:
        SearchSubscriber(array_of_subscriber, size);
	break;
      case 3:
        DeleteSubscriber(array_of_subscriber,SearchSubscriber(array_of_subscriber,size), &size);
        array_of_subscriber = realloc(array_of_subscriber, size);
	break;
      case 4:
        DisplaySubscriber(array_of_subscriber, size);
	break;
      case 5:
          printf("End\n");
	  break;
      default:
	printf("Error\n");
    }
  }
  }
  free(array_of_subscriber);
  return 0;
}
