

#ifndef SIGNAL_SINE_H
#define SIGNAL_SINE_H

#include "SignalBase.hpp"
#include <math.h>



class SignalSine : public SignalBase
{
	public:
	
		SignalSine(double amplitude, double frequency, double phaseShift, double splitFactor = 0.5)
			: SignalBase::SignalBase(amplitude, frequency, phaseShift, splitFactor)
		{}

		virtual ~SignalSine() {}


		virtual double value
		(
			const unsigned long & micros
		) const override
		{
			return _amplitude * sin(this->argument(micros));
		}
		
	protected:
		
};


#endif // SIGNAL_SINE_H