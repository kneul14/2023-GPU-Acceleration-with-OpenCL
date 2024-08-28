#include "GameEngine.h"

vector<GameObject*> GameEngine::grass;
vector<GameObject*> GameEngine::gnomes;
std::vector<std::vector<GameObject*>> GameEngine::grid;
vector<Particle>* GameEngine::objectParticle;
int GameEngine::oldTimeSinceStart;
int GameEngine::newTimeSinceStart;
LoadTexture GameEngine::texture;
Camera GameEngine::camera;
openCLSetup GameEngine::openCL;
int GameEngine::xGridCells = 100;
int GameEngine::yGridCells = 100;
int GameEngine::gnomeCount = 10;
int GameEngine::percentAlive = 10;

bool start;
std::chrono::steady_clock::time_point startTime;
std::chrono::steady_clock::time_point lastUpdateTime;

int gridSize;

enum object { SPHERE, FUNGUS }; // VAO ids.
enum buffer { SPHERE_VERTICES, FUNGUS_VERTICIES }; // VBO ids.

struct Material
{
	vec4  ambRefl;
	vec4  difRefl;
	vec4  specRefl;
	vec4  emitCols;
	float shininess;
};

struct Light
{
	vec4  ambCols;
	vec4  difCols;
	vec4  specCols;
	vec4  coords;
};

static mat4 modelViewMat = mat4(1.0);
static mat4 projMat = mat4(1.0);
static mat3 normalMat = mat3(1.0);  //create normal matrix

static const vec4 globAmb = vec4(0.2, 0.2, 0.2, 1.0);
// Front and back material properties.
static const Material sphereFandB =
{
	vec4(1.0, 1.0, 1.0, 1.0),    // ambient reflection
	vec4(1.0, 1.0, 1.0, 1.0),    // diffuse reflection
	vec4(10.0, 10.0, 10.0, 1.0), // specular reflection
	vec4(0.0, 0.0, 0.0, 1.0),    // emit
	100.0f                       // shininess
};

static const Light light0 =
{
	vec4(0.0, 0.0, 0.0, 1.0),    // ambient
	vec4(1.0, 1.0, 1.0, 1.0),    // specular
	vec4(1.0, 1.0, 1.0, 1.0),    // diffuse
	vec4(1.0, 1.0, 0.0, 0.0)     // direction 
};

static const Light light1 =
{
	vec4(0.0, 0.0, 0.0, 1.0),    // ambient
	vec4(0.0, 0.0, 0.0, 1.0),    // specular
	vec4(0.0, 0.0, 0.0, 1.0),    // diffuse
	vec4(1.0, 1.0, 0.0, 0.0)     // direction 
};

static unsigned int
programId,
vertexShaderId,
fragmentShaderId,
modelViewMatLoc,
projMatLoc,
modelMatLoc,
viewMatLoc,
objectLoc,
sphereColour,
fungusColour,
buffer[10],      // VBO List
vao[10],	     // VAO List
texture[8],      // Texture List
fungusTexLoc;

static Fungus fungus;
float d = 1.0f;  // Camera position
int numFrames = 0;
int framesRate = 0;
float timeSinceFrameUpdate = 0;
double waitForTime = 2.0f; // 2 second time step.

struct GnomeMovementInfo {
	glm::vec2 direction;
	int timeStepsLeft;
};
unordered_map<GameObject*, GnomeMovementInfo> gnomeMovements; // Gnome movement info

