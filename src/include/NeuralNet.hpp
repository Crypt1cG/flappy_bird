#include <chrono>
#include <cmath>
#include <random>
#include <string>

class NeuralNet
{
public:
	// CHANGE THESE 2 LINES TO CHANGE NN SHAPE
	static const int num_hidden_layers = 3;
	static const inline int sizes[] = {3, 4, 4, 1};

	static std::default_random_engine generator;

	double* weights[num_hidden_layers];
	double* biases[num_hidden_layers];

	/**
	 * returns a particular weight
	 */
	double& at(int layer, int neuron, int input)
	{
		return weights[layer][neuron * sizes[layer] + input];
	}

	/**
	 * returns the bias for a nueron on a certain layer
	 */
	double& at(int layer, int neuron)
	{
		return biases[layer][neuron];
	}

	NeuralNet();

	void print();

	double* eval(double* input);

	NeuralNet create_random_child(double mutation_chance, double std_deviation);

	NeuralNet create_child(NeuralNet old1, NeuralNet old2);

	void write(std::string file_path);

	static NeuralNet read(std::string file_path);
};
