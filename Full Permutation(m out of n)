#include <iostream>
using namespace std;
void perm(int n, int m)
{
	int *s = new int[m];
	int *a = new int[m];
	int *b = new int[m];
	int *flag = new int[n];
	int top = -1;
	int pop = 0;
	int k = 0;
	for (int i = 0; i < m; i++)b[i] = 0;
	for (int i = 0; i < n; i++)flag[i] = 0;
	while (1)
	{
		if (top == m - 1)
		{
			for (int i = 0; i < m; i++) printf("%d ", a[i]);
			printf("\n");
			flag[b[k-1]-1] = 0;
			top--;
			k--;
			pop = 1;
		}
		else if (b[k]==n && pop == 1 && top!=-1)
		{
			b[k] = 0;
			flag[b[k-1]-1] = 0;
			top--;
			k--;
		}
		else if (flag[b[k]] == 0)
		{
			s[++top] = b[k];
			flag[b[k]] = 1;
			a[k] = b[k] + 1;
			b[k]++;
			pop = 0;
			k++;
		}
		else if (top == -1)break;
		else b[k]++;
	}
}
int main()
{
	cout << "Print perm(3, 2): " << endl;
	perm(3, 2);
	cout << "Print perm(4, 3): " << endl;
	perm(4, 3);
	system("pause");
}
