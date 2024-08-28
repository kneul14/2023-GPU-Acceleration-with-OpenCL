#include "GrassState.h"

void UpdateStateInKernel(__global GrassState* currentState, int aliveCount) {

    switch (*currentState) {
    case startingStateAlive:
        if (!(aliveCount == 2 || aliveCount == 3)) {
            *currentState = otherAliveCells;
        }
        else {
            *currentState = aliveWith2or3Neighbors;
        }
        break;
    case startingStateDead:
        if (aliveCount < 3) {
            *currentState = otherDeadCells;

        }
        else {
            *currentState = deadWith3Neighbors;
        }
        break;
    default:
        break;
    }
}

// https://github.com/openwall/john/issues/5456
// (): Warning: Function StateTransitionsKernel is a kernel, so overriding noinline attribute. The function may be inlined when called.

// Writing 'inline' stops the warning/ bug that was happening
// However the kernel creation fails due to this
// Seems to be an Nvidia driver bug
// Please see link

__kernel /*inline*/ void UpdateStateKernel(__global GrassState * currentState, __global int* aliveCount)
{
    // Get index
    int idx = get_global_id(0);

    UpdateStateInKernel(&currentState[idx], aliveCount[idx]);
}