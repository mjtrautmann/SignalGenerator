

#ifndef SIGNAL_CONSTANT_H
#define SIGNAL_CONSTANT_H

#include "SignalBase.h"


class SignalConstant : public Signal
{
	public:

		SignalConstant(double amplitude, double frequency, double phaseShift) : Signal::Signal(amplitude, frequency, phaseShift) {}

		virtual ~SignalConstant() {}


		virtual Type type() const
		{
			return Type::CONSTANT;
		}

		virtual double periodTime() const override
		{
			return 0;
		}

		virtual double value
		(
			const std::chrono::high_resolution_clock::time_point & now
		) const override
		{
			(void)now;
			return _amplitude;
		}

	protected:

};


#endif // SIGNAL_CONSTANT_H