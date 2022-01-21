#include "include/Game.hpp"
#include <iostream>

#define GRAVITY 25
#define SPEED 5

Game::Game(int w, int h)
{
	height = h;
	width = w;
	paused = true;

	bird = Bird(height / 2, 0);

	pipes = {Pipe(300, 400, 100), Pipe(600, 200, 75), Pipe(900, 100, 100)};
}

void Game::draw(wxDC& dc)
{
	bird.draw(dc);
	for (Pipe& p : pipes)
	{
		p.draw(dc);
	}
}

void Game::update(double tps)
{
	if (paused) return;

	if (bird.falling)
		bird.velocity -= GRAVITY / tps;
	else bird.velocity = 0;
	double terminal_v = -17;
	if (bird.velocity < terminal_v) bird.velocity = terminal_v;
	int new_height = bird.height + bird.velocity;
	if (new_height <= 0 + bird.size)
	{
		bird.falling = false;
		new_height = 0 + bird.size;
	}
	if (new_height >= height)
	{
		new_height = height;
		bird.velocity = 0;
	}
	bird.height = new_height;

	for (Pipe& p : pipes)
	{
		p.x -= SPEED;
		if (p.x < -25)
		{
			p = Pipe(1000 + rand() % 100 - 50, rand() % 600, 100);
		}
	}
}

void Game::OnKeyPressed(wxKeyEvent& evt)
{
	wxChar uc = evt.GetUnicodeKey();
	if (uc != WXK_NONE)
	{
		if (uc == WXK_RETURN)
		{

		}
		else if (uc == WXK_SPACE)
		{
			paused = false;
			double top_speed = 17.0;
			double speed_left = top_speed - bird.velocity;
			double delta_v = 12;
			if (speed_left / 2 < 10) delta_v = speed_left / 2;
			double new_velocity = bird.velocity + delta_v;
			//if (new_velocity > top_speed) new_velocity = top_speed;
			bird.velocity = new_velocity;
			bird.falling = true;
			std::cout << bird.velocity << "\n";
		}
		std::cout << "you pressed: " << (char)uc << std::endl;
	}
	else
	{
		bool shift = evt.ShiftDown();
		int num = evt.GetKeyCode();
	}

}

Bird::Bird(int h, double v)
{
	height = h;
	velocity = v;
	falling = true;
}

Bird::Bird() : Bird(0, 0) {};

void Bird::draw(wxDC& dc)
{
	dc.SetBrush(wxBrush(wxColor(255, 0, 0, 255)));
	dc.DrawRectangle(100, 800 - height, size, size);
}

void Pipe::draw(wxDC& dc)
{
	dc.SetBrush(wxBrush(wxColor(0, 255, 0, 255)));
	dc.DrawRectangle(x, 0, 75, y - gap / 2);
	dc.DrawRectangle(x, y + gap / 2, 75, 800 - (y + gap / 2));
}

Pipe::Pipe(int x, int y, int gap)
{
	this->x = x;
	this->y = y;
	this->gap = gap;
}
