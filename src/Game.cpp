#include "include/Game.hpp"
#include "wx/font.h"
#include <iostream>

#define GRAVITY 40
#define SPEED 4
#define TOP_SPEED 12
#define GAP 175

Game::Game(int w, int h)
{
	height = h;
	width = w;
	paused = true;

	//bird = Bird(height / 2, 0);
	//bird.falling = false;

	for (int i = 0; i < num_nns; i++)
	{
		nets.push_back(NeuralNet());
		//nets[i].print();
		birds.push_back(Bird(height / 2, 0));
		birds[i].falling = true;
		birds[i].moving = true;
	}

	pipes = {Pipe(550, rand() % 400 + 200, GAP), Pipe(900, rand() % 400 + 200, GAP), Pipe(1250, rand() % 400 + 200, GAP), Pipe(1600, rand() % 400 + 200, GAP)};
}

void Game::draw(wxDC& dc)
{
	for (Pipe& p : pipes)
	{
		p.draw(dc, width, height);
	}

	std::string msg = std::to_string(score);
	dc.SetFont(wxFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	dc.DrawText(msg, width / 2, 100);

	dc.SetBrush(wxBrush(wxColor(255, 0, 0, 50)));
	for (int i = 1; i < birds.size(); i++)
	{
		birds[i].draw(dc);
	}
	dc.SetBrush(wxBrush(wxColor(0, 0, 255, 255)));
	birds[0].draw(dc);
	//bird.draw(dc);
}

void Game::reset()
{
	pipes = {Pipe(550, rand() % 400 + 200, GAP), Pipe(900, rand() % 400 + 200, GAP), Pipe(1250, rand() % 400 + 200, GAP), Pipe(1600, rand() % 400 + 200, GAP)};

	for (int i = 0; i < num_nns; i++)
	{
		birds[i] = Bird(height / 2, 0);
		birds[i].falling = true;
		birds[i].moving = true;
	}
	// do something to update the nns
	if (nets.size() > 2)
	{
		nets.erase(nets.begin() + 2, nets.end());
	}
	// there should be one left in nets
	for (int i = nets.size(); i < num_nns; i++)
	{
		nets.push_back(nets[0].create_random_child(0.5, std::max(0.01, 0.6 - generation / 100.0)));
		//nets[i].print();
	}

	generation++;
	if (generation % 10 == 0) std::cout << "Generation: " << generation << std::endl;
	
	score = 0;
	
	//bird = Bird(height / 2, 0);
	//bird.falling = false;
	//paused = true;
}

void Game::update(double tps)
{
	if (paused) return;

	// move the pipes
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
		else if (p.x < 25 && p.x >= 25 - SPEED)
			score++;
	}

	// move the birds
	std::vector<Bird> new_birds; // useful so that we can discard any birds that go off screen
	std::vector<NeuralNet> new_nets;
	for (int i = 0; i < num_nns; i++)
	{
		Bird& bird = birds[i];
		if (bird.falling)
		{
			if (bird.falling)
				bird.velocity -= GRAVITY / tps;
			else bird.velocity = 0;

			double terminal_v = -17;
			if (bird.velocity < terminal_v) bird.velocity = terminal_v;
			int new_height = bird.height + bird.velocity;
			if (new_height <= 0 + bird.size)
			{
				bird.moving = false;
				bird.falling = false;
				bird.velocity = 0;
				new_height = 0 + bird.size;
			}
			else if (new_height >= height)
			{
				new_height = height;
				bird.velocity = 0;
			}
			bird.height = new_height;
		}
		if (!bird.moving)
			bird.x -= SPEED;

		if (bird.moving || (!bird.moving && bird.x > 0))
		{
			new_nets.push_back(nets[i]);
			new_birds.push_back(bird);
		}
	}
	birds = new_birds;
	if (new_nets.size() >= 1)
		nets = new_nets;
	if (birds.size() == 0) reset();

	// check for collisions
	// all birds that are not dead are at x=100
	for (Pipe& p : pipes)
	{
		if (p.x >= 25 && p.x <= 100 + birds[0].size) // when p.x = 25, right edge is on left edge of bird
		{
			for (Bird& b : birds)
			{
				int bird_y = 800 - b.height;
				if (bird_y <= p.y - p.gap / 2 || bird_y + b.size >= p.y + p.gap / 2)
				{
					b.velocity = 0;
					b.moving = false;
				}
			}
		}
	}

	tick++;
	if (tick % 2 == 0)
	{
		int closest_x = width;
		int closest_y;
		for (int i = 0; i < pipes.size(); i++)
		{
			if (pipes[i].x > 25 && pipes[i].x < closest_x)
			{
				closest_x = pipes[i].x;
				closest_y = pipes[i].y;
			}
		}
		double closest_height = (double)height - closest_y;
		double delta_x = closest_x - 100.0;
		for (int i = 0; i < num_nns; i++)
		{
			// if (birds[i].moving && (double)rand() / RAND_MAX > 0.8)
			// {
			// 	birds[i].jump();
			// }
			if (birds[i].moving)
			{
				double inputs[] = {delta_x, closest_height - birds[i].height, birds[i].velocity};
				//nets[i].print();
				double* res = nets[i].eval(inputs);
				//std::cout << res[0] << ", ";
				if (res[0] > 0.5)
				{
					birds[i].jump();
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
		if (uc == WXK_SPACE)
		{
			paused = false;
			//bird.jump();
		}
		else if (uc == 's')
		{
			nets[0].write("best.txt");
		}
		else if (uc == 'l')
		{
			nets[0] = NeuralNet::read("best.txt");
			reset();
		}
		else if (uc == 'p')
		{
			nets[0].print();
		}
		else if (uc == 'r')
			reset();
		// std::cout << "you pressed: " << (char)uc << std::endl;
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
	dc.DrawRectangle(x, 800 - height, size, size);
}

void Bird::jump()
{
	velocity = TOP_SPEED;
	falling = true;
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
