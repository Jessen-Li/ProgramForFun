/* Generate a fixed-size (here is 10) list of random-sized (here no bigger than 10) arrays whose elements' value is also random but on larger than a number 
(here is 100). Find all combinations of elements in the arrays (at least one, at most all) whose sum is equal to a given number (here is 100), and print them 
out to a file.*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

const unsigned int ui_val = 100;  //the cap on the value of elements
char strbuf[64]={0};    //the buff holding the string written to the file
char * pobuf=strbuf; //reserved for the starting point of strbuf
char *pbuf=strbuf;   //trace the current position in strbuf
int str_len=0;  //length of string written in strbuf

struct uiArray
{
	unsigned int size;
	unsigned int * arr;
};

void init_uiarrays(unsigned int num, struct uiArray ** pArrs)
{
	struct uiArray * uiArrs;
	uiArrs=malloc(num*sizeof(struct uiArray));
	//printf("Enter init_uiarrays, num=%u, uiArrs=%p, sizeof(uiArrs)=%ld\n", num, uiArrs, sizeof(uiArrs));
	for(int i=0; i<num; i++)
	{
		struct uiArray * uiAr = uiArrs+i;
		uiAr->size=(unsigned int)(rand()%10);
		//printf("init: uiAr.size is %u\n",uiAr->size);
		if(uiAr->size==0)
			uiAr->size=1;
		uiAr->arr=malloc(uiAr->size*sizeof(unsigned int));
		for(int j=0;j<uiAr->size;j++)
		{
			uiAr->arr[j]=(unsigned int)(rand()%100);
		}
	}
	*pArrs=uiArrs;
}

//print the list of struct uiArray holding the randomly generated arrays to a file 
void print_uiarrays(unsigned int num, struct uiArray * arrs)
{
	FILE * f_arrs;
	f_arrs=fopen("uiarrays.txt","w");
	if(!f_arrs)
	{
		printf("fopen failed!\n");
		return;
	}
	//printf("arrs = %p\n", arrs);

	for(int i=0; i<num; i++)
	{
		char buf[100];
		int n=0;
		struct uiArray uiAr;
		uiAr=arrs[i];
		n+=snprintf(buf,10,"Array %d: ", i);
		//printf("The %d uiAr has %d elements:\n",i,uiAr.size);
		for(int j=0;j<uiAr.size;j++)
		{
			n+=snprintf(buf+n,6,"%u ",uiAr.arr[j]);
		}
		n+=sprintf(buf+n,"\n");
		//printf("The buf is %s",buf);
		fwrite(buf,1,n,f_arrs);
	}
	fclose(f_arrs);
}

void free_uiarrays(unsigned int num, struct uiArray * pArrs)
{
	for(int i=0; i< num; i++)
	{
		struct uiArray uiAr=pArrs[i];
		free(uiAr.arr);
	}
	//printf("after free uiAr.arr\n");
	free(pArrs);
}

//find all the commbinations of elements in the arrays whose sum is equal to a given number and print them out to a file 
void cal_sum(unsigned int pre_sum, unsigned int cPos, unsigned int size, struct uiArray * pArrs)
{
	static int count =0;
	int sum = pre_sum;
	FILE * f_arrs;
	
	
	if (cPos < size-1)
	{
		cal_sum(sum,cPos+1,size,pArrs); //go to the last element in pArrs first   
	}
	for(int i=0;i<pArrs[cPos].size;i++)
	{
		sum+=pArrs[cPos].arr[i];
		if(sum==ui_val)
		{
			f_arrs=fopen("uiarrays.txt","a");
			if(f_arrs)
			{
				int n=0;
				n=sprintf(pbuf,"%u ",pArrs[cPos].arr[i]);
				n+=sprintf(pbuf+n, " - Bingo, count=%d\n", count++);
				fwrite(pobuf, 1, str_len+n, f_arrs);
				fclose(f_arrs);
			}
			sum-=pArrs[cPos].arr[i];
		}
		else if(sum<ui_val)
		{
			if(cPos==size-1)  //already the last element in pArrs
			{
				sum-=pArrs[cPos].arr[i];
			}
			else
			{
				int n=0;
				n=sprintf(pbuf,"%u ",pArrs[cPos].arr[i]);
				pbuf+=n;
				str_len+=n;
				cal_sum(sum,cPos+1,size,pArrs); // go to the next element in pArrs
				sum-=pArrs[cPos].arr[i];
				pbuf-=n;
				str_len-=n;
				memset(pbuf,0,n);
			}
		}
		else
		{
			sum-=pArrs[cPos].arr[i];
		}
	}

}

int main(void) {
	
  struct uiArray * puiArr;
	
	
	init_uiarrays(10,&puiArr);
	
	print_uiarrays(10,puiArr);
	
	cal_sum(0,0,10,puiArr);

	free_uiarrays(10,puiArr);
	
	return 0;
}
