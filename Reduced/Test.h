

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


#ifndef SIGNAL_GENERATOR_H
#define SIGNAL_GENERATOR_H

#include <vector>
#include <numeric>
#include <algorithm>
#include "SignalBase.hpp"
#include "SignalConstant.hpp"
#include "SignalSine.hpp"
#include "SignalStep.hpp"
#include "SignalTriangle.hpp"


class SignalGenerator
{
	public:
	
		enum class SignalType{BASE = 0, CONSTANT, SINE, STEP, TRIANGLE};

		~SignalGenerator()
		{
			for (auto & i : _signals)
			{
				delete i;
			}
		}


		inline double maxPeriodTime()
		{
			auto signalsTemp = _signals;
			if (signalsTemp.size() == 0)
				return 0.;
			auto element = *std::max_element(signalsTemp.begin(), signalsTemp.end(), [](const SignalBase * signal1, const SignalBase * signal2) {return signal1->periodTime()<signal2->periodTime();});
			element->periodTime();
		}


		inline double value
		(
			const unsigned long & now
		) const
		{
			return std::accumulate(_signals.begin(), _signals.end(), 0.0, [&now](const double & sum, SignalBase * signal) { return sum + signal->value(now); });
		}



		static SignalBase * buildSignal
		(
			const SignalType & type,
			const double & amplitude,
			const double & frequency,
			const double & phaseShift,
			const double & splitFactor = 0.5
		)
		{
			switch (type)
			{
				case SignalType::CONSTANT:
					return new SignalConstant(amplitude, frequency, phaseShift);
				case SignalType::SINE:
					return new SignalSine(amplitude, frequency, phaseShift);
				case SignalType::STEP:
					return new SignalStep(amplitude, frequency, phaseShift, splitFactor);
				case SignalType::TRIANGLE:
					return new SignalTriangle(amplitude, frequency, phaseShift);
				default:
					return new SignalBase();
			}
		}


		inline void addSignal
		(
			const SignalType & type,
			const double & amplitude,
			const double & frequency,
			const double & phaseShift,
			const double & splitFactor = 0.5
		)
		{
			_signals.push_back(this->buildSignal(type, amplitude, frequency, phaseShift, splitFactor));
		}


		template<class SignalClass>
		inline void addSignal
		(
			const SignalClass & signal
		)
		{
			_signals.push_back(new SignalClass(signal));
		}


		inline void replaceSignal
		(
			const std::size_t & i,
			SignalBase * & signal
		)
		{
			delete _signals[i];
			_signals[i] = signal;
		}


		inline void replaceSignal
		(
			const int & i,
			const SignalType & type,
			const double & amplitude,
			const double & frequency,
			const double & phaseShift,
			const double & splitFactor = 0.5
		)
		{
			delete _signals[i];
			_signals[i] = this->buildSignal(type, amplitude, frequency, phaseShift, splitFactor);
		}


	protected:

		std::vector<SignalBase*> _signals;
};




#endif // SIGNAL_GENERATOR_H


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
