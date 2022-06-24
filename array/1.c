#include <stdio.h>
#define n 3
int main() {
    int el=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) {
            el++;
            printf("%d ",el);
        }
        printf("\n");
    }
    
    return 0;
}