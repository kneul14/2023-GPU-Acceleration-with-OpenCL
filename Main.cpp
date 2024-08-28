#include <cmath>
#include <fstream>
#include <iostream>
#include "GameEngine.h"

using namespace std;
using namespace glm;

GameEngine engine;

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{    
	engine.Initialise();
}

// Main routine.
int main(int argc, char** argv)
{
	printInteraction();

	engine.InitEngine(argc, argv, "", 500, 500);
		
	engine.StartEngine();

	return 0;
}