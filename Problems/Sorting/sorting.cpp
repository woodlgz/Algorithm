
/*
 * Common Sorting Algorithm
 *		by woodlgz
 */

#include <iostream>
#include <string.h>
#include <cstdlib>

using namespace std;

typedef int (*CMPFUNC)(void* a,void* b);

template<typename T>
void DumpArray(T array[],int size){
	for(int i=0;i<size;i++)cout<<array[i]<<" ";
	cout<<std::endl;
}

template<typename T>
void BubleSort(T array[],int size,CMPFUNC cmp){
	for(int i=size-1;i>=0;i--){
		T tmp;
		for(int j=0;j<i;j++){
			if(cmp(&array[j],&array[j+1])>0){
				tmp = array[j+1];
				array[j+1] =array[j];
				array[j] = tmp;
			}
		}
	}
}

template<typename T>
void InsertSort(T array[],int size,CMPFUNC cmp){
	for(int i=1;i<size;i++){
		T tmp = array[i];
		int j;
		for(j=i-1;j>=0;j--){
			if(cmp(&tmp,&array[j])<0){
				array[j+1] = array[j];
			}else {
				break;
			}
		}
		array[j+1] = tmp;
	}
}

template<typename T>
void _ShellSort(T array[],int size,int k,CMPFUNC cmp){
	for(int i=k;i<size;i++){
		T tmp = array[i];
		int j;
		for(j=i-k;j>=0;j-=k){
			if(cmp(&tmp,&array[j])<0){
				array[j+k]=array[j];
			}else {
				break;
			}
		}
		array[j+k] = tmp;
	}
}

template<typename T>
void ShellSort(T array[],int size,CMPFUNC cmp){
	int k[3] = {5,3,1};
	for(int i=0;i<3;i++)_ShellSort<T>(array,size,k[i],cmp);
}

template<typename T>
void Swap(T &a,T &b){
	T tmp = a;
	a = b;
	b = tmp;
}

template<typename T>
void SelectionSort(T array[],int size,CMPFUNC cmp){
	int max;
	for(int i=size-1;i>=0;i--){
		max = i;
		for(int j=0;j<=i;j++){
			if(cmp(&array[max],&array[j])<=0)max = j;
		}
		Swap<T>(array[i] , array[max]);
	}
}

template<typename T>
int Partition(T array[],int size,CMPFUNC cmp){
	int low =0,high = size-1;
	int key = array[low];
	while(low<high){
		while(low<high&&cmp(&array[high],&key)>=0)high--;
		array[low] =array[high];
		while(low<high&&cmp(&array[low],&key)<=0)low++;
		array[high]=  array[low];
	}
	array[low] = key;
	return low;
}

template<typename T>
void QuickSort(T array[],int size,CMPFUNC cmp){
	if(size<=1)return;
	int k= Partition<T>(array,size,cmp);
	QuickSort<T>(array,k,cmp);
	QuickSort<T>(array+k+1,size-k-1,cmp);
}

template<typename T>
void HeapAdjust(T array[],int start,int size,CMPFUNC cmp){
	int left= start<<1,right =(start<<1)+1;
	if(left<=size){
		int max = start;
		if(left<=size&&cmp(&array[max-1],&array[left-1])<0){
			max = left;
		}
		if(right<=size&&cmp(&array[max-1],&array[right-1])<0){
			max = right;
		}
		if(max!=start){
			T tmp = array[start-1];
			array[start-1] = array[max-1];
			array[max-1] =tmp;
		}
		HeapAdjust<T>(array,start*2,size,cmp);
		HeapAdjust<T>(array,start*2+1,size,cmp);
	}
}

template<typename T>
void HeapSort(T array[],int size,CMPFUNC cmp){
	for(int i=(size>>1)+1;i>=0;i--)HeapAdjust<T>(array,i+1,size,cmp);
	for(int i=0;i<size;i++){
		T tmp = array[0];
		array[0] = array[size-i-1];
		array[size-i-1] = tmp;
		HeapAdjust<T>(array,1,size-i-1,cmp);
	}
}

template<typename T>
void Merge(T X[],int sizex,T Y[],int sizey,CMPFUNC cmp){
	int i=0,j=0,k=0,size = sizex+sizey;
	T* tmp= new T[size];
	while(k<size){
		if(i==sizex){
			tmp[k++] = Y[j++];
			continue;
		}
		if(j==sizey){
			tmp[k++] = X[i++];
			continue;
		}
		while(i<sizex&&j<sizey&&cmp(&X[i],&Y[j])<=0){
			tmp[k++] = X[i++];
		}
		while(i<sizex&&j<sizey&&cmp(&Y[j],&X[i])<0){
			tmp[k++] = Y[j++];
		}
	}
	for(int i=0;i<sizex;i++)X[i]=tmp[i];
	for(int i=0;i<sizey;i++)Y[i]=tmp[sizex+i];
	delete[] tmp;
}

