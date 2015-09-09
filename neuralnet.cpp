#include "neuralnet.h"

int main (int argc, char *argv[]) {

	neuralnet myNeuralnet;
	cout << "initializing ..." << endl;

//#pragma offload target (mic)
	myNeuralnet.init(20,100); // was: 10,100
	cout << "done initializing." << endl;
	cout << myNeuralnet.myneurons[0][0] << " " << myNeuralnet.spikes[0][0] << " " << myNeuralnet.voltages[0][0] <<endl;
	cout << "calculating ..." << endl;
	myNeuralnet.calculate();
	cout << myNeuralnet.voltages[1][100] << endl;
	cout << "done calculating" << endl;
	myNeuralnet.writecsv();
}