void GameEngine::StartEngine()
{
	cout << "" << endl;
	cout << "" << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "                                        CONTROLS:                                       " << endl << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;

	cout << "   USE :         SPACEBAR          PLAY THE SIMULATION                                  " << endl << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	cout << "   USE :          W                MOVE CAMERA UP                                       " << endl;
	cout << "   USE :          A                MOVE CAMERA LEFT                                     " << endl;
	cout << "   USE :          S                MOVE CAMERA DOWN                                     " << endl;
	cout << "   USE :          D                MOVE CAMERA RIGHT                                    " << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	cout << "   USE :   UP    ARROW             ZOOM IN                                              " << endl;
	cout << "   USE :   DOWN  ARROW             ZOOM OUT                                             " << endl << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	cout << "   USE :         ESCAPE            STOP THE SIMULATION                                  " << endl << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;

	grid.resize(yGridCells);
	for (int i = 0; i < yGridCells; i++)
	{
		//grid[i].resize(xGridCells, i);					// ??? 
		//grid[i].resize(yGridCells, nullptr);				//x i cant be bigger:(
		grid[i].resize(xGridCells, nullptr);
	}

	for (int i = 0; i < yGridCells; i++)
	{
		for (int j = 0; j < xGridCells; j++)
		{
			grid[i][j] = new GameObject();
		}
	}

	gridSize = yGridCells * xGridCells;
	glutMainLoop();
}

void GameEngine::Initialise()
{
	const int arraySize = 5;
	const int a[arraySize] = { 1, 2, 3, 4, 5 };
	const int b[arraySize] = { 10, 20, 30, 40, 50 };
	int c[arraySize] = { 0 };

	// Sets up the devices, and kernels:)
	openCL.openCLMain(mainFunc);

	cout << "Please enter the width(x) of the garden between 100 and 1000 to be generated." << endl;
	cin >> xGridCells;
	cout << "Please enter the height(y) of the garden between 100 and 1000 to be generated." << endl;
	cin >> yGridCells;
	cout << "Please enter what percent of alive grass(fungi) you want." << endl;
	cin >> percentAlive;
	cout << "Please enter the amount of gnomes in the garden between 10 and 1000 to be generated." << endl;
	cin >> gnomeCount;

	startTime = std::chrono::steady_clock::now();
	lastUpdateTime = startTime;
}

void FPSCalc() {
	//FPS COUNTER
	if (glutGet(GLUT_ELAPSED_TIME) - timeSinceFrameUpdate > 1000) {
		timeSinceFrameUpdate = glutGet(GLUT_ELAPSED_TIME);
		char windowTitle[169];
		sprintf_s(windowTitle, "Game of life Sim -------------- frame rate is: %d", numFrames);
		glutSetWindowTitle(windowTitle);
		numFrames = 0;
	}
	numFrames++;
}

void GameEngine::UpdateGame(void)
{
	FPSCalc();

	// Timestep game logic.
	bool timeElapsed = false;

	if (start) {
		std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsedTime = currentTime - lastUpdateTime;

		if (elapsedTime.count() >= waitForTime) {
			std::cout << "Game of Life in session..." << std::endl;

			lastUpdateTime = currentTime;
			timeElapsed = true;

			for (int i = 0; i < gnomes.size(); i++)
			{
				//gnomes[i]->particlePTR->position.x = gnomes[i]->particlePTR->position.x + 1;
				GnomeMovement(grass, gnomes);
			}
			GnomeInteractor(grass, gnomes);  // Because this rule works irrespective of the rules it should be called first in the loop

			GrassRules(grass);
			texture.ChangeTextureOnGrass(grass);
			numFrames++;
			glutPostRedisplay();
		}
	}

	glutPostRedisplay();
}

#pragma region Grass State handling 
void UpdateState(GrassState& currentState, int aliveCount) {
	switch (currentState) {
	case aliveWith2or3Neighbors:
		if (!(aliveCount == 2 || aliveCount == 3)) {
			currentState = otherAliveCells;
		}
		break;
	case deadWith3Neighbors:
		if (aliveCount != 3) {
			currentState = otherDeadCells;
		}
		break;
	default:
		break;
	}
}

