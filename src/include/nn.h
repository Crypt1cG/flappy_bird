#define NUM_LAYERS 3 // number of layers excluding input

struct NeuralNet
{
	double* weights[NUM_LAYERS];
	double* biases;
};

const int sizes[] = {5, 5, 1, 1};

static int offset(int x, int y, int layer) {return x * sizes[layer - 1] + y;}
