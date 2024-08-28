#include "openCLSetup.h"
#include <vector>

/*	Basically, this is saying 'How big might my kernel be'?
	We're allocating a lot of memory to it:
	1MB - just in case our kernel is complex.
*/
#define MAX_KERNEL_SIZE (0x100000)


/* Define the size of our problem here... */
const float SIZE_OF_ARRAY = 1000000;

/* These variables are preamble to help us identify and store our
platform and device information. This will be important when it comes
to deploying our kernel. Note that this process is a bit more complicated
than it is in CUDA, simply because OpenCL supports so many different
vendors' devices - it requires additional steps to determine what suitable
platforms and devices are present. */

cl_device_id deviceID = NULL;
cl_uint numPlatforms, numDevices;
cl_int device;
cl_platform_id* platforms = NULL;

openCLSetup::openCLSetup()
{
}

openCLSetup::~openCLSetup()
{
	if (updateStateKernel)
		clReleaseKernel(updateStateKernel);
	if (updateStateProgram)
		clReleaseProgram(updateStateProgram);

	//if (stateTransitionKernel)
	//	clReleaseKernel(stateTransitionKernel);
	//if (stateTransitionProgram)
	//	clReleaseProgram(stateTransitionProgram);
}

int openCLSetup::openCLMain(int) {

	///* Declare a kernel string of max kernel size, and a variable
	//to store the actual kernel size */

	#pragma region READ THE UpdateState KERNEL
	// UpdateState KERNEL
	char* updateStateKernelString = new char[MAX_KERNEL_SIZE];
	size_t updateStateKernelSize;
	const char* updateStateKernelName = "UpdateStateKernel.cl";

	readKernel(updateStateKernelString, updateStateKernelSize, updateStateKernelName);

	if (!updateStateKernelString)
	{
		std::cout << "Critical error. updateStateKernel not read" << std::endl;
		return 0;
	}
	#pragma endregion 


	#pragma region READ THE StateTransition KERNEL
	//// StateTransition KERNEL
	//char* stateTransitionString = new char[MAX_KERNEL_SIZE];
	//size_t stateTransitionKernelSize;
	//const char* stateTransitionKernelName = "StateTransitions.cl";
	
//	readKernel(stateTransitionString, stateTransitionKernelSize, stateTransitionKernelName);
//
//	if (!stateTransitionString)
//	{
//		std::cout << "Critical error. StateTransition not read" << std::endl;
//		return 0;
//	}
	#pragma endregion 

	/* Call our device setup function. It's worth noting that our cl_int device
	isn't actually an identifier for which device is being used (most of the time).
	Instead, it's sort of a catch-all variable that we assign the results of
	OpenCL's internal functions to, to let us know if there's an error, similar
	to our use of cudaError_t variables in our CUDA code */

	setupDevice(deviceID, numPlatforms, numDevices, device, platforms);

	if (!numPlatforms || !numDevices)
	{
		std::cout << "Critical error - no devices/platforms detected.";
		return 0;
	}

	/* Like OpenGL, we create a context */
	context = clCreateContext(NULL, 1, &deviceID, NULL, NULL, &device);
	/* We then need to create a command queue. This binds the context to the deviceID
	we intend to deploy our OpenCL kernel to, and will bind the memory buffers we're
	about to declare. */
	command_queue = clCreateCommandQueue(context, deviceID, 0, &device);

	/* Like OpenGL shaders, our OpenCL kernel has to be turned into a
	'program' in order to be processed. This next command does this. Note
	that it relies on the size of the kernel - because it has to know how
	much of the string to read. */

	#pragma region CREATE THE UpdateState KERNEL PROGRAM

	//// Create a context
	//context = clCreateContext(NULL, 1, &deviceID, NULL, NULL, &device);
	//// Create a command queue
	//command_queue = clCreateCommandQueue(context, deviceID, 0, &device);
	// Create a program with the kernel source
	updateStateProgram = clCreateProgramWithSource(context, 1,
		(const char**)&updateStateKernelString, (const size_t*)&updateStateKernelSize, &device);

	if (updateStateProgram)
	{
		std::cout << "updateStateProgram program created." << std::endl;
	}
	else
	{
		std::cout << "Critical error. Program not created from kernel." << std::endl;
		return 0;
	}

	#pragma endregion 

	#pragma region CREATE THE StateTransition KERNEL PROGRAM
//
//	stateTransitionProgram = clCreateProgramWithSource(context, 1, 
//		(const char**)&stateTransitionString,(const size_t*)&stateTransitionKernelSize, &device);
//
//	if (stateTransitionProgram)
//	{
//		std::cout << "stateTransitionProgram program created." << std::endl;
//	}
//	else
//	{
//		std::cout << "Critical error. Program not created from kernel." << std::endl;
//		return 0;
//	}
//
	#pragma endregion 

	/* And then, as in OpenGL, the program has to be compiled or 'built'. */

	//https://registry.khronos.org/OpenCL/sdk/3.0/docs/man/html/clGetProgramBuildInfo.html

	device = clBuildProgram(updateStateProgram, 1, &deviceID, NULL, NULL, NULL);
	std::vector<char> buildLog;
	size_t logSize;
	clGetProgramBuildInfo(updateStateProgram, deviceID, CL_PROGRAM_BUILD_LOG, 0, nullptr, &logSize);

	buildLog.resize(logSize);
	clGetProgramBuildInfo(updateStateProgram, deviceID, CL_PROGRAM_BUILD_LOG, logSize, &buildLog[0], nullptr);
	std::cout << &buildLog[0] << std::endl;

	//device = clBuildProgram(stateTransitionProgram, 1, &deviceID, NULL, NULL, NULL);
	//check build success

	if (device != CL_SUCCESS)
	{
		std::cout << "Critical error. Failed to build program" << std::endl;
		return 0;
	}

	/* Our next step is to configure the kernel. This is analogous to matching
	buffers to shader inputs and outputs in OpenGL. In each line, we're indicating
	the order of the variable (0 = 1st, 1 = 2nd, 2 = 3rd, etc.), its size, and where
	it's located in memory.*/


	#pragma region LINKING THE KERNEL TO updateStateProgram

	updateStateKernel = clCreateKernel(updateStateProgram, "UpdateStateKernel", &device);

	if (!updateStateKernel)
	{
		std::cout << "Critical error. Failed to create kernel" << std::endl;
		return 0;
	}

	#pragma endregion

	#pragma region LINKING THE KERNEL TO stateTransitionProgram
//
//	stateTransitionKernel = clCreateKernel(stateTransitionProgram, "StateTransitions", &device);
//
//	if (!stateTransitionProgram)
//	{
//		std::cout << "Critical error. Failed to create kernel" << std::endl;
//		return 0;
//	}
//
	#pragma endregion



	/* Like OpenGL, there's a lot of tidying to do when we finish our OpenCL
	task. Like CUDA, most of this cleanup only occurs when you're closing down
	the software, as you'd likely want to reuse already-allocated arrays etc
	each frame of your simulation. */

	// This is done within the function:)

	return 0;
}

