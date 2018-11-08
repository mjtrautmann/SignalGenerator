

#ifndef SIGNAL_SINE_H
#define SIGNAL_SINE_H

#include "SignalBase.h"
#include <math.h>



class SignalSine : public Signal
{
	public:
	
		SignalSine(double amplitude, double frequency, double phaseShift, double splitFactor = 0.5)
			: Signal::Signal(amplitude, frequency, phaseShift, splitFactor)
		{}

		virtual ~SignalSine() {}


		virtual Type type() const
		{
			return Type::SINE;
		}

		virtual double value
		(
			const std::chrono::high_resolution_clock::time_point & now
		) const override
		{
			return _amplitude * std::sin(this->argument(now));
		}
		
	protected:
		
};


#endif // SIGNAL_SINE_H