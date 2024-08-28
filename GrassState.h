#pragma once

//enum GrassState
//{
//	startingStateAlive,
//	startingStateDead,
//	aliveWith2or3Neighbors,
//	deadWith3Neighbors,
//	otherAliveCells,
//	otherDeadCells,
//};

typedef enum {
    startingStateAlive,
    startingStateDead,
    aliveWith2or3Neighbors,
    deadWith3Neighbors,
    otherAliveCells,
    otherDeadCells,
} GrassState;
