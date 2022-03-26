#include <stdio.h>
#include <stdlib.h>
 
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
  
int judge(pbucket h,int n);
void deleteExcess(pbucket h, int time, int window);
 
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
 
pbucket memory(bool printbuckets=false)
{
	int sign;       //���ڱ������������
	int time = 1;	//ע��ʱ�����1��ʼ����ͳ�ƴ��ڷ�ΧΪ(time - window, time] 
	count[0] = 0;
 
	FILE *fp;
 
	pbucket p, h = NULL;
 
	fp = fopen("01stream.txt","r");
	FILE *analysis_fp = fopen("./results.txt","w");
 
	while((feof(fp) == 0))    
	{
		fscanf(fp,"%d",&sign);
 
		if(sign == 1)    //������Ϊ1������
		{
			count[time] = count[time - 1] + 1;
			p = (pbucket)malloc(sizeof(bucket));   //�����������һ���ڵ�
			p->timestamp = time;
			p->number = 1;
			if(h)
			{
				p->next = h;    
			}
			if(!h)
			{
				p->next = NULL;
			}
			h = p;		//ͷ�巨
			judge(h,1);
		}
		else
		{
			count[time] = count[time - 1];
		}
		
 		deleteExcess(h, time, window);	
			
		if(time % printFreq == 0 && !printbuckets)
		{
			int prediction = DGIM(h, time, window);
			printf("Timestamp = %d\n", time);
			printf("\testimate: %d\n", prediction);
			printf("\ttruth: %d\n", count[time] - count[time - window]);
			printf("\n");
			fprintf(analysis_fp,"%d\t%d\t%d\n",time,prediction,count[time] - count[time - window]);
		}
		if(printbuckets &&(time == 10000 || time == 500000 || time == 1000000)){
			print_buckets_at(time,h);
		}
		time++;    //ʱ������
	}
	fclose(analysis_fp);
	return h;
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
	pbucket h,q;
	h = memory(false);
	q = memory(true);
	destory(&h);
	destory(&q);
 
	return 0;
}
