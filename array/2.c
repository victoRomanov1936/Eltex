#include <stdio.h>
#define N 5
int main() {
    int arr[N];
    for (int i = 0;i < N;i++) {
        arr[i] = i + 1;
        printf("%d ", arr[i]);
    }
    printf("\n");
    for (int i = N - 1;i >= 0;i--) {
        printf("%d ", arr[i]);
    }

    return 0;
}