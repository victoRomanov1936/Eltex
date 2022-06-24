#include <stdio.h>
#define n 4
int main() {
    int el=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) {
            if(j>=n-1-i)printf("1 ");
            else printf("0 ");
        }
        printf("\n");
    }
    
    return 0;
}