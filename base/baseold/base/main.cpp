#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

const int dlina = 7;
const int razmer = 12;
struct Record 
{
	char ID[7];
	char string[razmer];
	int number;
};

int main()
{
	FILE *f=NULL;
	//Record element;
	f = fopen("base.dat","a+b");
	FILE *f2 = fopen("base.search","a+b");
	char names[] = {'q','w','e','r','t','y','u','i','o','p','a','s','d','f','g','h','j','k','l','z','x','c','v','b','n','m'};
	
	char j[dlina+1];
	for (int i = 0; i<dlina+1; i++)
	{
		j[i]='0';
	}

	for (long i = 0; i< 1000200;i++)
	{
		Record element ;
		if (j[0]=='Z')
		{
			j[0] = '0';
			j[1]++;
			element.ID[0]=j[0];
		} else {
			j[0]++;
			element.ID[0]=j[0];
			
		}
		for (int k = 1; k< 7; k++)
		{
			if (j[k]=='Z')
			{
				j[k] = '0';
				j[k+1]++;
				element.ID[k]=j[k];
			} else {
				element.ID[k]=j[k];
			}
		}
		element.ID[6] = 0;

		for (int j = 0; j<razmer-1; j++)
		{
			element.string[j]= names[rand()%26];
		}
		element.string[razmer-1] = 0;
		element.number = rand() % 10000;

		if (i < 1000000) fwrite(&element,sizeof(Record),1,f);
		fwrite(&(element.ID), 7, 1, f2);
		
	}
	fclose(f);
	fclose(f2);
	return 0;
}
