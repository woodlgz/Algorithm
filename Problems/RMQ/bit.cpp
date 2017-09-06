#include <iostream>
#include <stdlib.h>

using namespace std;

class BIT{
private :
	BIT(){}
public:
	/**
	* 初始化查询统计信息数据结构
	*/
	static void init(){
		TreeSize = 0;
		memset(Tree,0,sizeof(Tree)/sizeof(int));
	}

	/**
	* 清理查询统计信息数据结构
	*/
	static void clean(){
		memset(Tree,0,sizeof(Tree)/sizeof(int));
		TreeSize = 0;
	}

	/**
	* 根据原区间统计信息建立查询区间信息
	*/
	static void build(int array[],int size){
		TreeSize = size;
		for(int i = 1;i<=size;i++){
			update(i,array[i]);
		}
		MaxBit = 0;
		while(TreeSize>0){
			TreeSize >>=1;
			MaxBit++;
		}
		MaxBit = MaxBit>0?MaxBit-1:MaxBit;
		TreeSize = size;
	}

	/**
	* 更新原区间i的值
	*/
	static void update(int i,int val){
		while(i<=TreeSize){
			Tree[i]+=val;
			i+= (i&-i);
		}
	}

	/**
	* 查询原区间信息中[1,end]的总的统计值
	*/
	static int query(int end){
		end = end<TreeSize?end:TreeSize;
		int count = 0;
		while(end>0){
			count+= Tree[end];
			end-=(end&-end);
		}
		return count;
	}

	/**
	* 查询原区间信息中[begin,end]的总的统计值
	*/
	static int query(int begin,int end){
		begin = begin<1?1:begin;
		end = end<TreeSize?end:TreeSize;
		return query(end)-query(begin-1);
	}

	/**
	* 查询原区间统计信息中第i个位置的值
	*/
	static int querySingle(int i){
		if(i<=0||i>TreeSize)return 0;
		int count = Tree[i];
		int z = i - (i&-i);
		i--;
		while(i!=z){
			count -= Tree[i];
			i-=(i&-i);
		}
		return count;
	}

	/**
	* 将原区间统计信息中[begin,end]部分的统计信息乘以p
	*/
	static void scale(int begin,int end,float p){
		// f[i]*p = f[i]+(p-1)*f[i]
		for(int i=begin;i<=end;i++){
			update(i,(p-1.0)*querySingle(i));
		}
	}

	/**
	* 查找原区间统计信息中从零开始累计值为val的区间位置
	*/
	static int find(int val){
		int t = 1<<MaxBit;
		int target = 0;	
		while(t>0&&target<TreeSize){
			int tidx = target+t;
			if(val>=Tree[tidx]){
				val -= Tree[tidx];
				target = tidx;
			}
			t>>=1;
		}
		if(val==0)return target;
		else return  -1;
	}
private:
	/** 树状数组的最大限定大小 */
	static const int MAX_SIZE = 100;
	/** 查询中树状数组的实际大小*/
	static int TreeSize;
	/** floor(Log(TreeSize))*/
	static int MaxBit;
	/** 树状数组，索引从1开始*/
	static int Tree[MAX_SIZE+1];
};

int BIT::TreeSize;
int BIT::MaxBit;
int BIT::Tree[BIT::MAX_SIZE+1];

int main(int argc,char** argv){
	int array[] = {-1,1,3,4,5,6,4,3,2,6,8,9,2}; //array[0]无效
	BIT::init();
	BIT::build(array,sizeof(array)/sizeof(int)-1);
	BIT::scale(4,5,2.0);
	cout<<BIT::query(3,5)<<endl;
	system("pause");
	return 0;
}
