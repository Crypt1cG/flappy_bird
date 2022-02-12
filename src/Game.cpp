#include "include/Game.hpp"
#include <iostream>

#define GRAVITY 40
#define SPEED 4
#define GAP 175

Game::Game(int w, int h)
{
	height = h;
	width = w;
	paused = true;
	ended = false;

	bird = Bird(height / 2, 0);
	bird.falling = false;

	pipes = {Pipe(550, rand() % 400 + 200, GAP), Pipe(900, rand() % 400 + 200, GAP), Pipe(1250, rand() % 400 + 200, GAP), Pipe(1600, rand() % 400 + 200, GAP)};
}

void Game::draw(wxDC& dc)
{
	for (Pipe& p : pipes)
	{
		p.draw(dc, width, height);
	}
	bird.draw(dc);
}

void Game::reset()
{
	pipes = {Pipe(550, rand() % 400 + 200, GAP), Pipe(900, rand() % 400 + 200, GAP), Pipe(1250, rand() % 400 + 200, GAP), Pipe(1600, rand() % 400 + 200, GAP)};
	bird = Bird(height / 2, 0);
	bird.falling = false;
	paused = true;
	ended = false;
}

void Game::update(double tps)
{
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

	if (ended) return;
	if (paused) return;

	for (int i = 0; i < pipes.size(); i++)
	{
		Pipe& p = pipes[i];
		p.x -= SPEED;
		if (p.x < -50)
		{
			int y = rand() % 400 + 200;
			int prev_index = i - 1 >= 0 ? i - 1 : pipes.size() - 1;
			int x = pipes[prev_index].x + 300 + rand() % 100;
			p = Pipe(x, y, GAP);
		}
		else
		{
			// bird left side is x = 100, it is bird.size wide
			// pipes are 75 wide
			if (p.x >= 25 && p.x <= 100 + bird.size) // when p.x = 25, right edge is on left edge of bird
			{
				int bird_y = 800 - bird.height;
				if (bird_y <= p.y - p.gap / 2 || bird_y + bird.size >= p.y + p.gap / 2)
				{
					paused = true;
					bird.velocity = 0;
					ended = true;
				}
			}
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
		else if (uc == WXK_SPACE && !ended)
		{
			paused = false;
			double top_speed = 12.0;
			double speed_left = top_speed - bird.velocity;
			double delta_v = 12;
			if (speed_left / 2 < 10) delta_v = speed_left / 2;
			double new_velocity = bird.velocity + delta_v;
			//if (new_velocity > top_speed) new_velocity = top_speed;
			bird.velocity = top_speed;
			bird.falling = true;
		}
		else if (uc == 'r')
			reset();
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

void Pipe::draw(wxDC& dc, int width, int height)
{
	if (x > width) return;
	dc.SetBrush(wxBrush(wxColor(0, 255, 0, 255)));
	dc.DrawRectangle(x, 0, 75, y - gap / 2);
	dc.DrawRectangle(x, y + gap / 2, 75, height - (y + gap / 2));
}

Pipe::Pipe(int x, int y, int gap)
{
	this->x = x;
	this->y = y;
	this->gap = gap;
}
