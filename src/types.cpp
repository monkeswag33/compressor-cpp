#include "types.h"

bool Compare::operator()(Node a, Node b) {
    return a.frequency > b.frequency;
}
