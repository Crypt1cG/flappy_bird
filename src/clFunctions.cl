#include "include/nn.h"

__kernel void eval(__global double* input, __global NeuralNet net, __global double* output)
{
	double* nums = input;
	double* out;
	for (int layer_num = 0; layer_num < NUM_LAYERS; layer_num++)
	{
		double** layer = net.weights[layer_num];
		double* biases = net.biases[layer_num];
		int num_neurons = sizeof(layer) / sizeof(layer[0]);
		out = (double*)malloc(sizeof(double) * num_neurons);
		for (int i = 0; i < num_neurons; i++)
		{
			int num_weights = sizeof(layer[i] / sizeof(double));
			double sum = 0;
			for (int j = 0; k < num_weights; j++)
			{
				sum += layer[i][j] * nums[j];
			}
			sum += biases[i];
			// ReLU (could use sigmoid)
			out[i] = max(0, sum);
		}
		if (layer_num != 0) free(nums);
		nums = out;
	}
	output = out;
}

__kernel void vMul(__global double* input, __global double* weights)
{
	int id = get_global_id(0);
	input[id] *= weights[id];
}

__kernel void vAdd(__global double* input, __global double* sum)
{
	int id = get_global_id(0);
	*sum += input[id];
}

__kernel void sigmoid(__global double* input)
{
	int id = get_global_id(0);
	double result = 1 / (1 + exp(-input[id]));
	input[id] = result;
}
