#include <chrono>

#include "Util.hpp"

using namespace std::chrono;

double Util::obter_tempo()
{
    return duration_cast<microseconds>(
        system_clock::now().time_since_epoch()
	).count() ;
}