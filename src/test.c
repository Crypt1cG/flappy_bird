#include "include/nn.h"
#include <stdio.h>
#include <stdlib.h>

void eval(double* input, struct NeuralNet net, double* output)
{
	double* nums = input;
	double* out;
	for (int layer_num = 0; layer_num < NUM_LAYERS; layer_num++)
	{
		double** layer = net.weights[layer_num];
		double* biases = net.biases[layer_num];
		int num_neurons = sizes[layer_num + 1]; // add 1 because the first num is input size
		out = (double*) malloc(sizeof(double) * num_neurons);
		for (int i = 0; i < num_neurons; i++)
		{
			int num_weights = sizes[i]; // size of previous layer
			double sum = 0;
			for (int j = 0; j < num_weights; j++)
			{
				sum += layer[i][j] * nums[j];
			}
			sum += biases[i];
			// ReLU (could use sigmoid)
			out[i] = sum > 0 ? sum : 0;
		}
		if (layer_num != 0) free(nums);
		nums = out;
	}
	output = out;
}

int main()
{
	struct NeuralNet nn;
	double input[5] = {1.0, 10.0, 0.0, -5.0, 2.0};
	double* output = (double*)malloc(sizeof(double));

	for (int i = 0; i < NUM_LAYERS; i++)
	{
		nn.biases[i] = (double*)calloc(sizes[i + 1], sizeof(double));
		for (int k = 0; k < sizes[i + 1]; k++) // loop through all neurons
		{
			double* tmp = (double*)malloc(sizes[i] * sizeof(double));
			nn.weights[i][k] = tmp;
			for (int j = 0; j < sizes[i]; j++)
				nn.weights[i][k][j] = rand() % 50 / 10.0 - 2.5;
		}
	}
	eval(input, nn, output);
	printf("%f", output[0]);
}
