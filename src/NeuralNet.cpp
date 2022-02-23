#include <cmath>
#include <cfloat>
#include <iostream>
#include <random>
#include <fstream>
#include "include/NeuralNet.hpp"

NeuralNet::NeuralNet()
{
	for (int i = 0; i < num_hidden_layers; i++)
	{
		// sizes[i] is previous layer size, i.e. input size for current layer
		weights[i] = new double[sizes[i] * sizes[i + 1]];
		biases[i] = new double[sizes[i + 1]];
	}

	std::default_random_engine generator = std::default_random_engine();
	std::normal_distribution<double> distribution(0.0, 1.0);

	for (int i = 0; i < num_hidden_layers; i++)
	{
		for (int j = 0; j < sizes[i + 1]; j++) // loop through all neurons
		{
			for (int k = 0; k < sizes[i]; k++)
			{
				at(i, j, k) = distribution(generator);
			}
			at(i, j) = 0;
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
			if (i < num_hidden_layers - 1)
				sum = std::max(0.0, sum);
			else 
			{
				sum = 1 / (1 + exp(-sum));
			}
			output[j] = sum;
		}
		if (i != 0)
			delete[] input;
		input = output;
	}
	return output;
}

NeuralNet NeuralNet::create_random_child(double mutation_chance, double std_deviation)
{
	// loop through all weights and biases and if rand() / RAND_MAX <= mutation chance
	// change the weight/bias

	NeuralNet new_nn = NeuralNet();

	std::default_random_engine generator = std::default_random_engine();
	std::normal_distribution<double> distribution(0.0, std_deviation);

	for (int i = 0; i < num_hidden_layers; i++)
	{
		for (int j = 0; j < sizes[i + 1]; j++) // j is neuron index
		{
			// do the weights
			for (int k = 0; k < sizes[i]; k++) // k is weight index
			{
				if ((double)rand() / RAND_MAX <= mutation_chance)
					new_nn.at(i, j, k) = this->at(i, j, k) + distribution(generator) * this->at(i, j, k);
				else
					new_nn.at(i, j, k) = this->at(i, j, k);
			}

			// do the bias
			if ((double)rand() / RAND_MAX <= mutation_chance)
			{
				if (at(i, j) == 0)
					new_nn.at(i, j) = distribution(generator);
				else
					new_nn.at(i, j) = this->at(i, j) + distribution(generator) * this->at(i, j);
			}
			else
				new_nn.at(i, j) = this->at(i, j);
		}
	}
	return new_nn;
}

void NeuralNet::write(std::string file_path)
{
	std::ofstream file(file_path);
	file.precision(DBL_DIG + 1);
	// blank line between layers, neuron on each line, comma between weights "|" between weights and bias
	for (int i = 0; i < num_hidden_layers; i++)
	{
		for (int j = 0; j < sizes[i + 1]; j++) // j is neuron index
		{
			file << at(i, j, 0);
			for (int k = 1; k < sizes[i]; k++) // k is weight index
			{
				file << ",";
				file << at(i, j, k);
			}
			file << "|";
			file << at(i, j);
			file << "\n";
		}
		file << "\n";
	}		
}

NeuralNet NeuralNet::read(std::string file_path)
{
	std::ifstream file(file_path);
	NeuralNet nn;
	if (!file.is_open())
	{
		std::cout << "Could not open file!!" << std::endl;
	}
	else
	{
		std::string line;
		int neuron_num = 0;
		int layer_num = 0;
		while (std::getline(file, line))
		{
			if (line == "")
			{
				layer_num++;
				neuron_num = 0;
			}
			else
			{
				size_t pos = line.find('|');

				std::string weights;
				weights = line.substr(0, pos);

				int bias = std::stod(line.substr(pos + 1));

				// break up the weights (they are separated by commas)
				size_t comma_pos = weights.find(',');
				std::vector<double> weights_vec;
				while (comma_pos != std::string::npos)
				{
					weights_vec.push_back(std::stod(weights.substr(0, comma_pos)));
					weights.erase(0, comma_pos + 1);
					comma_pos = weights.find(',');
				}
				weights_vec.push_back(std::stod(weights));

				nn.at(layer_num, neuron_num) = bias;
				for (int i = 0; i < weights_vec.size(); i++)
				{
					nn.at(layer_num, neuron_num, i) = weights_vec[i];
				}
				neuron_num++;
			}
		}
	}
	nn.print();
	return nn;
}

// int main()
// {
// 	NeuralNet nn;
// 	nn.print();
// 	NeuralNet nn2 = nn.create_random_child(0.5, 0.5);
// 	nn2.print();
// 	double* input = new double[4]{1, 0, 0, 0};
// 	double* result = nn.eval(input);
// 	std::cout << result[0] << std::endl;
// }