int GameEngine::GrassRules(vector<GameObject*> grassObjs) {
	/*
		1. Any Alive cell with two or three live neighbours will survive into the next time step.
		2. Any Dead cell with three live neighbours will become Alive in the next time step.
		3. All other Alive cells will become Dead in the next time step.
		4. All other Dead cells remain Dead in the next time step.
	*/

	// I want the percentile of the rows to find out if it is at the start or end of a row
	//

	int gridX = GameEngine::xGridCells;
	int gridY = GameEngine::yGridCells;

	int grassAmount = grassObjs.size();


	GrassState* currentStateArray = new GrassState[grassAmount];


	int* aliveArray = new int[grassAmount];

	// Populate grassFlagsArray with initial flag values
	for (int i = 0; i < grassAmount; ++i) {
		aliveArray[i] = 0;
	}

	int loop = 0;

	for (int i = 0; i < grassAmount; i++) {

		int aliveCount = 0; // Initialize aliveCount here

		if (grassObjs[i]->HasFlag(alive)) {
			currentStateArray[i] = startingStateAlive;
		}
		else
		{
			currentStateArray[i] = startingStateDead;
		}

		// For working out if there is a row above a tile
		int k = grassAmount + i;
		int j = grassAmount + gridX;


		// To the right in the grid.
		if (i % gridX != gridX - 1 && grassObjs[i + 1]->HasFlag(alive)) {					// smaller than or equal too
			aliveCount++;
		}
		// To the left in the grid.
		if (i % gridX != 0 && grassObjs[i - 1]->HasFlag(alive))
		{
			aliveCount++;
		}
		// Tile above.
		if (i - gridX >= 0 && grassObjs[i - gridX]->HasFlag(alive))
		{
			aliveCount++;
			int q; // for debugging
		}
		// Tile beneath.
		if (i + gridX < grassObjs.size() && grassObjs[i + gridX]->HasFlag(alive)) {      // if i is bigger than  
			aliveCount++;
		}
		// Top-left diagonal
		if (i - gridX - 1 >= 0 && grassObjs[i - gridX - 1]->HasFlag(alive)) {
			aliveCount++;
		}
		// Top-right diagonal
		if (i - gridX + 1 >= 0 && i % gridX != gridX - 1 && grassObjs[i - gridX + 1]->HasFlag(alive)) {
			aliveCount++;
		}
		// Bottom-left diagonal
		if (i + gridX - 1 < grassObjs.size() && i % gridX != 0 && grassObjs[i + gridX - 1]->HasFlag(alive)) {
			aliveCount++;
		}
		// Bottom-right diagonal
		if (i + gridX + 1 < grassObjs.size() && i % gridX != gridX - 1 && grassObjs[i + gridX + 1]->HasFlag(alive)) {
			aliveCount++;
		}


#if 0       // cpu func for testing
		UpdateState(currentStateArray[i], aliveCount);
	}
#else
		aliveArray[i] = aliveCount;

	}

	openCL.UpdateStateOpenCL(currentStateArray, aliveArray, grassAmount);
#endif

#if 1
	for (int j = 0; j < grassAmount; j++)
	{
		//Applies the transitions between the states
		switch (currentStateArray[j]) {
		case otherAliveCells:
			grassObjs[j]->RemoveFlag(alive);
			grassObjs[j]->AddFlag(dead);
			break;
		case deadWith3Neighbors:
			grassObjs[j]->RemoveFlag(dead);
			grassObjs[j]->AddFlag(alive);
			// Do nothing,dead
			break;
		default:
			// Do nothing
			break;
		}
	}
#else

	// Initialize grassFlagsArray
	unsigned int* grassFlagsArray = new unsigned int[grassAmount];

	// Populate grassFlagsArray with initial flag values
	for (int i = 0; i < grassAmount; ++i) {
		grassFlagsArray[i] = grassObjs[i]->flag; // gets the starting state
	}

	//cudaStatus = cudaClass.UpdateStateCuda(currentStateArray, aliveArray, grassAmount);
	openCL.StateTransitionsOpenCL(currentStateArray, aliveArray, grassFlagsArray, grassAmount);

	// Populate grassFlagsArray with initial flag values
	for (int i = 0; i < grassAmount; ++i) {
		grassObjs[i]->flag = grassFlagsArray[i]; // gets the starting state
	}

	delete[] grassFlagsArray;
#endif
	delete[] currentStateArray;

	return 0;
}

