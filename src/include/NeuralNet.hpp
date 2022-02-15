class NeuralNet
{
public:
	// CHANGE THESE 2 LINES TO CHANGE NN SHAPE
	static const int num_hidden_layers = 2;
	static const inline int sizes[] = {5, 3, 1};

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
};
