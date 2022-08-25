#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "include/struct.h"

#define SIZE_ARRAY  sizeof(*array_of_struct)/sizeof(array_of_struct[0])

int AddSubscriber(struct SubscriberList *array_of_struct, int size) {
  array_of_struct = realloc(array_of_struct, size);
  if(array_of_struct!=NULL) {
    printf("%d\n", size);
    printf("First name:\n");
    scanf("%s", array_of_struct[size].first_name);
    printf("Last name:\n");
    scanf("%s", array_of_struct[size].last_name);
    printf("Phone:\n");
    scanf("%s", array_of_struct[size].phone);
    printf("OK\n");
    return 1;
   }
  printf("Error\n");
  return 0;
}

int SearchSubscriber(struct SubscriberList *array_of_struct, int size) {
  char search_word[MAX_LENGHT_LAST_NAME];
  printf("Search word:\n");
  scanf("%s", search_word);
  for(int i = 0; i < size+1; i++) {
    if(strcmp(array_of_struct[i].phone,search_word)==0 || strcmp(array_of_struct[i].first_name,search_word)==0 || strcmp(array_of_struct[i].last_name,search_word)==0) {
      ShowOneSubscriber(array_of_struct, i);
      return i;
    } 
  }
  printf("Not found\n");
  return -1;
}

void DeleteSubscriber(struct SubscriberList *array_of_struct, int index,int *size) {
  struct SubscriberList save_struct[1];
  if(index != -1) {
    save_struct[0] = array_of_struct[*size];
    array_of_struct[*size] = array_of_struct[index];
    array_of_struct[index] = save_struct[0];
    *size-=1;
  } 
}

void DisplaySubscriber(struct SubscriberList *array_of_struct, int size) {
  for(int i = 1; i < size+1; i++) {
    ShowOneSubscriber(array_of_struct, i);
  }
}

void ShowOneSubscriber(struct SubscriberList *array_of_struct, int index) {
  printf("First name: %s\t", array_of_struct[index].first_name);
  printf("Last name: %s\t", array_of_struct[index].last_name);
  printf("Phone: %s\t", array_of_struct[index].phone);
  printf("\n");
}

int menu(int *menu_item) {
  printf("\n1. Add subscriber\n2. Search\n3. Delete\n4. Show\n5. Exit\n\n");
  scanf("%d", menu_item);
  return *menu_item;
}

