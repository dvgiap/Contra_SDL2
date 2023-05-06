#ifndef IMP_TIMER_H
#define IMP_TIMER_H

class ImpTimer
{
public:
	ImpTimer();
	~ImpTimer();

	void start();
	int get_ticks();

private:
	int start_tick_;
	bool is_started_;
};

#endif // !IMP_TIMER_H
