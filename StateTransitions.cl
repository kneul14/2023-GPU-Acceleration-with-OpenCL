#include "GrassState.h"
//OpenCL operates on lower-level constructs compared to C++, so you need to manage the passing of data explicitly??

enum gameObjectflags {
    base = 1 << 0,
    alive = 1 << 1,
    dead = 1 << 2,
    infected = 1 << 3,
    gnome = 1 << 4,
    infectedAndDead = 1 << 5

};

void StateTransitionsInKernel(__global GrassState* currentState, int aliveCounts, __global unsigned int* grassFlags) {

    switch (*currentState) {
    case otherAliveCells:
        if (aliveCounts < 2 || aliveCounts > 3) {
            *grassFlags &= ~alive; // Removes alive flag?
            *grassFlags |=  dead; // Add dead flag if not already dead

        }
        break;
    case otherDeadCells:
        // Do nothing, already dead
        break;
    default:
        // Do nothing
        break;
    }
}

// https://github.com/openwall/john/issues/5456
// (): Warning: Function StateTransitionsKernel is a kernel, so overriding noinline attribute. The function may be inlined when called.

// Writing 'inline' stops the warning/ bug that was happening
// Seems to be an Nvidia driver bug
// Please see link

__kernel inline void StateTransitionsKernel( __global GrassState* currentState, __global int* aliveCounts, __global unsigned int* grassFlags)
{
    // Get index
    int idx = get_global_id(0);

    StateTransitionsInKernel(&currentState[idx], aliveCounts[idx], &grassFlags[idx]);

}
