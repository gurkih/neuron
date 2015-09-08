#include <stdio.h> // for printf
#include <stdlib.h> // for rand
#include <math.h> // for sqrt
#include <iostream> //for std::cout, std::end
#include <fstream> //for writing csvs

using namespace std;

class neuralnet {

public:
int netsize;
int maxtime;
double v_th;
double v_res;
double i_reo;
double tau_0;
double timestep;
double K;
double J0;
double tauM;
double i0;
double C;
int spikes_counter;

double** myneurons;
double** voltages;
double ** spikes;

void init(int mynetsize, int mymaxtime) {
	netsize = mynetsize;
	maxtime = mymaxtime;	

	v_th = 0.99;
	v_res = 0;
	i_reo = 1;
	tau_0 = 10;
	timestep = 0.1;
	K = 22;
	J0 = 1;
	tauM = 0.001;
	i0 = 0.2*tauM*sqrt(K);
	C = -J0 / sqrt(K);
	spikes_counter = 1;

	// this initializes our neural network. its an adjazenzmatrix.
//	double ** myneurons = (double**)(malloc(netsize));
//
//	magic taken from https://www.c-plusplus.net/forum/277504-full
//
	myneurons = new double*[netsize];
	for(int i = 0; i < netsize; i++) {
		myneurons[i]= new double[netsize];
	}
	for (int i = 0; i < netsize; i++) {
		for (int j = 0; j < netsize; j++) {
			double tmp;
			tmp = rand() % 10;
//			cout << tmp << "<- this is my epic rand \n";
			if (tmp > 6) {
				this->myneurons[i][j] = ((double) rand() / (RAND_MAX)); //* 2 - 1; uncomment this if hemmung is wanted/needed
			} else {
				myneurons[i][j] = 0;
			}
		}
	}

	// this initializes the matrix that will hold the voltages at each timestep

	int myysize = (int)(maxtime/timestep);
//	double voltages [netsize][myysize]; <- this is seriously wrong. dont do this!!!
	voltages = new double*[netsize];
	for(int x = 0; x < netsize; x++){
		voltages[x] = new double[myysize];
	}
	for (int x = 0; x < netsize; x++) {
		for (int y = 0; y < myysize; y++) {
			voltages[x][y]= 0;
		}
	}
	for (int x = 0; x < netsize; x++) {
		int myrand = rand() % 10;
		int currentindex = (int)((double)myrand/timestep);
		voltages[x][currentindex] = ((double) rand() / (RAND_MAX));
	}

	// this initializes the matrix that will hold the spikes
	int spikeysize = myysize * netsize;
//	double spikes [spikeysize][2];
	spikes = new double*[spikeysize];
	for (int a = 0; a < spikeysize; a++) {
		spikes[a] = new double[2];
	}
	for (int a = 0; a < spikeysize; a++) {
		for (int b = 0; b < 2; b++) {
			spikes[a][b] = 0;
		}
	}
}

void calculate() {
	int maxtimeindex = (int) ((double)maxtime/(double)timestep);
	int zerovoltage = 0;
	int nonzerovoltage= 0;
//	for (double time = 2*timestep; time +=timestep; time < maxtime-2*timestep) {	
	for (int timeindex=1; timeindex < maxtimeindex; timeindex++) {
		//long timeindex = (long) (double)time/(double)timestep;
		//cout << timeindex << endl;
		double time = timeindex/timestep;
		for (int i = 1; i < netsize; i++) {
			if (voltages[i][timeindex-1] == 0) {
				zerovoltage++;
			} else {
				nonzerovoltage++;
			}
			if (this->voltages[i][timeindex-1] >= v_th) {
				//cout << "i am spiking! \n";
				spikes[spikes_counter][1] = time;
				spikes[spikes_counter][2] = i;
				spikes_counter = spikes_counter + 1;
				voltages[i][timeindex] = v_res;
				for (int y = 1; y < netsize; y++) {
					if(myneurons[i][y] != 0) {
						voltages[y][timeindex] += myneurons[i][y] * C;
					}
				}
			} else {
				double add = 0.8*(i_reo - (voltages[i][timeindex-1]+0) * tau_0*0.5);
			//	cout << add << " " << i_reo << " " << voltages[i][timeindex] << " " << tau_0 <<  "\n";
				voltages[i][timeindex] += add; // missing: * timestep	
			}
		}
	}
	printf("i had %d zero voltages, and %d non-zero voltages. i spiked %d times. \n",zerovoltage,nonzerovoltage, spikes_counter);
}

void writecsv() {
	ofstream myneuronsfile;
	ofstream myvoltagesfile;
	ofstream myspikesfile;
	myneuronsfile.open("myneurons.csv");
	for (int i = 0; i < netsize; i++) {
		for(int j = 0; j < netsize; j++) {
			myneuronsfile << myneurons[i][j] << ", ";
		}
		myneuronsfile << "\n";
	}
	myneuronsfile.close();

	int myysize = (int)(maxtime/timestep);
	myvoltagesfile.open("myvoltages.csv");
	for(int i = 0; i < myysize; i++) {
		for (int j = 0; j < netsize; j++) {
			myvoltagesfile << voltages[j][i] << ", ";
		}
		myvoltagesfile << "\n";
	}
	myvoltagesfile.close();

//	myspikesfile.open("myspikesfile.csv");
}
};
