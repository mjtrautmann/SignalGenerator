

#ifndef SIGNAL_TRIANGLE_H
#define SIGNAL_TRIANGLE_H

#include "SignalBase.h"


class SignalTriangle : public Signal
{
	public:
	
		SignalTriangle(double amplitude, double frequency, double phaseShift, double splitFactor = 0.5)
			: Signal::Signal(amplitude, frequency, phaseShift, splitFactor)
		{}
		
		virtual ~SignalTriangle(){}
		

		virtual Type type() const
		{
			return Type::TRIANGLE;
		}

		virtual double value
		(
			const std::chrono::high_resolution_clock::time_point & now
		) const override
		{
			auto f = 2 * (this->argument(now) - pi()) / this->pi();
			return _amplitude * (std::abs(f)-1);
		}
		
	protected:
		
};


#endif // SIGNAL_TRIANGLE_H