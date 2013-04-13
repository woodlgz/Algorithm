
/*
 * Suffix Array Implementation
 *		by woodlgz
 */

#include <iostream>
#include <string.h>
using namespace std;

namespace StringSort{

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
};

namespace DASuffixArray{

	int inline type(char c){
		if(0x30<=c&&c<=0x39)return c-0x30+1;
		else if(0x41<=c&&c<=0x5A)return c-0x41+11;
		else if(0x61<=c&&c<=0x7A)return c-0x61+37;
		else return 0;
	}


	void LSDRadixSort(int I[],int O[],int size){
		unsigned short M[100][100];
		unsigned short SM[100];
		memset(SM,0,sizeof(SM));
		for(int i=0;i<size;i++){
			int k=I[(i<<1)+1];
			M[k][SM[k]++]=i;
		}
		int s=0;
		for(int i=0;i<100;i++){
			for(int j=0;j<SM[i];j++)O[s++]=M[i][j];
		}
		memset(SM,0,sizeof(SM));
		for(int i=0;i<size;i++){
			int k=I[O[i]<<1];
			M[k][SM[k]++]=O[i];
		}
		s=1;
		for(int i=0;i<100;i++){
			for(int j=0;j<SM[i];j++){
				O[M[i][j]]=j>=1&&I[(M[i][j]<<1)+1]==I[(M[i][j-1]<<1)+1]?s-1:s++;
			}
		}
	}
	//str:输入字符串
	//size:字符串的大小
	//Rank:输入的空数组，输出为字符串的Rank数组
	void DA(const char* str,int size,int Rank[]){
		unsigned short SRadix[63];
		memset(SRadix,0,sizeof(SRadix));
		for(int i=0;i<size;i++){
			int k= type(str[i]);
			if(SRadix[k]==0)SRadix[k]++;	
		}
		for(int i=1;i<63;i++)SRadix[i]+=SRadix[i-1];
		for(int i=0;i<size;i++)	Rank[i]=SRadix[type(str[i])];
		int* Rank2=new int[size*2];
		for(int l=1;l<size;l=(l<<1)){
			for(int i=0;i<size;i++){
				Rank2[i<<1]=Rank[i];
				Rank2[(i<<1)+1]=((i+l>=size)?0:Rank[i+l]);
			}
			LSDRadixSort(Rank2,Rank,size);		
		}
		for(int i=0;i<size;i++)Rank[i]--;
		delete[] Rank2;
	}
};
#define MAXSIZE	100
namespace DC3SuffixArray{

	int wa[MAXSIZE];
	int wb[MAXSIZE];
	int ws[MAXSIZE];
	int wv[MAXSIZE];	
	
#define F(x)	((x)/3+((x)%3==1?0:tb))
#define G(x)	((x)<tb?(x)*3+1:((x)-tb)*3+2)
	inline bool equ(int r[],int a,int b){
		return r[a]==r[b]&&r[a+1]==r[b+1]&&r[a+2]==r[b+2];
	}
	inline bool leq(int mod,int r[],int a,int b){
		if(mod==2)return r[a]<r[b]||r[a]==r[b]&&leq(1,r,a+1,b+1);
		else return r[a]<r[b]||r[a]==r[b]&&wv[a+1]<wv[b+1];
	}
	int inline type(char c){
		if(0x30<=c&&c<=0x39)return c-0x30+1;
		else if(0x41<=c&&c<=0x5A)return c-0x41+11;
		else if(0x61<=c&&c<=0x7A)return c-0x61+37;
		else return 0;
	}
	void RadixPass(int r[],int a[],int b[],int n,int m){
		for(int i=0;i<n;i++)wv[i]=r[a[i]];
		for(int i=0;i<m;i++)ws[i]=0;
		for(int i=0;i<n;i++)ws[wv[i]]++;
		for(int i=1;i<m;i++)ws[i]+=ws[i-1];
		for(int i=n-1;i>=0;i--)b[--ws[wv[i]]]=a[i];
		return;
	}

