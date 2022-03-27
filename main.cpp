#include <stdio.h>
#include <stdlib.h>
#include <cmath>

typedef struct bucket
{
	int number;          //����1�ĸ���
	int timestamp;       //����ʱ���
	struct bucket *next;
}bucket,*pbucket;
 
int window = 10000;      //���ڴ�С
int count[1000001];      //��ȷ���� 
int printFreq = 100000;  //��ӡƵ�� 
bool deleted = false;    //�Ƿ���Ͱ��ɾ�� 

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
				if(i == 3)    //��������Ͱ�������һ��ʱ�����кϲ�
				{
					r->number = n * 2;    //Ͱ�������*2
					r->next = q->next;    //ɾ���ڵ�
					free(q);    //�ͷſռ�
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
	
	//pCurָ���ƶ��������ڶ���Ͱ 
	while(pNext && pNext->next) 
	{
		pCur = pCur->next;
		pNext = pCur->next;
	}
	//���һ��Ͱ����window 
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
	int sign; 		//���ڱ���01
	int integer;    //���ڱ�������
	int time = 1;	//ע��ʱ�����1��ʼ����ͳ�ƴ��ڷ�ΧΪ(time - window, time] 
 	count[0] = 0;   //��ȷ���� 
 	
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
			if(sign == 1)    //������Ϊ1������
			{
				p = (pbucket)malloc(sizeof(bucket));   //�����������һ���ڵ�
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
				h[b] = p;		//ͷ�巨
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
		time++;    //ʱ������
	}
	if(analysis_fp) fclose(analysis_fp);
	return h;
}

void destory(pbucket *h)    //��������
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
