#include <stdio.h>
#include <math.h>

int count_square_numbers(int A, int B) 
{
	int j = 0;
	for (int i = A; i < B; i++) 
	{
		if ((i * i) <= B) 
		{
			j += 1;
		}
	}
	return j;
}

int main() 
{
	int A, B;
	scanf_s("%d", &A);
	scanf_s("%d", &B);

	int result = count_square_numbers(A, B);
	printf("%d\n", result);

	return 0;

}