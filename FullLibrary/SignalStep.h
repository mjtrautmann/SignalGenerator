

#ifndef SIGNAL_STEP_H
#define SIGNAL_STEP_H

#include "SignalBase.h"


class SignalStep : public Signal
{
	public:
	
		SignalStep(double amplitude, double frequency, double phaseShift, double splitFactor = 0.5)
			: Signal::Signal(amplitude, frequency, phaseShift, splitFactor)
		{}
		
		virtual ~SignalStep(){}
		

		virtual Type type() const
		{
			return Type::STEP;
		}

		virtual double value
		(
			const std::chrono::high_resolution_clock::time_point & now
		) const override
		{
			return _amplitude * (this->argument(now) / (2 * this->pi()) < _splitFactor ? 1 : -1);
		}
		
	protected:
		
};


#endif // SIGNAL_SINE_H