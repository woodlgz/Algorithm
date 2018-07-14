#include <math.h>
#include <map>
#include <iostream>
#include <sys/time.h>

int64_t now() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (int64_t)(tv.tv_sec) * 1000 + (int64_t)(tv.tv_usec) / 1000;
}


void pressureTest(){
	srand(time(NULL));
	std::map<int,int> m;
	int64_t beginTime = now();
	int count = 100000;
	for(int i=0;i<count;i++){
		m.insert(std::make_pair(rand(),1));
	}
	int64_t endTime = now();
	std::cout<<"time collapsed "<<(endTime-beginTime)<<" ms for map "<< count <<" add operation"<<std::endl;
}


int main(){
	pressureTest();
	return 0;
}

