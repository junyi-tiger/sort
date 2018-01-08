#include<iostream>
#include<stdlib.h>
#include<ctime> 
#include<windows.h>
#define ElemType int
using namespace std;
const int N=2000;
int a[N];
const int Cutoff=100;
//快速排序的阈值 当待排序元素数量小于之，改用简单排序 
void Swap(ElemType &a1,ElemType &a2)//交换两元素的值 
{
	int t=a1;
	a1=a2;
	a2=t;
}
void Bubble_sort(ElemType a[],int N)//冒泡排序 
{
	for(int P=N-1;P>1;P--)
	{
		int flag=0;
		for(int i=0;i<P;i++)
		{
			if(a[i]>a[i+1])
			{
				Swap(a[i],a[i+1]);
				flag=1;
			}
			
		}
		if(flag==0)break;
	}
}
void Insertion_sort(ElemType a[],int N)//简单插入排序
{
	for(int P=1;P<N;P++)
	{
		int tmp=a[P];
		int i=P;
		for(;i>0&&a[i-1]>tmp;i--)//移出空位 
		{
			a[i]=a[i-1];
		}
		a[i]=tmp;//插入 
	}
}
ElemType Median3(ElemType a[],int left,int right)//供快速排序调用 
{
	int center=(left+right)/2;
	if(a[left]>a[center])Swap(a[left],a[center]);
	if(a[left]>a[right])Swap(a[left],a[right]);
	if(a[center]>a[right])Swap(a[center],a[right]);
	Swap(a[center],a[right-1]);//将pivot置于右边
	//只需要考虑a[left+1]......a[right-2]
	return a[right-1]; 
}
void Quicksort(ElemType a[],int left,int right)//快速排序 
{
	if(Cutoff<=right-left){
	int pivot=Median3(a,left,right);
	int i=left;
	int j=right-1;
	for(;;)
	{
		while(a[++i]<pivot){}
		while(a[--j]>pivot){}
		if(i<j)Swap(a[i],a[j]);
		else break;
	}
	Swap(a[i],a[right-1]);
	Quicksort(a,left,i-1);
	Quicksort(a,i+1,right);
	}
	else Insertion_sort(a+left,right-left+1);//参数1：排序开始地址  参数2：待排序的个数 
}
void Quick_sort(ElemType a[],int N)//快速排序 
{
	Quicksort(a,0,N-1);
}
//对序列a中元素进行调整，使其成为一个最大堆，已知从s到m的位置只有s位置的元素不满足最大堆的定义 
void HeapAdjust(ElemType a[],int root,int len)//s：根节点位置 m：当前堆里面的元素个数
{
	for(int j=2*root+1;j<len;j=2*j+1)//因为下标从0开始，所以s的左子树为2*s+1,右子树为2*s+2
	{
		if(j+1<len&&a[j]<a[j+1])j++;//j为较大记录的下标 
		if(a[root]<a[j])
		{
			Swap(a[root],a[j]);
			root=j;
		}
		else break;
	}
}
void Heap_sort(ElemType a[],int N)//堆排序 
{
	for(int i=N/2;i>=0;i--)//建立一个最大堆
	{
		HeapAdjust(a,i,N);
	}
	for(int i=N-1;i>0;i--)
	{
		Swap(a[0],a[i]);//此时a[0]处的元素在0到i这段中最大，将其置于末尾
		HeapAdjust(a,0,i);
	}
}
//以下属归并排序，运用了非递归的算法实现 
void Merge(ElemType a[],ElemType Tmp[],int L,int R,int RightEnd)
//将两个有序子列合并，结果存于Tmp数组 
// L=起始位置 R=右边起始位置 RigthEnd=右边终点位置 
{
	int LeftEnd=R-1;//左边终点位置
	int temp=L;//存放结果的数组的起始位置
	while(L<=LeftEnd&&R<=RightEnd)
	{
		if(a[L]<=a[R])Tmp[temp++]=a[L++];
		else Tmp[temp++]=a[R++];
	}
	while(L<=LeftEnd)//直接复制左边剩下的 
	Tmp[temp++]=a[L++];
	while(R<=RightEnd)//直接复制右边剩下的 
	Tmp[temp++]=a[R++];
}
void Merge_pass(ElemType a[],ElemType Tmp[],int N,int length)
//每一趟归并：length=当前有序子列的长度 
{ 
	int i;
	for(i=0;i<=N-2*length;i+=2*length)
	Merge(a,Tmp,i,i+length,i+2*length-1);
	if(i+length<N)//归并最后两个子列
	Merge(a,Tmp,i,i+length,N-1);
	else//最后只剩一个子列
	 for(int j=i;j<N;j++)Tmp[j]=a[j];
}
void Merge_sort(ElemType a[],int N)
{
	int length=1;
	ElemType *Tmp;
	Tmp=(ElemType*)malloc(N*sizeof(ElemType));
	if(Tmp!=NULL)
	{
		//最终结果存在数组a中 
		while(length<N)
		{
			Merge_pass(a,Tmp,N,length);
			length*=2;
			Merge_pass(Tmp,a,N,length);
			length*=2;
		}
		free(Tmp);
	}
	else cerr<<"空间不足！"<<endl;
}
int main()
{
	srand(time(NULL));
	
	for(int k=1;k<=5;k++)//不同的排序算法 
	{
		double sum=0.0;
		for(int j=0;j<1000;j++)
		{
			for(int i=0;i<N;i++)//随机生成数组大小为2000的数组 
			a[i]=rand()%1000;
			LARGE_INTEGER fre;
			LARGE_INTEGER begin;
			LARGE_INTEGER end;
			QueryPerformanceFrequency(&fre);
			QueryPerformanceCounter(&begin);
			switch(k)
			{
				case 1:Bubble_sort(a,N);break;
				case 2:Insertion_sort(a,N);break;
				case 3:Quick_sort(a,N);break;
				case 4:Heap_sort(a,N);break;
				case 5:Merge_sort(a,N);break;
			}
			QueryPerformanceCounter(&end);
			double runtime=(double)(end.QuadPart-begin.QuadPart)*1000/fre.QuadPart;
			sum+=runtime;
//			cout<<"k="<<k<<" time="<<runtime<<"ms"<<endl;
			for(int i=0;i<N;i++)//重新赋值 
			{
				a[i]=rand()%1000;
			}
		}
		switch(k)
		{
			case 1:cout<<"冒泡";break;
			case 2:cout<<"直接插入";break;
			case 3:cout<<"快速";break;
			case 4:cout<<"堆";break;
			case 5:cout<<"归并";break;
		}
		cout<<"排序所用平均时间为："<<sum/1000<<"ms"<<endl;
	}
}
