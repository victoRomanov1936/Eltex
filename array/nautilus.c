#include <stdio.h>
#define N 4

int main(void){
	int arr[N][N];
	int n=1;
	int i=0,j=0,count=N;
	int ibeg=i,jbeg=j;
	
	for(int k=0;k<N;k++) {
	
		for(;j<count+jbeg;j++)arr[i][j]=n++;
		count--,j--,i++;
		ibeg=i;
		
		for(;i<count+ibeg;i++)arr[i][j]=n++;
		
		i--,j--,jbeg=j;
		
		for(;j>jbeg-count;j--)arr[i][j]=n++;
		count--,i--,j++;
		ibeg=i;

		for(;i>ibeg-count;i--)arr[i][j]=n++;
		i++,j++,jbeg=j;
	}
	
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			printf("%d ",arr[i][j]);
		}
		printf("\n");
	}
	return 0;
}
