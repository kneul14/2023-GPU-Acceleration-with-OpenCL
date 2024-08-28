#pragma once
#include <iostream>
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define _CRT_SECURE_NO_WARNINGS
#include <CL/cl.h>

#include "GrassState.h"

class openCLSetup
{
private:
	cl_device_id deviceID = NULL;
	cl_uint numPlatforms, numDevices;
	cl_int device;
	cl_platform_id* platforms = NULL;

	cl_context context;
	cl_command_queue command_queue;

	cl_program updateStateProgram  = nullptr;
	cl_kernel  updateStateKernel   = nullptr;	
	//cl_program stateTransitionProgram = nullptr;
	//cl_kernel  stateTransitionKernel = nullptr;

public:

	openCLSetup();
	~openCLSetup();
	int openCLMain(int);
	void setupData(int* A, int* B, int* D, int* E);
	void readKernel(char* kernelString, size_t& kernelSize, const char* kernalName);
	void setupDevice(cl_device_id& deviceID, cl_uint& numPlatforms, cl_uint& numDevices, cl_int& device, cl_platform_id* platforms);

	int UpdateStateOpenCL(GrassState* currentStates, int* aliveCounts, int sizeOfArrays);
	int StateTransitionsOpenCL(GrassState* currentStates, int* aliveCounts, unsigned int* grassStatesArray, int sizeOfArrays);
};

