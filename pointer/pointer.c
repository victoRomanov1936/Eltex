#include <stdio.h>
int main(void) {
int num = 123456789;
char* num_pointer = &num;
for(int i=0;i<sizeof(int);i++){
printf("%d\n",num_pointer[i]);
}
num_pointer[sizeof(int)-1]=2;
printf("%d\n",num);
return 0;
}
