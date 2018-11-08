

#ifndef SIGNAL_STEP_H
#define SIGNAL_STEP_H

#include "SignalBase.hpp"


class SignalStep : public SignalBase
{
	public:
	
		SignalStep(double amplitude, double frequency, double phaseShift, double splitFactor = 0.5)
			: SignalBase::SignalBase(amplitude, frequency, phaseShift, splitFactor)
		{}
		
		virtual ~SignalStep(){}
		

		virtual double value
		(
			const unsigned long & micros
		) const override
		{
			return _amplitude * (this->argument(micros) / (2 * this->pi()) < _splitFactor ? 1 : -1);
		}
		
	protected:
		
};


#endif // SIGNAL_SINE_H