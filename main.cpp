#include <stdio.h>
#include <stdlib.h>
 
typedef struct bucket
{
	int number;    //保存1的个数
	int timestamp;    //保存时间戳
	struct bucket *next;
}bucket,*pbucket;
 
int windows;    //窗口大小
int buckets;    //桶个数
int time;       //任意时间
int numbers;    //数据流
int count;      //精确计数
 
int judge(pbucket h,int n);
 
pbucket memory()
{
	int sign;    //用于保存读出来的数
	int i = 1;
 
	FILE *fp;
 
	pbucket p,q;
	pbucket h = NULL;
 
//	windows = 1000;
	count = 0;
	numbers = 0;
 
	printf("请输入窗口的大小\n");
	scanf("%d",&windows);
	printf("请输入查看的时间\n");
	scanf("%d",&time);
 
	fp = fopen("E:\\大数据\\实验1\\01stream.txt","r");
 
	while((feof(fp) == 0) && (numbers < time))    //读取查看时间之前的数据流
	{
		fscanf(fp,"%d",&sign);
//	    printf("sign = %d\n",sign);
 
		if(sign == 1)    //数据流为1入链表
		{
			if(numbers > (time - windows))    //用于精确统计
			{
				count++;
			}
 
			p = (pbucket)malloc(sizeof(bucket));    //存进来就申请一个节点
			p->timestamp = i;
			p->number = 1;
			q = p;
			if(h)
			{
				q->next = h;    //第一个直接插在h后面，其他使用头插法
			}
			if(!h)
			{
				p->next = NULL;
			}
			h = q;
			judge(h,1);
		}
 
		i++;    //用于记录时间戳
		numbers++;
	}
 
	return h;
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
	int sum = 0;
 
	pbucket h,q;
	h = memory();
	q = h;
 
	buckets = 0;
 
	while(q)
	{
		if(q->timestamp > (time - windows))
		{
			if(q->next->timestamp <= (time - windows))
			{
				q->number /= 2;
			}
			sum += q->number;
			if(q->next->timestamp < (time - windows))
			{
				q->number *= 2;
			}
			printf("q->timestamp = %d\n",q->timestamp);
			printf("bucket = %d\n",q->number);
		    buckets++;
		}
		q = q->next;
	}
 
	printf("sum = %d\n",sum);
	printf("buckets = %d\n",buckets);
	printf("windows = %d\n",windows);
	printf("count = %d\n",count);
 
	destory(&h);
 
	return 0;
}
