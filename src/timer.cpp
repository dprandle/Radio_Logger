#include <iostream>
#include <string>

#include "logger.h"
#include "utility.h"
#include "timer.h"

Timer::Timer():
    m_running(false),
	m_start(),
	m_prev(),
	m_cur(),
	m_last_cb(),
	m_cb_delay(),
	m_pause(),
    m_cmode(no_shot)
{}

Timer::~Timer()
{}

void Timer::start()
{
	m_running = true;
	clock_gettime(CLOCK_MONOTONIC, &m_cur);
	m_prev = m_cur;
	timespec cur_start = m_cur - (m_pause - m_start);
	m_last_cb = cur_start + (m_last_cb - m_start);
	m_start = cur_start;
	m_pause = m_start;
}

std::function<void(Timer*)> Timer::callback()
{
	return m_cb;
}

Timer::cb_mode Timer::callback_mode()
{
	return m_cmode;
}

void Timer::set_callback(std::function<void(Timer*)> cb_func)
{
	m_cb = cb_func;
}

double Timer::callback_delay()
{
	return to_ms_(m_cb_delay);
}

void Timer::set_callback_mode(cb_mode mode)
{
	m_cmode = mode;
	m_last_cb = m_prev;
}

void Timer::set_callback_delay(double ms)
{
	m_cb_delay = from_ms_(ms);
}

void Timer::update()
{
	if (!m_running)
		return;

	m_prev = m_cur;
	clock_gettime(CLOCK_MONOTONIC, &m_cur);

	timespec elpsed = m_cur - m_start;
	timespec cb_elapsed = m_cur - m_last_cb;
	if (((m_cmode == single_shot) && (m_last_cb == m_start) && (elpsed >= m_cb_delay)) ||
	    ((m_cmode == continous_shot) && (cb_elapsed >= m_cb_delay)))
	{
		m_last_cb = m_cur;
		if (m_cb == nullptr)
			wlog("No callback set for timer yet callback mode is enabled");
		else
			m_cb(this);
	}
}

bool Timer::paused()
{
	return (!running() && !(m_pause == m_start));
}


void Timer::stop()
{
	update();
	m_running = false;
}

void Timer::pause()
{
	stop();
	clock_gettime(CLOCK_MONOTONIC, &m_pause);
}

bool Timer::running()
{
	return m_running;
}

double Timer::to_ms_(const timespec & tspec)
{
	return double(tspec.tv_sec) * 1000.0 + double(tspec.tv_nsec) * 0.000001;
}

timespec Timer::from_ms_(double ms)
{
	timespec rtn;
	double whole, fract;
	fract = modf(ms*0.001, &whole);
	rtn.tv_sec = whole;
	rtn.tv_nsec = fract * 1000000000;
	return rtn;
}

double Timer::dt()
{
	timespec delta = m_cur - m_prev;
	return to_ms_(delta);
}

double Timer::elapsed()
{
	timespec delta = m_cur - m_start;
	return to_ms_(delta);
}

double Timer::elapsed_since_callback()
{
	timespec delta = m_cur - m_last_cb;
	return to_ms_(delta);
}

bool operator<(const timespec& lhs, const timespec& rhs)
{
    if (lhs.tv_sec == rhs.tv_sec)
        return lhs.tv_nsec < rhs.tv_nsec;
    else
        return lhs.tv_sec < rhs.tv_sec;
}

bool operator<=(const timespec& lhs, const timespec& rhs)
{
    if (lhs.tv_sec <= rhs.tv_sec)
        return lhs.tv_nsec <= rhs.tv_nsec;
    else
        return false;
}

bool operator==(const timespec& lhs, const timespec& rhs)
{
    if (lhs.tv_sec == rhs.tv_sec)
        return lhs.tv_nsec == rhs.tv_nsec;
    else
        return false;
}

bool operator>=(const timespec& lhs, const timespec& rhs)
{
	return !(lhs < rhs);
}

bool operator>(const timespec& lhs, const timespec& rhs)
{
	return !(lhs <= rhs);
}

timespec operator-(const timespec& lhs, const timespec& rhs)
{
	timespec rtn;
	rtn.tv_sec = lhs.tv_sec - rhs.tv_sec;
	rtn.tv_nsec = lhs.tv_nsec - rhs.tv_nsec;
	return rtn;
}

timespec operator+(const timespec& lhs, const timespec& rhs)
{
	timespec rtn;
	rtn.tv_sec = lhs.tv_sec + rhs.tv_sec;
	rtn.tv_nsec = lhs.tv_nsec + rhs.tv_nsec;
	return rtn;
}
