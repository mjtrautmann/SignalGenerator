// Base class for a signal type which is used as an signal generator in raspberry pi or whatever device is used with GPIOs

#ifndef SIGNAL_BASE_H
#define SIGNAL_BASE_H

#include <math.h>
#include <iostream>
#include <chrono>
#include <cstdint>
#include <atomic>

class Signal
{
	public:

		enum class Type { BASE, CONSTANT, SINE, STEP, TRIANGLE };

		Signal()
			: _amplitude(0), _frequency(1), _phaseShift(0), _splitFactor(0.5)
		{ }

		Signal(double amplitude, double frequency, double phaseShift, double splitFactor = 0.5)
			: _amplitude(amplitude), _frequency(frequency), _phaseShift(phaseShift), _splitFactor(splitFactor)
		{ }

		Signal(const Signal & rhs) : _amplitude(rhs.amplitude()), _frequency(rhs.frequency()), _phaseShift(rhs.phaseShift()), _splitFactor(rhs.splitFactor())
		{ }

		virtual ~Signal() {}


		virtual Type type() const
		{
			return Type::BASE;
		}

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
			return 3.14159265359;
		}

		double argument
		(
			const std::chrono::high_resolution_clock::time_point & now
		) const
		{
			auto timeSeconds = now.time_since_epoch().count() / 1e9 + _phaseShift / (2 * pi() * _frequency);
			if (timeSeconds < 0)
				timeSeconds += std::int64_t(-timeSeconds * _frequency + 1) / _frequency;
			timeSeconds -= std::uint64_t(timeSeconds * _frequency) * this->periodTime();

			return 2 * pi() * _frequency * timeSeconds;
		}


		void setAmplitude
		(
			const double & amplitude
		)
		{
			_amplitude = amplitude;
		}

		void setFrequency
		(
			const double & frequency
		)
		{
			_frequency = frequency;
		}

		void setPhaseShift
		(
			const double & phaseShift
		)
		{
			_phaseShift = phaseShift; 
		}

		void setSplitFactor
		(
			const double & splitFactor
		)
		{
			_splitFactor = splitFactor;
		}


		virtual double value
		(
			const std::chrono::high_resolution_clock::time_point & now
		) const
		{
			(void)now;
			return 0.; 
		}

		double value() const
		{ 
			return this->value(std::chrono::high_resolution_clock::now());
		}

	protected:

		std::atomic<double> _amplitude;
		std::atomic<double> _frequency;
		std::atomic<double> _phaseShift;
		std::atomic<double> _splitFactor;

};


#endif // SIGNAL_BASE_H














