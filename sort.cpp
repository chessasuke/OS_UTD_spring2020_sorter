// *** compile with -lpthread -lrt ***

#include <stdio.h>
#include <sys/types.h> 
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <semaphore.h>

#include "indexes.h"

using namespace std;

sem_t mutex;
sem_t turnstile; 
sem_t turnstile2;
int count;
string mode;
vector<int>list;

//pthread function
void * sorting(void * arg);

void swap(int i, int j);

void print_vector();


int main (int argc, char** argv) {

	string input_file = argv[1];
	mode = argv[2];
	cout <<  "input file: "<<input_file<<endl;
	cout <<  "mode: "<<argv[2]<<endl;
         

	int mutual_ex;
	int bar1;
	int bar2;
	
	int num;
	int size = 0;
	string line;
	
	//semaphore initialization
	ifstream sem_file ("sem_init.txt");
	if (sem_file.is_open())
	{
		if(mode.compare("-o") == 0) cout << "Opening sem_init file..."<<endl;
		getline (sem_file,line); // get number of semaphores
		string M = line; // number of semaphores
		if(mode.compare("-o") == 0) cout << "Number of semaphores: "<< M << endl;
		
		//get number of elements to sort
		getline (sem_file,line);
		mutual_ex = stoi(line);
		//cout << "mutual_ex: "<<mutual_ex<<endl;
		sem_init(&mutex, 0, 1);//mutual_ex); 
		if(mode.compare("-o") == 0) cout << "Semaphore mutex initialized to: "<<mutual_ex<<endl;
		
		getline (sem_file,line);
		bar1 = stoi(line);
		//cout << "bar1: "<<bar1<<endl;
		sem_init(&turnstile, 0, 0);//bar1);
		if(mode.compare("-o") == 0) cout << "Semaphore turnstile initialized to: "<<bar1<<endl;
		
		getline (sem_file,line);
		bar2 = stoi(line);
		//cout << "bar2: "<<bar2<<endl;
		sem_init(&turnstile2, 0, 1);//bar2); 
		if(mode.compare("-o") == 0) cout << "Semaphore turnstile2 initialized to: "<<bar2<<endl;
		
	}
	
	else 
	{
	cout << "Unable to open file";
	exit(0);
	}
	sem_file.close();
  	
  	// get array elements
	ifstream myfile (input_file);
	if (myfile.is_open())
	{
		//get number of elements to sort
		getline (myfile,line);
		size = stoi(line);
		if(mode.compare("-o") == 0) cout << "Number of elements to sort: "<<size<<endl;
		
		while (getline(myfile,line))
		{
		      	num = stoi(line);
			if(num == 0)break;
			//cout << line << endl;
	    		list.push_back(num);
    		}
	}
	
	else 
	{
	cout << "Unable to open file";
	exit(0);
	}
    	myfile.close();
    	
    		
	//unsorted
	cout << "unsorted list: "; print_vector();
	cout << endl;

	//create threads
	//cout << "size: "<<size<<endl;
	int num_threads = size/2;
	pthread_t sort_threads[num_threads];
	//cout << "created "<<num_threads<<" threads"<<endl;

	for(int i = 0; i<num_threads;i++){
		//necessary so tid in thread function doesnt get messed up
		int *arg = (int*)malloc(sizeof(*arg)); //allocating new memory so threads dont try to access the same one
        	if ( arg == NULL ) {
            		cout <<"Error allocating memory"<<endl;
            		exit(EXIT_FAILURE);
        	}
        	*arg = i;
        
		if(pthread_create(&sort_threads[i], NULL, sorting, arg) != 0)
		cout << "Failed to create thread" << endl;
	}
	
	int i = 0;
        while(i < num_threads)
        pthread_join(sort_threads[i++],NULL);
        
        //sorted
        cout << endl<<"sorted list: ";
        print_vector();
        cout <<endl;

     return 0; 
}

void * sorting(void * arg){
	
	int tid = *((int *) arg);	// thread id
	int N = list.size();		// size of array to sort
	int n = list.size()/2; 		// number of threads
	int stages = log_two(N)+1;	// number of stages *will start from 1 not 0*
	int phases = log_two(N)+1;	// number of phases *will start from 1 not 0*
	int st = 1;
	int ph = 1;
	
	//start STAGE
	while(st<stages){
		
		//start PHASE	<CS>
		while(ph<phases){
		
			//barrier code
    			sem_wait(&mutex); //protecting mutex
			count = count + 1;
			if (count == n){
				sem_wait(&turnstile2); // lock second
				sem_post(&turnstile);  // unlock first
			}
			sem_post(&mutex); //protecting mutex

			sem_wait(&turnstile);	// first turnstile
			sem_post(&turnstile);
			//barrier code

			int i = get_s_idx(tid, ph, N);
			int j = get_l_idx(tid, ph, N);
			//cout << "i, j: "<<i<<" "<<j<<endl;
			swap(i, j);
			sem_wait(&mutex); //using mutex to print nicely
			if(mode.compare("-o") == 0) print_vector();
			if(mode.compare("-o") == 0) cout << "Thread "<<tid<<" finished stage "<< st << " phase "<<ph<<endl;
			sem_post(&mutex);
			ph++;//update phase


			//barrier code
    			sem_wait(&mutex); //protecting mutex
			count = count - 1;
			if (count == 0){
			sem_wait(&turnstile);	//	 lock first
			sem_post(&turnstile2);  //	 unlock second
			}
			sem_post(&mutex); //protecting mutex
		
			sem_wait(&turnstile2); 	// second turnstile
			sem_post(&turnstile2);
			//barrier code	
		}
		//END PHASE	<CS>

		ph = 1; //re-start phase

	st++;//update stage

	}//END STAGE
}

void swap(int i, int j){
	
	if(list[i] <= list[j])return;//if do nothing already smaller or equal
	
	else // swap!
	{
		int temp = list[j];
		list[j] = list[i];
		list[i] = temp;
	}	
}


void print_vector()
{
//cout << "*****Printing List*****"<<endl;
	for(int i = 0;i<list.size();i++)
	{
		if(i == 8) cout << endl;
		cout<<list[i] <<"  ";
	}
	cout <<endl;
}





