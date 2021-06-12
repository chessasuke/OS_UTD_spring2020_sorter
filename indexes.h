#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

// to compare (i, j) given phase and thread id

// N array size, T # of threads
// ph phase
// ps partition size 		-  N/(2^(ph-1))
// pn partition #    		-  Floor(tid/ts)
// ts threads per partition 	-  T/(2^(ph-1))
// tid thread id

// i = tid % ts + pn * ps
// j = (ps - 1) - (tid % ts) + pn * ps

// return log(n) base 2
double log_two(double n){
	return log(n)/log(2);
}

void get_indexes(int tid, int ph, int N);
int get_s_idx(int tid, int ph, int N);
int get_l_idx(int tid, int ph, int N);
int get_tpp(int ph, int N);
int get_pn(int tid, int ph, int N);
int get_ps(int ph, int N);


// get indexes to be compared given thread id and phase
// thread ids start at 0, phases at 1
void get_indexes(int tid, int ph, int N){

	// smaller index
	// tid % threads per partition + partition number of a specific thread * partition size
	cout << ( (tid % get_tpp(ph, N)) + get_pn(tid, ph, N) * get_ps(ph, N) )<<endl;
	
	// larger index
	// (partition size-1) tid % threads per partition + partition number of a specific thread * partition size
	cout << ( ((get_ps(ph, N) - 1) - (tid % get_tpp(ph, N))) + get_pn(tid, ph, N) * get_ps(ph, N) )<<endl;
}

// return smaller index
int get_s_idx(int tid, int ph, int N){

	int i = ( (tid % get_tpp(ph, N)) + get_pn(tid, ph, N) * get_ps(ph, N) );
}

// return larger index
int get_l_idx(int tid, int ph, int N){
	
	int j = ( ((get_ps(ph, N) - 1) - (tid % get_tpp(ph, N))) + get_pn(tid, ph, N) * get_ps(ph, N) );
}

// get partition size: N/(2^phase) *phases start at 1, that's why ph-1*
int get_ps(int ph, int N){
	
	return (int)(N/(pow(2, (ph-1))));
}

// get partition number of a specific thread: floor(tid / number of threads per partition)
int get_pn(int tid, int ph, int N){	

	return floor(tid/(get_tpp(ph, N)));
}

//get threads per partition: T/(2^phase) *phases start at 1, that's why ph-1*
int get_tpp(int ph, int N){
	
	// number of threads T = N/2
	return (int)((N/2)/(pow(2, (ph-1))));
}


