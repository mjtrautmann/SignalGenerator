

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
	
		const auto & signalVector() const
		{
			return _signals;
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
