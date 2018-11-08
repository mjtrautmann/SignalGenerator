

#ifndef SIGNAL_TRIANGLE_H
#define SIGNAL_TRIANGLE_H

#include "SignalBase.hpp"


class SignalTriangle : public SignalBase
{
	public:
	
		SignalTriangle(double amplitude, double frequency, double phaseShift, double splitFactor = 0.5)
			: SignalBase::SignalBase(amplitude, frequency, phaseShift, splitFactor)
		{}
		
		virtual ~SignalTriangle(){}
		

		virtual double value
		(
			const unsigned long & micros
		) const override
		{
			double f = 2 * (this->argument(micros) - pi()) / this->pi();
			return _amplitude * (abs(f)-1);
		}
		
	protected:
		
};


#endif // SIGNAL_TRIANGLE_H