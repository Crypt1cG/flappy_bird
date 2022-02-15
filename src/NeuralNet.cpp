#include <iostream>
#include "include/NeuralNet.hpp"

NeuralNet::NeuralNet()
{
	for (int i = 0; i < num_hidden_layers; i++)
	{
		// sizes[i] is previous layer size, i.e. input size for current layer
		weights[i] = new double[sizes[i] * sizes[i + 1]];
		biases[i] = new double[sizes[i]];
	}

	for (int i = 0; i < num_hidden_layers; i++)
	{
		for (int j = 0; j < sizes[i + 1]; j++) // loop through all neurons
		{
			for (int k = 0; k < sizes[i]; k++)
			{
				at(i, j, k) = rand() % 100 / 10.0 - 5.0;
			}
			at(i, j) = rand() % 100 / 10.0 - 5.0;
		}
	}
}

void NeuralNet::print()
{
	for (int i = 0; i < num_hidden_layers; i++)
	{
		std::cout << "Layer " << i + 1 << ":" << std::endl;
		for (int j = 0; j < sizes[i + 1]; j++)
		{
			std::cout << "[" << at(i, j, 0);
			for (int k = 1; k < sizes[i]; k++)
			{
				std::cout << ",\t" << at(i, j, k);
			}
			std::cout << "]\t";
			std::cout << "[" << at(i, j) << "]\n";
		}
	}
}

double* NeuralNet::eval(double* input)
{
	double* output;
	for (int i = 0; i < num_hidden_layers; i++)
	{
		output = new double[sizes[i + 1]];
		for (int j = 0; j < sizes[i + 1]; j++)
		{
			double sum = 0;
			for (int k = 0; k < sizes[i]; k++)
			{
				sum += at(i, j, k) * input[k];
			}
			sum += at(i, j); // add bias
			// ReLU, could use sigmoid
			sum = std::max(0.0, sum);
			output[j] = sum;
		}
		delete[] input;
		input = output;
	}
	return output;
}

int main()
{
	NeuralNet nn;
	nn.print();
	double* input = new double[5]{1, 0, 0, 0, 0};
	double* result = nn.eval(input);
	std::cout << result[0] << std::endl;
}
