#include <stdio.h>
#include <string.h>
#include "include/struct.h"

int AddSubscriber(struct SubscriberList *array_of_struct) {
  for(int i = 0; i < MAX_SIZE_STRUCT; i++) {
    if(array_of_struct[i].first_name[0] == 0) {
      printf("First name:\n");
      scanf("%s", array_of_struct[i].first_name);
      printf("Last name:\n");
      scanf("%s", array_of_struct[i].last_name);
      printf("Phone:\n");
      scanf("%s", array_of_struct[i].phone);
      printf("OK\n");
      return 0; 
    }
  }
  printf("Error\n");
  return 0;
}

int SearchSubscriber(struct SubscriberList *array_of_struct) {
  char search_word[MAX_LENGHT_LAST_NAME];
  printf("Search word:\n");
  scanf("%s", search_word);
  for(int i = 0; i < MAX_SIZE_STRUCT; i++) {
    if(strcmp(array_of_struct[i].phone,search_word)==0 || strcmp(array_of_struct[i].first_name,search_word)==0 || strcmp(array_of_struct[i].last_name,search_word)==0) {
      ShowOneSubscriber(array_of_struct, i);
      return i;
    } 
  }
  printf("Not found\n");
  return -1;
}

void DeleteSubscriber(struct SubscriberList *array_of_struct, int index) {
  if(index != -1) {
    for (int j = 0; j < MAX_LENGHT_FIRST_NAME; j++) {
      array_of_struct[index].first_name[j] ='\0';
    }
    for (int j = 0; j < MAX_LENGHT_LAST_NAME; j++) {
      array_of_struct[index].last_name[j] ='\0';
    }
    for (int j = 0; j < MAX_LENGHT_PHONE; j++) {
      array_of_struct[index].phone[j] ='\0';
    }
  } else {
    printf("Not found");
  }
  
}

void DisplaySubscriber(struct SubscriberList *array_of_struct) {
  for(int i = 0; i < MAX_SIZE_STRUCT; i++) {
    if(array_of_struct[i].first_name[0] != 0) {
      ShowOneSubscriber(array_of_struct, i);
    }
  }
}

int menu(int *menu_item) {
  printf("\n1. Add subscriber\n2. Search\n3. Delete\n4. Show\n5. Exit\n\n");
  scanf("%d", menu_item);
  return *menu_item;
}

void CleanStructur(struct SubscriberList *array_of_struct) {
  for (int i = 0; i < MAX_SIZE_STRUCT; i++) {
    DeleteSubscriber(array_of_struct,i);
  }
}

void ShowOneSubscriber(struct SubscriberList *array_of_struct, int index) {
  printf("First name: %s\t", array_of_struct[index].first_name);
  printf("Last name: %s\t", array_of_struct[index].last_name);
  printf("Phone: %s\t", array_of_struct[index].phone);
  printf("\n");
}
