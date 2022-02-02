#include <wx/wx.h>
#include <wx/timer.h>
#include "include/Game.hpp"
#include "wx/defs.h"
#include <iostream>

#define WIDTH 1000
#define HEIGHT 800
#define TICK_SPEED 16 // ms

class Panel;

class Timer : public wxTimer
{
	Panel* panel;
public:
	Timer(Panel* panel);
	void Notify();
	void start();
};

class Panel : public wxPanel
{
public:
	Panel(wxWindow* parent);
	~Panel();
	Game* game;

	void paintEvent(wxPaintEvent& event);
	void paintNow();
	void render(wxDC& dc);
};

class Frame;

class App : public wxApp
{
	bool OnInit();
	Frame* frame;
};

// https://wiki.wxwidgets.org/Making_a_render_loop

Timer::Timer(Panel* panel) : wxTimer()
{
	Timer::panel = panel;
}

void Timer::Notify()
{
	panel->game->update(1000.0 / TICK_SPEED);
	panel->Refresh();
}

void Timer::start()
{
	wxTimer::Start(TICK_SPEED); // ~60 fps (16 ms per frame)
}

IMPLEMENT_APP(App)

class Frame : public wxFrame
{
public:
	Frame() : wxFrame(NULL, wxID_ANY, "Flappy Bird")
	{
	#ifdef __MACH__
		SetPosition(wxPoint(50, 50));
	#endif
		SetClientSize(wxSize(WIDTH, HEIGHT));
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		panel = new Panel(this);
		sizer->Add(panel, 1, wxEXPAND);
		this->SetSizer(sizer);

		timer = new Timer(panel);

		Bind(wxEVT_CLOSE_WINDOW, &Frame::onClose, this);
		Show();
		timer->start();
	}
	~Frame()
	{
		delete timer;
	}
	void onClose(wxCloseEvent& evt)
	{
		timer->Stop();
		evt.Skip();
	}
private:
	Timer* timer;
	Panel* panel;
};

bool App::OnInit()
{
	//wxInitAllImageHandlers();
	frame = new Frame();
	frame->Show(true);
	//SetTopWindow(frame);
	return true;
}



Panel::Panel(wxWindow* parent) : wxPanel(parent)
{
	game = new Game(WIDTH, HEIGHT);
	Bind(wxEVT_CHAR, &Game::OnKeyPressed, game);
	Bind(wxEVT_PAINT, &Panel::paintEvent, this);
}

Panel::~Panel()
{
	delete game;
}

void Panel::paintEvent(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}

void Panel::paintNow()
{
	wxClientDC dc(this);
	render(dc);
}

void Panel::render(wxDC& dc)
{
	game->draw(dc);
}