/* This function populates the data for our problem... */
//void openCLSetup::setupData(int* A, int* B, int* D, int* E)
//{
//	for (int i = 0; i < SIZE_OF_ARRAY; i++)
//	{
//		A[i] = SIZE_OF_ARRAY - i;
//		B[i] = i;
//		D[i] = SIZE_OF_ARRAY - i;
//		E[i] = i;
//	}
//}

/*	Below is the function which reads our Kernel.

	We declare a FILE pointer which we'll use basically to check
	that kernel.cl actually exists and to access it if it does.

	The char pointer (an array declared in main) will contain the
	text of the kernel, and the size_t kernelSize will store the
	number of characters in the kernel - notice we're passing
	by reference, so that the kernelSize declared in main is updated
	by the function, rather than a local variable in the readKernel
	function. */

void openCLSetup::readKernel(char* kernelString, size_t& kernelSize, const char* kernalName)
{
	FILE* kernelFile;

	kernelFile = fopen(kernalName, "r");
	if (!kernelFile) {
		std::cout << "Critical Error. Failed to load Kernel file." << std::endl;
		return;
	}

	kernelSize = fread(kernelString, 1, MAX_KERNEL_SIZE, kernelFile);
	fclose(kernelFile);

	std::cout << "Kernel " << kernalName << " successfully read." << std::endl;
}