void GameEngine::GnomeInteractor(vector<GameObject*> grassObjs, vector<GameObject*> gnomes)
{
	/*
		If a Gnome passes over an Alive cell, they will apply Fungicide to that cell.
		This will make the cell Dead in the next time step, and will prevent the cell
		from becoming Alive(irrespective of the rules above) for a further four time steps

		* Alive cells should appear as fungus of some kind (toadstools, for example).
		* Dead cells should appear as grass.
		* Cells exposed to Fungicide should appear as brown earth during the next timestep.
		* Gnomes should appear as Gnomes.
	*/

	const int xGridCells = GameEngine::xGridCells;
	const int yGridCells = GameEngine::yGridCells;

	for (GameObject* gnome : gnomes) {
		glm::vec2 gnomePosition = glm::round(gnome->particlePTR->position);

		if (gnomePosition.x >= 0 && gnomePosition.x < xGridCells &&
			gnomePosition.y >= 0 && gnomePosition.y < yGridCells) {

			// Convert 2D grid coordinates to 1D line/index
			int gnomeIndex = static_cast<int>(gnomePosition.x) + static_cast<int>(gnomePosition.y) * xGridCells;

			// Checks if the current cell occupied by the gnome contains an Alive cell
			if (gnomeIndex >= 0 && gnomeIndex < grassObjs.size() && grassObjs[gnomeIndex]->HasFlag(alive)) {
				// Apply Fungicide to the Alive cell
				if (grassObjs[gnomeIndex]->HasFlag(alive))
				{
					grassObjs[gnomeIndex]->ApplyFungicide();
					//grassObjs[gnomeIndex]->RemoveFlag(alive);
					//grassObjs[gnomeIndex]->AddFlag(infected);
					//grassObjs[gnomeIndex]->timeStepCounter = 5; // Set a counter for preventing the cell from becoming Alive for four time steps
				}
			}
		}

	}

	for (GameObject* grass : grassObjs) {
		if (grass->HasFlag(infected)) {
			// If the counter = 0, remove the infected flag, add  dead flag
			if (grass->timeStepCounter <= 0) {
				grass->HandleInfection();
			}
			else {
				grass->timeStepCounter--;
			}
		}
	}
}
#pragma endregion

float GnomeStayInGrid(float value, float min, float max)
{
	return (value < min) ? min : (value > max) ? max : value;
}

glm::vec2 GetRandomDirection() {
	int randomDirectionIndex = rand() % 4;
	glm::vec2 randomDirection(0.0f);
	switch (randomDirectionIndex) {
	case 0: // Up
		randomDirection.y = 1.0f;
		break;
	case 1: // Right
		randomDirection.x = 1.0f;
		break;
	case 2: // Down
		randomDirection.y = -1.0f;
		break;
	case 3: // Left
		randomDirection.x = -1.0f;
		break;
	}
	return randomDirection;
}

void GameEngine::GnomeMovement(vector<GameObject*> grassBlocks, vector<GameObject*> gnomes) {

	for (int i = 0; i < gnomes.size(); i++) {
		GameObject* gnome = gnomes[i];

		// If gnome's movement information doesn't exist
		//  initialize it
		if (gnomeMovements.find(gnome) == gnomeMovements.end()) {
			glm::vec2 randomDirection = GetRandomDirection();
			gnomeMovements[gnome] = { randomDirection, 2 }; // Two time steps
		}

		// Move the gnome 
		GnomeMovementInfo& movementInfo = gnomeMovements[gnome];
		gnome->GetParticle()->position += movementInfo.direction;

		// stay within the grid bounds
		gnome->GetParticle()->position.x = GnomeStayInGrid(gnome->GetParticle()->position.x, 0.0f, static_cast<float>(grid.size() - 1));
		gnome->GetParticle()->position.y = GnomeStayInGrid(gnome->GetParticle()->position.y, 0.0f, static_cast<float>(grid[0].size() - 1));

		// Show on grid
		int gridX = static_cast<int>(gnome->GetParticle()->position.x);
		int gridY = static_cast<int>(gnome->GetParticle()->position.y);
		grid[gridX][gridY] = gnome;



		movementInfo.timeStepsLeft--;

		// If timsteps = 0, choose a new random direction
		if (movementInfo.timeStepsLeft <= 0) {
			movementInfo.direction = GetRandomDirection();
			movementInfo.timeStepsLeft = 2; // Two timesteps
		}
	}
}

