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