/* Here we actually detect and populate our lists of devices and platforms.
Notice that there's a lot of passing by reference here - that isn't technically
necessary, we could just break this function down into multiple smaller functions
and return each piece of the puzzle one at a time. For the sake of brevity, however,
we've merged several operations into one function, and passed by reference to
ensure that our MAIN function can see all of the answers. */

void openCLSetup::setupDevice(cl_device_id& deviceID, cl_uint& numPlatforms, cl_uint& numDevices, cl_int& device, cl_platform_id* platforms)
{
	device = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (numPlatforms)
	{
		std::cout << numPlatforms << " platforms detected!" << std::endl;
	}
	else
	{
		std::cout << "No OpenCL platforms detected. How sad." << std::endl;
		return;
	}

	platforms = (cl_platform_id*)malloc(numPlatforms * sizeof(cl_platform_id));
	device = clGetPlatformIDs(numPlatforms, platforms, NULL);
	device = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 1, &deviceID, &numDevices);

	std::cout << numDevices << " devices discovered." << std::endl;
}

int openCLSetup::UpdateStateOpenCL(GrassState* currentStates, int* aliveCounts, int sizeOfArrays)
{
	// Ensure program and kernel are initialized
	if (!updateStateProgram || !updateStateKernel)
	{
		std::cout << "Critical error. Program or kernel not initialized" << std::endl;
		return 0;
	}

	/* Now we create buffers for our Device arrays - this is analogous
	to cudaMalloc operations. */
	cl_mem devCurrentStatesBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE,
		sizeOfArrays * sizeof(GrassState), NULL, &device);
	cl_mem devAliveCountsBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY,
		sizeOfArrays * sizeof(int), NULL, &device);

	/* This step is analogous to cudaMemcpyHostToDevice, taking the information from
	A and copying it to devABuffer, and the same with B and devBBuffer. */
	clEnqueueWriteBuffer(command_queue, devCurrentStatesBuffer, CL_TRUE, 0,
		sizeOfArrays * sizeof(GrassState), currentStates, 0, NULL, NULL);
	clEnqueueWriteBuffer(command_queue, devAliveCountsBuffer, CL_TRUE, 0,
		sizeOfArrays * sizeof(int), aliveCounts, 0, NULL, NULL);

	/* Our next step is to configure the kernel. This is analogous to matching
	buffers to shader inputs and outputs in OpenGL. In each line, we're indicating
	the order of the variable (0 = 1st, 1 = 2nd, 2 = 3rd, etc.), its size, and where
	it's located in memory.*/
	clSetKernelArg(updateStateKernel, 0, sizeof(cl_mem), (void*)&devCurrentStatesBuffer);
	clSetKernelArg(updateStateKernel, 1, sizeof(cl_mem), (void*)&devAliveCountsBuffer);

	/* The next line is our OpenCL equivalent to a kernel call.
	Because we've already bound the variables to it, in the same
	way we'd bind variables to a shader, we don't actually pass
	them in explicitly as arguments. Instead, we simply tell OpenCL
	which command queue to reference, which kernel we want to use,
	how many times we want it to run, and how we want it to
	batch the work.

	IMPORTANT: You can't assign more than 64 tasks per work unit.

	*/
	size_t totalTasks = sizeOfArrays; // Assuming one task per grass state
	//size_t tasksPerCU = 64; // Adjust as needed
	clEnqueueNDRangeKernel(command_queue, updateStateKernel, 1, NULL, &totalTasks,
		NULL, 0, NULL, NULL); //works it out by itself

	/* Now our analogue to cudaMemcpyDeviceToHost, taking information
	from devCBuffer and copying it to our array C. */
	clEnqueueReadBuffer(command_queue, devCurrentStatesBuffer, CL_TRUE, 0,
		sizeOfArrays * sizeof(GrassState), currentStates, 0, NULL, NULL);

	//remember to release the memory and delete
	clReleaseMemObject(devCurrentStatesBuffer);
	clReleaseMemObject(devAliveCountsBuffer);

	return 0;
}