template<typename T>
void MergeSort(T array[],int size,CMPFUNC cmp){
	if(size<=1)return;
	MergeSort<T>(array,size>>1,cmp);
	MergeSort<T>(array+(size>>1),size-(size>>1),cmp);
	Merge<T>(array,size>>1,array+(size>>1),size-(size>>1),cmp);
}

int CMP(void* a,void* b){
	int _a = *(int*)a;
	int _b = *(int*)b;
	if(_a>_b)return 1;
	else if(_a<_b)return-1;
	else return 0;
}
template<typename T>
int BinarySearch1(T array[],int size,T key,CMPFUNC cmp){
	int l=0,u=size-1;
	int mid;
	while(l<u){
		mid = l+((u-l)>>1);
		if(cmp(&array[mid],&key)<0)l=mid+1;
		else u=mid;
	}
	return u;
}

template<typename T>
int BinarySearch(T array[],int size,T key,CMPFUNC cmp){
	int l=0,u=size-1;
	int mid;
	while(l<=u){
		mid=l+(u-l)>>1;
		if(cmp(&array[mid],&key)<0)l=mid+1;
		else u = mid-1;
	}
	return l;
}

//3 way Quicksort
template<typename T>
void ThreeWayQSort(T A[],int size,CMPFUNC cmp){
	int low=-1,high=size-1,p=low,q=high;
	if(low>=high)return;
	if(size>2)std::swap(A[rand()%(high-1)],A[high]);
	T key=A[high];
	for(;;)
	{
		while(cmp(&A[++low],&key)<0);
		while((--high)>=0&&cmp(&A[high],&key)>0);
		if(high<=low)break;
		std::swap(A[low],A[high]);
		if(cmp(&A[low],&key)==0){p++;std::swap(A[p],A[low]);}
		if(cmp(&A[high],&key)==0){q--;std::swap(A[q],A[high]);}
	}
	std::swap(A[low],A[size-1]);
	high=low+1;
	low=low-1;
	for(int k=0;k<=p;k++,low--){std::swap(A[k],A[low]);}
	for(int k=size-2;k>=q;k--,high++){std::swap(A[k],A[high]);}
	ThreeWayQSort(A,low+1,cmp);
	ThreeWayQSort(A+high,size-high,cmp);
}

//strs:原始字符串集合
//Sizes:原始字符串大小集合
//Sets:输入字符串序列集合,同时也是输出字符串序列集合
//start,end:start和end标定Sets的子集,子集是进行RadixSort的最小单位
//index:在本次RadixSort中Radix取决于字符串中第index个字符，index从0开始
void RadixSort(const char** strs,int Sizes[],int Sets[],int start,int end,int index){
	int Radix[27][100];
	int SRadix[27];
	memset(SRadix,0,sizeof(SRadix));
	for(int i=start;i<=end;i++){
		if(index>Sizes[Sets[i]])continue;
		char k=strs[Sets[i]][index];
		k=k=='\0'?0:k-0x61+1;
		Radix[k][SRadix[k]]=Sets[i];
		SRadix[k]++;
	}
	int s=start;
	for(int i=0;i<27;i++){
		for(int j=0;j<SRadix[i];j++)Sets[s+j]=Radix[i][j];
		if(SRadix[i]<=1||i==0){s+=SRadix[i];continue;}
		RadixSort(strs,Sizes,Sets,s,s+SRadix[i]-1,index+1);
		s+=SRadix[i];
	}
}

void StringSort(const char** strs,int size){
	int* Sizes = new int[size];
	int* Result=new int[size];
	for(int i=0;i<size;i++){
		Sizes[i]=strlen(strs[i]);
		Result[i]=i;
	}
	RadixSort(strs,Sizes,Result,0,size-1,0);
	for(int i=0;i<size;i++){
		cout<<strs[Result[i]]<<endl;
	}
	delete[] Sizes;
	delete[] Result;
}

int main(){
	//int array[]={1,1,3,2,2,2,9,8,7,7,7,3,3,3};
	//int size = sizeof(array)/sizeof(int);
	//ThreeWayQSort(array,size,CMP);
	//DumpArray(array,size);
	const char* strs[]={"abcdef","abc","bedf","bcde","adefg","acd","defgh","acd","adefafsdlakjfs"};
	int size= sizeof(strs)/sizeof(const char*);
	StringSort(strs,size);	
}
