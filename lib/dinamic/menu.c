#include <stdio.h>
 void menu(int *menu_item, int *first_num, int *second_num) {
  printf("\n1. Addition\n2. Subtraction\n3. Multiplication\n4. Division\n5. Exit\n\n");
  scanf("%d", menu_item);
  if(*menu_item != 5) {
  printf("\nFirst number : ");
  scanf("%d", first_num);
  printf("Second number : ");
  scanf("%d", second_num);
   }
}