	void dc3(int r[],int sa[],int n,int m){
		int i,j,*rn=r+n,*san=sa+n,tbc=0,tb=(n+1)/3,ta=0,p;
		r[n]=r[n+1]=0;
		for(i=0;i<n;i++)if(i%3!=0)wa[tbc++]=i;
		RadixPass(r+2,wa,wb,tbc,m);//LSD
		RadixPass(r+1,wb,wa,tbc,m);
		RadixPass(r,wa,wb,tbc,m);
		for(i=1,rn[F(wb[0])]=0,p=1;i<tbc;i++)rn[F(wb[i])]=equ(r,wb[i-1],wb[i])?p-1:p++;
		if(p<tbc)dc3(rn,san,tbc,p);
		else for(i=0;i<tbc;i++)san[rn[i]]=i;
		for(i=0;i<tbc;i++)if(san[i]<tb)wb[ta++]=san[i]*3;
		if(n%3==1)wb[ta++]=n-1;
		RadixPass(r,wb,wa,ta,m);
		for(i=0;i<tbc;i++)wv[wb[i]=G(san[i])]=i;
		r[n]=r[n+1]=0;
		for(i=0,j=0,p=0;i<ta&&j<tbc;p++)
			sa[p]=leq(wb[j]%3,r,wa[i],wb[j])?wa[i++]:wb[j++];
		for(;i<ta;p++)sa[p]=wa[i++];
		for(;j<tbc;p++)sa[p]=wb[j++];
		return;
	}
	//str:输入字符串
	//len:输入字符串长度
	//SA:输入字符串的空后缀数组SA,输出时为有效的后缀数组
	//retval:返回后缀数组有效的开始位置，当len为3的倍数时，retval和SA一样，否则retval将为SA的后一段
	int* SuffixArray(const char* str,int len,int SA[]){
		int len_3n=len;
		int r[MAXSIZE];
		for(int i=0;i<len;i++)r[i]=type(str[i]);
		while(len_3n%3)r[len_3n++]=0;
		dc3(r,SA,len_3n,63);
		return SA+len_3n-len;
	}

};

using DASuffixArray::DA;
using namespace DC3SuffixArray;

//h[i]=height[rank[i]],表示字符串中第i位和它排名前一位的字符串的最长公共前缀的长度
//height[i]=lcp(suffix(sa[i]),suffix(sa[i-1])),表示字符串中排名第i的字符串和排名i-1的字符串的最长公共前缀长度
//性质：h[i]>=h[i-1]-1
//证明：
//若h[i-1]<=1，则结论显然成立，因为所有的h[i]都大于等于0
//若h[i-1]>1,表明排位rank[i-1]的字符串和排位rank[i-1]-1的字符串的最长公共前缀长度大于1，设rank[j]=rank[i-1]-1
//有suffix(j)<suffix(i-1)=>suffix(j+1)<suffix(i)=>rank[j+1]<rank[i],再设k=rank[j+1],即有k<=rank[i]-1
//h[i]=lcp(suffix(i),suffix(sa[rank[i]-1]))>=lcp(suffix(i),suffix(j+1))=h[i-1]-1
void GetH(const char* str,int rank[],int sa[],int h[],int height[],int size){
	h[0]=0;
	for(int j=0;rank[0]!=0&&str[j]==str[sa[rank[0]-1]+j];j++,h[0]++);
	for(int i=1,j=0;i<size;i++){
		if(i==sa[0]){
			h[i]=0;
			continue;
		}else {
			for(j=h[i-1]>=1?h[i-1]-1:0;str[i+j]==str[sa[rank[i]-1]+j];j++);
			h[i]=j;
		}
	}
	for(int i=0;i<size;i++)height[rank[i]]=h[i];
}

int main()
{
	const char* str = "abcdcba$abcdcba";
	int len=strlen(str);
	int SA[MAXSIZE];
	int rank[MAXSIZE];
	DA(str,len,rank);
	for(int i=0;i<len;i++)SA[rank[i]]=i;
	int* h=new int[len];
	int* height=new int[len];
	GetH(str,rank,SA,h,height,len);
	for(int i=0;i<len;i++)cout<<h[i]<<" "<<height[i]<<endl;
	return 0;
}
