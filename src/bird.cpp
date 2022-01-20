#include "include/bird.hpp"

Bird::Bird(int h, double v)
{
	height = h;
	velocity = v;
}

Bird::Bird() : Bird(0, 0) {};

void Bird::draw(wxDC& dc)
{
	dc.SetBrush(wxBrush(wxColor(255, 0, 0, 255)));
	dc.DrawRectangle(50, 800 - height, 10, 10);
}
