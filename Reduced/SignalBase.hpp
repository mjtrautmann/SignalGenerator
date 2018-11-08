// Base class for a signal type which is used as an signal generator in raspberry pi or whatever device is used with GPIOs

#ifndef SIGNAL_BASE_H
#define SIGNAL_BASE_H

#include <math.h>

class SignalBase
{
	public:

		SignalBase()
			: _amplitude(0), _frequency(1), _phaseShift(0), _splitFactor(0.5)
		{ }

		SignalBase(double amplitude, double frequency, double phaseShift, double splitFactor = 0.5)
			: _amplitude(amplitude), _frequency(frequency), _phaseShift(phaseShift), _splitFactor(splitFactor)
		{ }

		SignalBase(const SignalBase & rhs) : _amplitude(rhs.amplitude()), _frequency(rhs.frequency()), _phaseShift(rhs.phaseShift()), _splitFactor(rhs.splitFactor())
		{ }

		virtual ~SignalBase() {}


		double amplitude() const
		{
			return _amplitude; 
		}

		double frequency() const
		{ 
			return _frequency;
		}

		virtual double periodTime() const
		{
			return 1 / _frequency;
		}

		double phaseShift() const
		{
			return _phaseShift;
		}

		double splitFactor() const
		{
			return _splitFactor;
		}

		static double pi()
		{
			return 3.14159265359f;
		}

		double argument
		(
			const unsigned long & micros
		) const
		{
			double timeSeconds = micros / 1e6 + _phaseShift / (2 * pi() * _frequency);
			if (timeSeconds < 0)
				timeSeconds += int64_t(-timeSeconds * _frequency + 1) / _frequency;
			timeSeconds -= uint64_t(timeSeconds * _frequency) * this->periodTime();

			return 2 * pi() * _frequency * timeSeconds;
		}


		void setAmplitude
		(
			double amplitude
		)
		{
			_amplitude = amplitude;
		}

		void setFrequency
		(
			double frequency
		)
		{
			_frequency = frequency;
		}

		void setPhaseShift
		(
			double phaseShift
		)
		{
			_phaseShift = phaseShift; 
		}

		void setSplitFactor
		(
			double splitFactor
		)
		{
			_splitFactor = splitFactor;
		}


		virtual double value
		(
			const unsigned long & micros
		) const
		{
			(void)micros;
			return 0.; 
		}

	protected:

		double _amplitude;
		double _frequency;
		double _phaseShift;
		double _splitFactor;

};


#endif // SIGNAL_BASE_H














