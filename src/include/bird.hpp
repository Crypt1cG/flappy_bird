#include <wx/wx.h>

class Bird
{
public:
	int height; // height in pixels
	double velocity; // y velocity (x is constant) units are pxiels/tick
	
	Bird(int h, double v);
	Bird();
	void draw(wxDC& dc);
};
