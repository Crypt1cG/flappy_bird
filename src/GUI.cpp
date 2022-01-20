#include <wx/wx.h>
#include <wx/timer.h>
#include <iostream>

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

	void paintEvent(wxPaintEvent& event);
	void paintNow();
	void render(wxDC& dc);
	//void OnKeyPressed(wxKeyEvent& event);
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
	std::cout << "tick" << std::endl;
	panel->Refresh();
}

void Timer::start()
{
	std::cout << "timer started" << std::endl;
	wxTimer::Start(1000);
}

IMPLEMENT_APP(App)

class Frame : public wxFrame
{
public:
	Frame() : wxFrame(NULL, wxID_ANY, "Mandelbrot")
	{
		SetInitialSize(wxSize(1000, 800));
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
	std::srand(std::time(0)); // so rand num is not the same every time ! (rand based on current time)

	//Bind(wxEVT_CHAR, &Panel::OnKeyPressed, this);
	Bind(wxEVT_PAINT, &Panel::paintEvent, this);
}

//void Panel::OnKeyPressed(wxKeyEvent& event)
//{
//	wxChar uc = event.GetUnicodeKey();
//	if (uc != WXK_NONE)
//	{
//		std::cout << "you pressed: " << (char)uc << std::endl;
//		paintNow();
//	}
//	else
//	{
//		bool shift = event.ShiftDown();
//		int num = event.GetKeyCode();
//		paintNow();
//	}
//}

void Panel::paintEvent(wxPaintEvent& event)
{
	std::cout << "1" << std::endl;
	wxPaintDC dc(this);
	render(dc);
}

void Panel::paintNow()
{
	std::cout << "2" << std::endl;
	wxClientDC dc(this);
	render(dc);
}

void Panel::render(wxDC& dc)
{
	std::cout << "tock?" << std::endl;
}