void GameEngine::Setup()
{
	int scrWidth = 35, scrHeight = 35, randNum = 1;

	objectParticle = new std::vector<Particle>();

	glClearColor(1.5, 0.5, .5, 0.0); // Pink background
	//glClearColor(0.5, 0.5, 0.5, 0.0); //Grey background
	glEnable(GL_DEPTH_TEST);

	// Create shader program executable.
	#pragma region Shader Setup
	vertexShaderId = setShader("vertex", "vertexShader.glsl");
	fragmentShaderId = setShader("fragment", "fragmentShader.glsl");
	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glUseProgram(programId);

#pragma endregion

	// Codes for OpenGL lighting
	#pragma region Lighting
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.ambRefl"), 1, &sphereFandB.ambRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.difRefl"), 1, &sphereFandB.difRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.specRefl"), 1, &sphereFandB.specRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.emitCols"), 1, &sphereFandB.emitCols[0]);
	glUniform1f(glGetUniformLocation(programId, "sphereFandB.shininess"), sphereFandB.shininess);

	glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

	glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1, &light0.ambCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1, &light0.difCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1, &light0.specCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);
#pragma endregion


	grass.reserve(1000000);						// Allocates enough memory for the grass and their colliders
	gnomes.reserve(1000);							// Allocates enough memory for the gnomes and their colliders
	objectParticle->reserve(1000000 + 1000);		// Allocates enough memory for the physics particles.

	Particle* particlePTR = new Particle(2.0f, 0.5f, glm::vec2(13.5f, 14.5f));

	int tempFloatX;

	GLuint fungusID = texture.Texture("Textures/funguy.png");
	GLuint grassID = texture.Texture("Textures/grass.png");
	GLuint gnomeID = texture.Texture("Textures/nome.png");

	///// Create fungi ////////////
	#pragma region Fungi Instances:)
	for (int y = 0; y < yGridCells; y++) //x axis
	{
		for (int x = 0; x < xGridCells; x++) //y axis
		{
			tempFloatX = rand() % 100;

			particlePTR = new Particle(2.0f, 0.0f, glm::vec2(x * 1, y * 1));
			objectParticle->emplace_back(*particlePTR);
			if (tempFloatX <= percentAlive)
			{
				GameObject* fungusObj = new Fungus(objectParticle->back().GetFriction(), objectParticle->back().GetMass(), glm::vec3(1.0, 0.0, 0.0), &objectParticle->back());
				fungusObj->texID = fungusID;
				grass.emplace_back(fungusObj);
				fungusObj->AddFlag(alive);
			}
			else if (tempFloatX > percentAlive)
			{
				GameObject* fungusObj = new Fungus(objectParticle->back().GetFriction(), objectParticle->back().GetMass(), glm::vec3(0.0, 1.0, 0.0), &objectParticle->back());
				fungusObj->texID = grassID;
				grass.emplace_back(fungusObj);
				fungusObj->AddFlag(dead);
			}

			tempFloatX = rand() % 1;
		}
	}
#pragma endregion

	/// Create Gnomes ////////////
	#pragma region Gnomes Instances:)
	for (int i = 0; i < gnomeCount; i++)
	{
		int randomX = rand() % xGridCells;
		int randomY = rand() % yGridCells;

		particlePTR = new Particle(2.0f, 0.0f, glm::vec2(randomX, randomY));
		objectParticle->emplace_back(*particlePTR);

		GameObject* gnomeObj = new Gnome(objectParticle->back().GetFriction(), objectParticle->back().GetMass(), glm::vec3(1.0, 0.0, 1.0), &objectParticle->back());
		gnomeObj->texID = gnomeID;
		gnomes.emplace_back(gnomeObj);
		gnomeObj->AddFlag(gnome);
	}
