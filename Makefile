serial: neuralnet.cpp
	icc neuralnet.cpp -o neuron
omp: neuralnet.cpp
	icc -openmp neuralnet.cpp -o neuron
mic: neuralnet.cpp
	icc -openmp -offload-attribute-target=mic neuralnet.cpp -o neuron
clean:
	rm neuron
