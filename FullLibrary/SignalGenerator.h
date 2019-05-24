

#ifndef SIGNAL_GENERATOR_H
#define SIGNAL_GENERATOR_H

#include <vector>
#include <numeric>
#include <algorithm>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>


#include "SignalBase.h"
#include "SignalConstant.h"
#include "SignalSine.h"
#include "SignalStep.h"
#include "SignalTriangle.h"

// You can of course try to change the code,
// but this is the most thread safe and flexible
// piece of code I have ever written in my 
// entire fucking life.


class SignalGenerator
{
	public:
	

		const auto signalVector() const
		{
			_mutex.lock();
			auto signalsTemp = _signals;
			_mutex.unlock();
			return signalsTemp;
		}


		inline double maxPeriodTime()
		{
			_mutex.lock();
			auto signalsTemp = _signals;
			_mutex.unlock();
			if (signalsTemp.size() == 0)
				return 0.;
			auto element = *std::max_element(signalsTemp.begin(), signalsTemp.end(), [](const std::shared_ptr<Signal> & signal1, const std::shared_ptr<Signal> & signal2) {return signal1->periodTime()<signal2->periodTime();});
			element->periodTime();
		}


		inline void removeSignal
		(
			const std::size_t & i
		)
		{
			(void)i;
			std::lock_guard<std::mutex> lock(_mutex);
			_signals.erase(_signals.begin(), _signals.begin() + 1);
		}


		inline double value
		(
			const std::chrono::high_resolution_clock::time_point & now = std::chrono::high_resolution_clock::now()
		) const
		{
			_mutex.lock();
			auto signalsTemp = _signals;
			_mutex.unlock();
			return std::accumulate(signalsTemp.begin(), signalsTemp.end(), 0.0, [&now](const double & sum, const std::shared_ptr<Signal> & signal) { return sum + signal->value(now); });
		}



		static std::shared_ptr<Signal> buildSignal
		(
			const Signal::Type & type,
			const double & amplitude,
			const double & frequency,
			const double & phaseShift,
			const double & splitFactor = 0.5
		)
		{
			switch (type)
			{
				case Signal::Type::CONSTANT:
					return std::make_shared<SignalConstant>(amplitude, frequency, phaseShift);
				case Signal::Type::SINE:
					return std::make_shared<SignalSine>(amplitude, frequency, phaseShift);
				case Signal::Type::STEP:
					return std::make_shared<SignalStep>(amplitude, frequency, phaseShift, splitFactor);
				case Signal::Type::TRIANGLE:
					return std::make_shared<SignalTriangle>(amplitude, frequency, phaseShift);
				default:
					return std::make_shared<Signal>();
			}
		}


		inline void addSignal
		(
			const Signal::Type & type,
			const double & amplitude,
			const double & frequency,
			const double & phaseShift,
			const double & splitFactor = 0.5
		)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_signals.push_back(this->buildSignal(type, amplitude, frequency, phaseShift, splitFactor));
		}


		inline void addSignal
		(
			const std::shared_ptr<Signal> & signal = std::make_shared<Signal>()
		)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_signals.push_back(signal);
		}


		template<class SignalClass>
		inline void addSignal
		(
			const SignalClass & signal
		)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_signals.push_back(std::make_shared<SignalClass>(signal));
		}


		inline void replaceSignal
		(
			const std::size_t & i,
			const std::shared_ptr<Signal> & signal
		)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_signals[i] = signal;
		}


		inline void replaceSignal
		(
			const int & i,
			const Signal::Type & type,
			const double & amplitude,
			const double & frequency,
			const double & phaseShift,
			const double & splitFactor = 0.5
		)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_signals[i] = this->buildSignal(type, amplitude, frequency, phaseShift, splitFactor);
		}


		template<class SignalClass>
		inline void replaceSignal
		(
			const std::size_t & i,
			const SignalClass & signal
		)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_signals[i] = std::make_shared<SignalClass>(signal);
		}


	protected:

		std::vector<std::shared_ptr<Signal>> _signals;
		mutable std::mutex _mutex;
};




#endif // SIGNAL_GENERATOR_H