#pragma endregion

	///// Create a fungus  ///////////
	glGenVertexArrays(1, &vao[FUNGUS]);
	glGenBuffers(1, &buffer[FUNGUS_VERTICIES]);

	// Obtain projection matrix uniform location and set value.
	projMatLoc = glGetUniformLocation(programId, "projMat");   //uniform mat4 projMat;
	camera.ProjectionCamera(projMatLoc);							///change to projcam

	// Obtain modelview matrix uniform and object uniform locations.
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");   //uniform mat4 modelViewMat;
	modelMatLoc = glGetUniformLocation(programId, "modelMat");   //uniform mat4 modelViewMat;
	viewMatLoc = glGetUniformLocation(programId, "viewMat");
	objectLoc = glGetUniformLocation(programId, "object");  //uniform uint object;
	sphereColour = glGetUniformLocation(programId, "sphereColour");
	fungusColour = glGetUniformLocation(programId, "fungusColour");
	fungusTexLoc = glGetUniformLocation(programId, "fungusTex");

	glm::mat4 viewMat = camera.WhichCamera();
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, value_ptr(viewMat));
}

void GameEngine::DrawGame(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region GameObject Draw 	
	// Draw gnomes, call this first so they are visually on top of the grass
	for (int i = 0; i < gnomes.size(); ++i)
	{
		glUniform1ui(objectLoc, FUNGUS);  //if (object == SPHERE)
		gnomes[i]->Draw(modelMatLoc, fungusColour, fungusTexLoc);
	}

	// Draw grass
	for (int i = 0; i < grass.size(); ++i)
	{
		glUniform1ui(objectLoc, FUNGUS);  //if (object == FUNGUS)

		grass[i]->Draw(modelMatLoc, fungusColour, fungusTexLoc);

		// If i want colours this works lmao
		//glUniform1ui(objectLoc, SPHERE);  //if (object == SPHERE)
		//gnome[i]->Draw(modelMatLoc, sphereColour);
	}
#pragma endregion

	glutSwapBuffers();
}

void GameEngine::CleanUpEngine()
{
	for (int i = 0; i < grass.size(); ++i)
	{
		delete grass[i];
	}

	for (int i = 0; i < gnomes.size(); ++i)
	{
		delete gnomes[i];
	}
}

void GameEngine::ResizeWindow(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

void GameEngine::InitEngine(int argc, char** argv, const char* windowTitle, int width, int height) //Basically the Main
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(350, 0);
	glutCreateWindow("Game of Life Sim");


	glutDisplayFunc(DrawGame);
	glutReshapeFunc(ResizeWindow);
	glutKeyboardFunc([](unsigned char key, int x, int y)
	{
		GameObject::keys[key] = true;
		switch (key)
		{
		case 27:
			exit(0);
			break;
		case 49:
			std::cout << "Camera 1" << std::endl;
			break;
		case 'w':
			camera.CameraMovement(vec3(0, 0, 10), viewMatLoc);
			break;
		case 'a':
			camera.CameraMovement(vec3(-10, 0, 0), viewMatLoc);
			break;
		case 's':
			camera.CameraMovement(vec3(0, 0, -10), viewMatLoc);
			break;
		case 'd':
			camera.CameraMovement(vec3(10, 0, 0), viewMatLoc);
			break;
		case 32:
			start = true;
			break;
		default:
			break;
		}
		});
	glutKeyboardUpFunc([](unsigned char key, int x, int y)
		{
			GameObject::keys[key] = false;
		});
	glutSpecialFunc([](int key, int x, int y)
		{
			GameObject::specialKeys[key] = true;
			switch (key)
			{
			case GLUT_KEY_UP:
				camera.zoom -= 0.1;
				camera.ProjectionCamera(projMatLoc);
				break;
			case GLUT_KEY_DOWN:
				camera.zoom += 0.1;
				camera.ProjectionCamera(projMatLoc);
				break;
			case GLUT_KEY_LEFT:
				break;
			case GLUT_KEY_RIGHT:
				break;
			default:
				break;
			}
			glutPostRedisplay();
		});
	glutSpecialUpFunc([](int key, int x, int y)
		{
			GameObject::specialKeys[key] = false;
		});


	glutIdleFunc(UpdateGame);

	glewExperimental = GL_TRUE;
	glewInit();

	GameEngine::Setup();
}