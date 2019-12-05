/*Huffman Code*/
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <iostream>
#define MaxN 256
using namespace std;
typedef struct {
	int parent,
		lchild,
		rchild;
	double w;
}HF_BTNode;
typedef struct {
	int n;
	unsigned char s[MaxN];
	double weight[MaxN];
	char code[MaxN][MaxN + 1];
	HF_BTNode hf[2 * MaxN - 1];
}HFT;

void count(char* fname, HFT&a)
{
	int len = 0;
	FILE *f = fopen(fname, "rb");
	if (f == NULL) exit(1);
	int c;
	while (!feof(f))
	{
		c = fgetc(f); if (c == -1)break;
		int i;
		for (i = 0; i < a.n; i++)
		{
			if ((int)a.s[i] == c)break;
		}
		if (a.weight[i] == 0)
		{
			a.s[i] = c; a.n++;
		}
		a.weight[i]++; len++;
	}
	fclose(f);
	for (int i = 0; i < a.n; i++)
		a.weight[i] /= len;
}

void createHF(HFT &a)
{
	//initialize
	int m = 2 * a.n - 1; 
	for (int i = 0; i < m; i++)
	{
		a.hf[i].parent = a.hf[i].lchild = a.hf[i].rchild = -1; 
	}
	for (int i = 0; i < a.n; i++)
		a.hf[i].w = a.weight[i];
	//create HFT
	for (int i = a.n; i < m; i++)
	{
		int j1, j2, j;
		for (j1 = 0; j1 < i; j1++) if (a.hf[j1].parent == -1)break;
		for (j = j1 + 1; j < i; j++)
			if (a.hf[j].parent == -1 && a.hf[j].w < a.hf[j1].w) j1 = j;
		a.hf[j1].parent = i;
		a.hf[i].lchild = j1;
		for (j2 = 0; j2 < i; j2++) if (a.hf[j2].parent == -1)break;
		for (j = j2 + 1; j < i; j++)
			if (a.hf[j].parent == -1 && a.hf[j].w < a.hf[j2].w) j2 = j;
		a.hf[j2].parent = i;
		a.hf[i].rchild = j2;
		a.hf[i].w = a.hf[j1].w + a.hf[j2].w;
	}
	//Generate code[]
	for (int i = 0; i < a.n; i++)
	{
		int j = i; char *p = a.code[i];
		while (a.hf[j].parent != -1)
		{
			int child = j; j = a.hf[j].parent;
			if (a.hf[j].lchild == child) *p++ = '1'; else *p++ = '0';
		}
		*p = '\0';
		char *q = a.code[i]; p--;
		while (q < p) { char ch = *q; *q = *p; *p = ch; q++; p--; }
	}
}

void Output(char* infile, char* outfile, HFT &a)
{
	FILE* inf = fopen(infile, "rb");
	if (inf == NULL) exit(1);
	FILE* outf = fopen(outfile, "wb");
	fseek(inf, 0, SEEK_SET);
	fseek(outf, 8, SEEK_SET);
	unsigned char c;
	char buf[512] = { 0 };
	int i;
	long pt = 8, flength = 0, length = 0;
	while (!feof(inf))
	{
		c = fgetc(inf); flength++;
		for (i = 0; i < a.n; i++)
			if (a.s[i] == c)break;
		strcat(buf, a.code[i]);
		c = 0;
		// handle code size of flength - flength%8
		while (strlen(buf) >= 8)
		{
			for (int j = 0; j < 8; j++)
			{
				if (buf[j] == '1') c = (c << 1) | 1; //move to left, LSB = 1
				else c = c << 1; //move to left, LSB = 0
			}
			fwrite(&c, 1, 1, outf); pt++;
			strcpy(buf, buf + 8);
		}
	}
	//handle less-than-8-bit code
	if (strlen(buf) > 0)
	{
		strcat(buf, "00000000");
		for (int j = 0; j < 8; j++)
		{
			if (buf[j] == '1') c = (c << 1) | 1;
			else c = c << 1;
		}
		fwrite(&c, 1, 1, outf);
		pt++;
	}
	length = pt - 8;
	fseek(outf, 0, SEEK_SET);
	fwrite(&length, 1, 8, outf);//save content length
	fseek(outf, pt, SEEK_SET);
	fwrite(&a.n, sizeof(int), 1, outf);//save n
	pt = pt + 4;
	//save character table
	for (i = 0; i < a.n; i++)
	{
		fwrite(&a.s[i], 1, 1, outf);
		pt++;
	}
	//save Huffman tree
	for (i = 0; i < 2 * a.n - 1; i++)
	{
		fwrite(&a.hf[i], sizeof(HF_BTNode), 1, outf);
		pt += sizeof(HF_BTNode);
	}
	fclose(inf);
	fclose(outf);
	printf("Compression ratio: %f%%\n", (float)pt / flength * 100);
}

void dec(HFT &a,char* infile, char* outfile)
{
	unsigned char buf;
	int i = 0, j = 0; long length;
	//read
	FILE* inf = fopen(infile, "rb");
	if (inf == NULL) exit(1);
	FILE* outf = fopen(outfile, "wb");
	fread(&length, 8, 1, inf);
	cout << length << endl;//
	int y;
	fseek(inf, length + 8, SEEK_SET);
	fread(&a.n, sizeof(int), 1, inf);
	for (i = 0; i < a.n; i++)
		fread(&a.s[i], 1, 1, inf);
	for (i = 0; i < 2 * a.n - 1; i++)
		fread(&a.hf[i], sizeof(HF_BTNode), 1, inf);
	fseek(inf, 8, SEEK_SET);
	int k = 2 * a.n - 2;
	for (y = 0; y < length; y++)
	{
		buf = fgetc(inf);
		unsigned m = 128;
		for (int v = 0; v < 8; v++)
		{
			unsigned char w = (buf & m);
			if (w == 0) k = a.hf[k].rchild;
			else k = a.hf[k].lchild;
			if (k < a.n)
			{
				fwrite(&a.s[k], sizeof(char), 1, outf);
				k = 2 * a.n - 2;
			}
			m = m >> 1; 
		}
	}
	fclose(inf);
	fclose(outf);
}
int main()
{
	char IN[100], OUT[100];
	while (true)
	{
		HFT a = { a.n = 0 };
		for (int i = 0; i < MaxN; i++) a.weight[i] = 0;
		cout << "-------Menu--------" << endl;
		cout << "Input 'a' to zip file, input 'b' to unzip." << endl;
		char IN[100], OUT[100], c[10];
		gets_s(c);
		cout << "Please input INFILE's name:" << endl;
		gets_s(IN);
		cout << "Please input OUTFILE's name:" << endl;
		gets_s(OUT);
		if (c[0] == 'a')
		{
			a.n = 0;
			count(IN, a);
			createHF(a);
			Output(IN, OUT, a);
			cout << "OK" << endl;
		}
		if (c[0] == 'b')
		{
			a.n = 0;
			dec(a, IN, OUT);
			cout << "OK" << endl;
		}
	}
}