//int openCLSetup::StateTransitionsOpenCL(GrassState* currentStates, int* aliveCounts, unsigned int* grassFlags, int sizeOfArrays) {
//	// Ensure program and kernel are initialized
//	if (!stateTransitionKernel || !stateTransitionProgram) {
//		std::cout << "Critical error. Program or kernel not initialized" << std::endl;
//		return 0;
//	}
//
//	// Like OpenGL, we create a context
//	cl_context context = clCreateContext(NULL, 1, &deviceID, NULL, NULL, &device);
//	// We then need to create a command queue. This binds the context to the deviceID
//	// we intend to deploy our OpenCL kernel to, and will bind the memory buffers we're
//	// about to declare.
//	cl_command_queue command_queue = clCreateCommandQueue(context, deviceID, 0, &device);
//
//	/* Now we create buffers for our Device arrays - this is analogous
//	to cudaMalloc operations. */
//	cl_mem devCurrentStatesBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE,
//		sizeOfArrays * sizeof(GrassState), NULL, &device);
//	cl_mem devAliveCountsBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY,
//		sizeOfArrays * sizeof(int), NULL, &device);
//	cl_mem devGrassFlagsBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE,
//		sizeOfArrays * sizeof(unsigned int), NULL, &device);
//
//	/* This step is analogous to cudaMemcpyHostToDevice, taking the information from
//	A and copying it to devABuffer, and the same with B and devBBuffer. */
//	clEnqueueWriteBuffer(command_queue, devCurrentStatesBuffer, CL_TRUE, 0,
//		sizeOfArrays * sizeof(GrassState), currentStates, 0, NULL, NULL);
//	clEnqueueWriteBuffer(command_queue, devAliveCountsBuffer, CL_TRUE, 0,
//		sizeOfArrays * sizeof(int), aliveCounts, 0, NULL, NULL);
//	clEnqueueWriteBuffer(command_queue, devGrassFlagsBuffer, CL_TRUE, 0,
//		sizeOfArrays * sizeof(unsigned int), grassFlags, 0, NULL, NULL);
//
//	/* Our next step is to configure the kernel. This is analogous to matching
//	buffers to shader inputs and outputs in OpenGL. In each line, we're indicating
//	the order of the variable (0 = 1st, 1 = 2nd, 2 = 3rd, etc.), its size, and where
//	it's located in memory.*/
//	clSetKernelArg(stateTransitionKernel, 0, sizeof(cl_mem), (void*)&devCurrentStatesBuffer);
//	clSetKernelArg(stateTransitionKernel, 1, sizeof(cl_mem), (void*)&devAliveCountsBuffer);
//	clSetKernelArg(stateTransitionKernel, 2, sizeof(cl_mem), (void*)&devGrassFlagsBuffer);
//
//	/* The next line is our OpenCL equivalent to a kernel call.
//	Because we've already bound the variables to it, in the same
//	way we'd bind variables to a shader, we don't actually pass
//	them in explicitly as arguments. Instead, we simply tell OpenCL
//	which command queue to reference, which kernel we want to use,
//	how many times we want it to run, and how we want it to
//	batch the work.
//
//	IMPORTANT: You can't assign more than 64 tasks per work unit.
//
//	*/
//	size_t totalTasks = sizeOfArrays; // Assuming one task per grass state
//	size_t tasksPerCU = 64; // Adjust as needed
//	clEnqueueNDRangeKernel(command_queue, stateTransitionKernel, 1, NULL, &totalTasks,
//		&tasksPerCU, 0, NULL, NULL);
//	clFinish(command_queue);
//
//	/* Now our analogue to cudaMemcpyDeviceToHost, taking information
//	from devCBuffer and copying it to our array C. */
//	clEnqueueReadBuffer(command_queue, devCurrentStatesBuffer, CL_TRUE, 0,
//		sizeOfArrays * sizeof(GrassState), currentStates, 0, NULL, NULL);
//	clEnqueueReadBuffer(command_queue, devGrassFlagsBuffer, CL_TRUE, 0,
//		sizeOfArrays * sizeof(unsigned int), grassFlags, 0, NULL, NULL);
//
//	// Remember to release the memory and delete
//	clReleaseMemObject(devCurrentStatesBuffer);
//	clReleaseMemObject(devAliveCountsBuffer);
//	clReleaseMemObject(devGrassFlagsBuffer);
//	clReleaseCommandQueue(command_queue);
//	clReleaseContext(context);
//
//	return 0;
//}
