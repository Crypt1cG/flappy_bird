#include <vector>
#include <wx/wx.h>


class Pipe
{
public:
	int x, y, gap;
	void draw(wxDC& dc);
	Pipe(int x, int y, int gap);
};

class Bird
{
public:
	int height; // height in pixels
	double velocity; // y velocity (x is constant) units are pxiels/tick
	int size = 50; // edge length size of bird
	bool falling;
	
	Bird(int h, double v);
	Bird();
	void draw(wxDC& dc);
};

class Game
{
	int width, height;
	bool paused;
	std::vector<Pipe> pipes;

public:
	Bird bird;
	Game(int w, int h);
	void draw(wxDC& dc);
	void update(double tps);
	void OnKeyPressed(wxKeyEvent& evt);
};
