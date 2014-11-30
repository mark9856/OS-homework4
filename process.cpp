#include "process.h"

bool
Process::addTime(int arrive, int leave) {
    return (time.insert(std::make_pair(arrive, leave)).second);
}