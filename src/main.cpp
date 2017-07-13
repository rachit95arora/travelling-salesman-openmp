#include "cx.h"
#include "gx.h"
#include "erx.h"
#include "pmx.h"
#define MAX_ITERS 10000
#define PMX 0
#define BESTS 10
using namespace std;
int myrandom (int i) { return std::rand()%i;}

int main(int argc, char *argv[])
{
	if(argc!=4){
		cout<<"Invalid number of arguments, please supply : inputfile outputfile timeLimitInSec\n";
		return 0;
	}
	ifstream inputFile;
	double start = omp_get_wtime(),limit =(double) strtof(argv[3],NULL);  //Time keeping
	inputFile.open(argv[1]);
	string s; 
	int N;
	int GREEDY,MUTATE,NUMT;
	getline(inputFile,s);
	inputFile>>s>>N;
	cout<<N<<endl;
	inputFile>>s;
	// Start reading coordinates.
	float coordinates[N][2];
	int i;
	vector<string> names(N,"");
	for(i=0;i<N;i++)
	{
		inputFile>>names[i]>>coordinates[i][0]>>coordinates[i][1];
		//Setting up coordinate matrix
	}
	inputFile.close();
	//Creating a distance matrix for cities
	vector< vector<float> > dist(N, vector<float> (N,0.0));
	for(i=0 ; i<N;i++)
	{
		dist[i][i] = 0.0;
		for(int j = i+1;j<N;j++)
		{
			dist[i][j] = dist[j][i] = (float)sqrt((double)(((coordinates[j][0] - coordinates[i][0])*(coordinates[j][0] - coordinates[i][0]))+((coordinates[j][1] - coordinates[i][1])*(coordinates[j][1] - coordinates[i][1]))));
		}
	}

	srand((int)(omp_get_wtime()));
	int N1 = ((10*N*N)>10000)?10000:(10*N*N);
	vector< vector<int> > chromos(N1, vector<int> (N,0));  //Complete set of chromosomes at any given point
	vector< vector<int> > bests(BESTS, vector<int> (N,0)); //best set of chromosomes at any given point
	int best = 0;
	vector<float> fitness(N1,2000000000.0);                // Stoes chromosome tour distance
	float maxfitness = 2000000000.0;
	omp_lock_t locks[N1],lock4bests;
	omp_init_lock(&lock4bests);							   //Initializing lock for access to bests vector array
	//Choosing an initial population.
	for(i = 0;i<N;i++)
	{
		chromos[0][i] = i;
	}
	#pragma omp parallel shared(fitness,maxfitness,best,chromos,locks,lock4bests,N,N1,dist,bests) private(GREEDY,MUTATE)
	{
	NUMT = omp_get_num_threads();
	cout<<NUMT<<endl;
	#pragma omp for
	for(int il = 0 ; il<N1;il++)
	{
		omp_init_lock(locks+il);
		chromos[il] = chromos[0];
		random_shuffle(chromos[il].begin(), chromos[il].end(),myrandom); //Shuffling to randomise initial population
		float x=0;
		for(int j = 0 ; j<N;j++)
		{
			x+=dist[chromos[il][j]][chromos[il][(j+1)%N]];
		}
		fitness[il] = x;
		if(maxfitness>fitness[il])
		{
			omp_set_lock(&lock4bests);  //Accessing best array safely
			maxfitness = fitness[il];
			best = (++best)%BESTS;
			bests[best] = chromos[il];
			omp_unset_lock(&lock4bests);
		}
		
	}
	
	int iterations;  //iterate to mature chromosomes and improve fitness
	for(iterations = 0 ; iterations < MAX_ITERS; iterations++)
	{
		if(omp_get_wtime()-start+1 >limit){
			break;                                                    //TIMEOUT
		}
		GREEDY = (int)(100- (((float)iterations)/MAX_ITERS)*10);      //Reevaluate GREEDY, MUTATE percentages
		MUTATE = (iterations<((4*MAX_ITERS)/5))?20:40;
		#pragma omp master                                            //Print max fitness at current stage
		{
		if((iterations%1000==999)||(iterations%1000==0))cout<<iterations<<" ~ "<<GREEDY<<" ~ "<<MUTATE<<" ~ "<<maxfitness<<endl;
		}
		#pragma omp for nowait
		for(int nb = 0;nb<BESTS;nb++)    							//Seeding for better fitness
		{
			int te = rand()%N1;
			omp_set_lock(locks+te);
			chromos[te] = bests[nb];
			omp_unset_lock(locks+te);	
		}
		#pragma omp for schedule(static,20)							//Core loop parallelised over threads
		for(int crossed = 0; crossed<=((3*N1)/4);crossed+=2)
		{
			int first=N1,second=N1;
			bool gotFirst = false,gotSecond= false;                //Roulette wheel selection
			while(!gotFirst)
			{
				first = rand()%N1;
				if((((rand()%1001)/(float)(1000)) < (maxfitness/fitness[first])))
				{
					gotFirst = true;
				}
			}
			while(!gotSecond)
			{
				second = rand()%N1;
				if((((rand()%1001)/(float)(1000)) < (maxfitness/fitness[second])) &&(second!=first))
				{
					gotSecond = true;
				}
			}
			if(first<second)										//Locking selected chromosome pair
			{
				omp_set_lock(locks+first);
				omp_set_lock(locks+second);
			}
			else
			{
				omp_set_lock(locks+second);
				omp_set_lock(locks+first);
			}
			//Cross a pair
			int select = (rand()%101),index,temp;					//Stochastically choosing from various crossover techniques
			if(select<GREEDY)gx(chromos[first],chromos[second],dist);
			else if(select<GREEDY+PMX)pmx(chromos[first],chromos[second]);
			else erx(chromos[first],chromos[second]);
			//Mutate the chromos
			select = (rand()%101);									//Mutating crossover results selectively
			if(select<=MUTATE)
			{
				index = rand()%(N*N);
				temp = chromos[first][index/N];
				chromos[first][index/N] = chromos[first][index%N];
				chromos[first][index%N] = temp;
			}
			select = (rand()%101);
			if(select<=MUTATE)
			{
				index = rand()%(N*N);
				temp = chromos[second][index/N];
				chromos[second][index/N] = chromos[second][index%N];
				chromos[second][index%N] = temp;
			}
			//Update stuff                                                         //Reevaluate fitness of crossed over pairs
			float x=0;
			for(int j = 0 ; j<N;j++)
			{
				x+=dist[chromos[first][j]][chromos[first][(j+1)%N]];
			}
			fitness[first] = x;
			
			x=0;
			for(int j = 0 ; j<N;j++)
			{
				x+=dist[chromos[second][j]][chromos[second][(j+1)%N]];
			}
			fitness[second] = x;


			if(maxfitness>fitness[first])
			{
				omp_set_lock(&lock4bests);
				best = (++best)%BESTS;
				bests[best] = chromos[first];
				maxfitness = fitness[first];
				omp_unset_lock(&lock4bests);
			}
			if(maxfitness>fitness[second])
			{
				omp_set_lock(&lock4bests);
				best = (++best)%BESTS;
				bests[best] = chromos[second];
				maxfitness = fitness[second];
				omp_unset_lock(&lock4bests);
			}

			if(first<second)							//Releasing lock from chromosome pair
			{
				omp_unset_lock(locks+second);
				omp_unset_lock(locks+first);
			}
			else
			{
				omp_unset_lock(locks+first);
				omp_unset_lock(locks+second);
			}
		}
	}
	
	//Destroy all chromosome locks
	#pragma omp for
	for(int bi = 0; bi<N1; bi++)
		omp_destroy_lock(locks+bi);

	} //Threads end here

	float x=0;
	for(int j = 0 ; j<N;j++)
	{
		x+=dist[bests[best][j]][bests[best][(j+1)%N]];
	}
	maxfitness = x;
	for(int k = 0; k<N;k++)
	{
		cout<<bests[best][k]<<" ";
	}
	cout<<endl;
	//Printing output tour file
	ofstream outputFile;
	outputFile.open(argv[2]);
	outputFile<<"DIMENSION : "<<N<<endl;
	outputFile<<"TOUR_LENGTH : "<<maxfitness<<endl;
	outputFile<<"TOUR_SECTION\n";
	for(int k = 0; k<N;k++)
	{
		outputFile<<names[bests[best][k]]<<" ";
	}
	outputFile<<endl;
	outputFile<<"-1\nEOF";
	outputFile.close();
	//Conclusion
	double end = omp_get_wtime();
	cout<<" FINAL "<<(maxfitness)<<" TIME : "<<(end-start)<<endl;
	return 0;
}
