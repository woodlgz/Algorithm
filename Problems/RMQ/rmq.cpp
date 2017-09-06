#include <iostream>
#include <cmath>
using namespace std;

#define MAXN	50
#define MAXLOG	10

namespace RMQSolution{
	
	template<typename T>
	inline T max(const T& a, const T& b) {
		return a > b ? a : b;
	}

	//Range[i][j]表示Range(i,i+2^j-1)的最大值
	//Range[i][j]=max(Range[i][j-1],Range[i+2^(j-1)][j-1])
	void RMQ(int Array[],int size,int Range[][MAXN]){
		for(int i=0;i<size;i++)Range[i][0]=Array[i];
		for(int j=1;(1<<j)<=size;j++)
		{
			for(int i=0;i+(1<<j)<=size;i++)
			{
				Range[i][j]=max(Range[i][j-1],Range[i+(1<<(j-1))][j-1]);
			}
		}
	}
	//(e-b)/2<=2^k<=e-b+1=>k=log2(e-b+1)
	//range(b,e)=max(Range[b][k],Range[e-2^k+1][k])
	int Query(int Range[][MAXN],int b,int e){
		int k;
		for(k=0;(1<<k)<=(e-b+1);k++);
		k--;
		return max(Range[b][k],Range[e-(1<<k)+1][k]);	
	}
	
	//Range[x][y][i][j] 表示在[x,x+2^i-1]和[y,y+2^j-1]的范围内的max值
	//Range[x][y][i][j]=max(Range[x][y][i-1][j-1],
    //                      Range[x][y+2^(j-1)][i-1][j-1],
    //                      Range[x+2^(i-1)][y][i-1][j-1],
    //                      Range[x+2^(i-1)][y+2^(j-1)][i-1][j-1]);
    //Range[x][y][0][j]=max(Range[x][y][0][j-1],Range[x][y+2^(j-1)][0][j-1])
    //Range[x][y][i][0]=max(Range[x][y][i-1][0],Range[x+2^(i-1)][i-1][0])
	//Range[x][y][0][0]=Array[x][y]
	template<int dimN,int dimM>
	void RMQ2D(int Array[][dimM],int Range[][MAXN][MAXLOG][MAXLOG]){
		for(int x=0;x<dimN;x++)
			for(int y=0;y<dimM;y++)
				Range[x][y][0][0]=Array[x][y];
		for(int y=0;y<dimM;y++)
			for(int i=1;(1<<i)<=dimN;i++)
				for(int x=0;x+(1<<i)<=dimN;x++)
					Range[x][y][i][0]=max(Range[x][y][i-1][0],Range[x+(1<<(i-1))][y][i-1][0]);
		for(int x=0;x<dimN;x++)
			for(int j=1;(1<<j)<=dimM;j++)
				for(int y=0;y+(1<<j)<=dimM;y++)
					Range[x][y][0][j]=max(Range[x][y][0][j-1],Range[x][y+(1<<(j-1))][0][j-1]);
		for(int j=1;(1<<j)<=dimM;j++)
			for(int i=1;(1<<i)<=dimN;i++)
				for(int y=0;y+(1<<j)<=dimM;y++)
					for(int x=0;x+(1<<i)<=dimN;x++)
					{
						Range[x][y][i][j]=max(Range[x][y][i-1][j-1],Range[x+(1<<(i-1))][y+(1<<(j-1))][i-1][j-1]);
						Range[x][y][i][j]=max(Range[x][y][i][j],Range[x][y+(1<<(j-1))][i-1][j-1]);
						Range[x][y][i][j]=max(Range[x][y][i][j],Range[x+(1<<(i-1))][y][i-1][j-1]);
					}
					
	}
	int Query2D(int Range[][MAXN][MAXLOG][MAXLOG],int bx,int by,int ex,int ey){
		int k1,k2;
		for(k1=0;(1<<k1)<=(ex-bx+1);k1++);k1--;
		for(k2=0;(1<<k2)<=(ey-by+1);k2++);k2--;
		int M = max(Range[bx][by][k1][k2],Range[ex-(1<<k1)+1][by-(1<<k2)+1][k1][k2]);
		M=max(Range[bx][ey-(1<<k2)+1][k1][k2],M);
		M=max(Range[ex-(1<<k1)+1][by][k1][k2],M);
		return M;
	}
};

using namespace RMQSolution;

int main(){
	int seq[10][10]={{1,	2,	3,	4,	5,	6,	7,	8,	9,	10},
					 {10,	9,	8,	7,	6,	5,	4,	3,	2,	1},
					 {1,	6,	4,	23,	8,	41,	25,	41,	3,	4},
					 {5,	4,	85,	1,	2,	1,	5,	1,	32,	2},
					 {4,	52,	1,	5,	5,	2,	3,	1,	5,	5},
					 {0,	0,	0,	0,	0,	0,	0,	0,	0,	0},
					 {1,	63,	1,	3,	14,	5,	10,	0,	0,	1},
					 {5,	6,	1,	9,	1,	0,	1,	6,	1,	1},
					 {1,	1,	1,	1,	1,	1,	1,	1,	1,	1},
					 {1,	1,	1,	1,	1,	1,	1,	1,	1,	1}};
	int Range[MAXN][MAXN][MAXLOG][MAXLOG];
	memset(Range,0,sizeof(Range));
	RMQ2D<10,10>(seq,Range);
	cout<<Query2D(Range,4,0,7,6)<<endl;
	int seq_1d[]={1,3,4,5,6,7,8,1,2,11,13,12};
	int Range1d[MAXN][MAXN];
	RMQ(seq_1d,sizeof(seq_1d)/sizeof(int),Range1d);
	cout<<Query(Range1d,2,10)<<endl;
	return 0;
}
