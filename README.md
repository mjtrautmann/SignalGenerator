# SignalGenerator
A set of classes for the generation of different signal forms. Thread-safe, low-latency, high throughput is tried to be achieved.
With this library you can generate signal forms like sine, step or triangle. The classes for the signals are fully threadsafe for
write and read operations. The code was testes with several hundred of threads without finding a bottleneck. The signal generator
can be used for example for voltage sources with a special signal form.
There is a version in full modern C++ for the host side. A reduced version without smart pointers and multithreadin is also in-
cluded for a possible microcontroller implementation.
