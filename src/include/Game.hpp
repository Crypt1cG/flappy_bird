#include <vector>
#include <wx/wx.h>
#include "NeuralNet.hpp"

class Pipe
{
public:
	int x, y, gap;
	void draw(wxDC& dc, int width, int height);
	Pipe(int x, int y, int gap);
};

class Bird
{
public:
	int height; // height in pixels
	double velocity; // y velocity (x is constant) units are pxiels/tick
	int size = 50; // edge length size of bird
	int x = 100;
	bool falling, moving;
	
	Bird(int h, double v);
	Bird();
	void jump();
	void draw(wxDC& dc);
};

class Game
{
	const static int num_nns = 32;
	int width, height;
	bool paused;
	std::vector<Pipe> pipes;
	std::vector<NeuralNet> nets;
	std::vector<Bird> birds;
	int tick;
	int generation = 0;
	int score = 0;

public:
	//Bird bird;
	Game(int w, int h);
	void draw(wxDC& dc);
	void update(double tps);
	void OnKeyPressed(wxKeyEvent& evt);
	void reset();
};
