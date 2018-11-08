

#ifndef SIGNAL_CONSTANT_H
#define SIGNAL_CONSTANT_H

#include "SignalBase.hpp"


class SignalConstant : public SignalBase
{
	public:

		SignalConstant(double amplitude, double frequency, double phaseShift) : SignalBase::SignalBase(amplitude, frequency, phaseShift) {}

		virtual ~SignalConstant() {}


		virtual double periodTime() const override
		{
			return 0;
		}


		virtual double value
		(
			const unsigned long & micros
		) const override
		{
			(void)micros;
			return _amplitude;
		}

	protected:

};


#endif // SIGNAL_CONSTANT_H