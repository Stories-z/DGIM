#include <stdio.h>
#include <stdlib.h>
#include <cmath>

typedef struct bucket
{
	int number;          //保存1的个数
	int timestamp;       //保存时间戳
	struct bucket *next;
}bucket,*pbucket;
 
int window = 10000;      //窗口大小
int count[1000001];      //精确计数 
int printFreq = 100000;  //打印频率 
bool deleted = false;    //是否有桶被删除 

int judge(pbucket h,int n)
{
	pbucket p,q,r;
	int i = 0;
	p = q = h;
 
//	r = p->next;
	while(p)
	{
		if(q)
		{
			if(q->number == n)
			{
				i++;
				if(i == 3)    //当有三个桶里面的数一样时，进行合并
				{
					r->number = n * 2;    //桶里面的数*2
					r->next = q->next;    //删除节点
					free(q);    //释放空间
					n *= 2;
					p = p->next;
					judge(p,n);
					break;
				}
				else
				{
					r = q;
					q = q->next;
				}
			}
			else
				break;
		}
		else
			break;
	}
	return 0;
}
 
void deleteExcess(pbucket h, int time, int window) {
	pbucket pCur, pNext;
	pCur = h;
	pNext = pCur->next;
	
	//pCur指针移动到倒数第二个桶 
	while(pNext && pNext->next) 
	{
		pCur = pCur->next;
		pNext = pCur->next;
	}
	//最后一个桶超出window 
	if(pNext && pNext->timestamp <= (time - window)) 
	{
		deleted = true;
		pCur->next = NULL;
		free(pNext);
	}
}

int DGIM(pbucket h, int time, int window)
{
	pbucket q = h;
	int sum = 0;
	while(q)
	{
		int toAdd = q->number;
		if(q->next == NULL && deleted) 
			toAdd /= 2;
		sum += toAdd;
		q = q->next;
	}
	return sum;
}

void print_buckets_at(int curTime,pbucket h){
	if(!h) return;
	printf("************************************\n");
	printf("Timestamp = %d\n",curTime);
	while(h){
		printf("%d,%d->",h->timestamp,h->number);
		h = h->next;
	}
	printf("null\n");
}


pbucket * memory(bool printbuckets=false)
{ 
	int sign; 		//用于保存01
	int integer;    //用于保存整数
	int time = 1;	//注意时间戳从1开始，且统计窗口范围为(time - window, time] 
 	count[0] = 0;   //精确计数 
 	
	FILE *fp[7];
	char biFileName[7][30];
	for(int i = 0; i < 7; i++) 
		sprintf(biFileName[i], "Binary_100w_%d.txt", i);

	for(int i = 0; i < 7; i++)
		fp[i] = fopen(biFileName[i], "r");
	
	FILE *ifp = fopen("Integer_100w.txt", "r");
		
	pbucket p;
	pbucket h[7];
	
	for(int i = 0; i < 7; i++)
		h[i] = NULL;
 
	FILE *analysis_fp = NULL;
 
	while(fscanf(ifp,"%d",&integer) == 1)    
	{
		count[time] = count[time - 1] + integer;
		for(int b = 0; b < 7; b++) 
 		{
		 	fscanf(fp[b], "%d", &sign);
			if(sign == 1)    //数据流为1入链表
			{
				p = (pbucket)malloc(sizeof(bucket));   //存进来就申请一个节点
				p->timestamp = time;
				p->number = 1;
				if(h[b])
				{
					p->next = h[b];    
				}
				if(!h[b])
				{
					p->next = NULL;
				}
				h[b] = p;		//头插法
				judge(h[b],1);
			}

			if(h[b])
	 			deleteExcess(h[b], time, window);	
		}
		
		if(time % printFreq == 0 && !printbuckets)
		{
			if(!analysis_fp) analysis_fp = fopen("./results.txt","w");
			int prediction = 0;
			for (int i = 0; i < 7; i++)
				prediction += DGIM(h[i], time, window) * pow(2, i);
				
			printf("Timestamp = %d\n", time);
			printf("\testimate: %d\n", prediction);
			printf("\ttruth: %d\n", count[time] - count[time - window]);
			printf("\n");
			fprintf(analysis_fp,"%d\t%d\t%d\n",time,prediction,count[time] - count[time - window]);
		}
		//if(printbuckets &&(time == 10000 || time == 500000 || time == 1000000)){
		//	print_buckets_at(time,h);
		//}
		time++;    //时间流动
	}
	if(analysis_fp) fclose(analysis_fp);
	return h;
}

void destory(pbucket *h)    //销毁链表
{
	pbucket p,q;
	p=*h;
	while(p)
	{
		q=p;
		p=p->next;
		free(q);
	}
	*h=NULL;
}
 
int main()
{
	
	pbucket *h,q;
	h = memory(false);
	//q = memory(true);
	for(int i = 0; i < 7; i++)
		destory(&(h[i]));
	
 	
	return 0;
}
