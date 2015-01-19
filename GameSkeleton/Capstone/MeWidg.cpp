#include "MeWidg.h"
#include "DebugMenu.h"

#define R_CS(a) reinterpret_cast<char*>(&a), sizeof(a)
#define R_C(t,a) reinterpret_cast<t>(a)

MeWidg::MeWidg()
{
	currentScene=-1;
	//QWidget* objectSelectorWidget;

	QVBoxLayout* mainLayout=new QVBoxLayout();
	setLayout(mainLayout);

#if EDITOR_ON

	QHBoxLayout* setUpLayout=new QHBoxLayout();
	
	setWindowTitle("Game Creator");

	QHBoxLayout *game =new QHBoxLayout();
#endif
	
	gameGLWindow=new MyGLWindow();
	
#if EDITOR_ON
	debugMenu.initialize(setUpLayout);
	debugMenu.addLayout("World");
	
	mb=new QMenuBar();
	mb->setMaximumHeight(20);
	
	QMenu* fileMenu = mb->addMenu("File");

	sceneNum=0;
	
	QAction* action;
	fileMenu->addAction(action = new QAction("Load Project", this));
	//action->setShortcut(QKeySequence::Open);
	connect(action, SIGNAL(triggered()), this, SLOT(loadProject()));
	
	fileMenu->addAction(action = new QAction("Save Project", this));
	//action->setShortcuts(QKeySequence::Save);
	connect(action, SIGNAL(triggered()), this, SLOT(saveProject()));
	
	fileMenu->addSeparator();
	
	fileMenu->addAction(action = new QAction("Load level", this));
	//action->setShortcuts(QKeySequence::Save);
	connect(action, SIGNAL(triggered()), this, SLOT(loadScene()));
	
	fileMenu->addAction(action = new QAction("Save Level", this));
	//action->setShortcuts(QKeySequence::Save);
	connect(action, SIGNAL(triggered()), this, SLOT(saveScene()));

	fileMenu->addSeparator();

	fileMenu->addAction(action=new QAction("Export Game", this));
	connect(action, SIGNAL(triggered()), this, SLOT(exportGame()));
	
	fileMenu->addSeparator();
	
	fileMenu->addAction(action = new QAction("Close", this));
	//action->setShortcuts(QKeySequence::Save);
	//connect(action, SIGNAL(triggered()), this, SLOT(saveNative()));
	
	QMenu* objectMenu=mb->addMenu("Objects");
	objectMenu->addAction(action=new QAction("Load Model", this));
	//action->setShortcut(QKeySequence::Open);
	connect(action, SIGNAL(triggered()), this, SLOT(loadModel()));
	
	objectMenu->addAction(action=new QAction("Add Ambient Light", this));
	//action->setShortcut(QKeySequence::Open);
	connect(action, SIGNAL(triggered()), this, SLOT(loadALight()));

	objectMenu->addAction(action=new QAction("Add Diffuse Light", this));
	//action->setShortcut(QKeySequence::Open);
	connect(action, SIGNAL(triggered()), this, SLOT(loadDLight()));

	objectMenu->addAction(action=new QAction("Add Instance Object", this));
	connect(action, SIGNAL(triggered()), this, SLOT(loadInstancedModel()));

	projectMenu=mb->addMenu("Project Scenes");
	projectMenu->addAction(action=new QAction("Add Scene", this));
	connect(action, SIGNAL(triggered()), this, SLOT(addScene()));

	projectMenu->addSeparator();

	mainLayout->addWidget(mb);
#else if EDITOR_OFF

#endif

#if EDITOR_ON
	game -> addWidget(gameGLWindow,1, 0);
	setUpLayout -> addLayout(game);
#else
	mainLayout->addWidget(gameGLWindow, 0,0);
#endif
	gameGLWindow->setMinimumHeight(600);
	gameGLWindow->setMinimumWidth(500);

#if EDITOR_ON
	mainLayout->addLayout(setUpLayout, 1);
	firstRun=false;
#else
	firstRun=true;
#endif

	connect(&myTimer, SIGNAL(timeout()), this, SLOT(myUpdate()));
	myTimer.start(16);
}

void MeWidg::myUpdate()
{
	if(!gameGLWindow->switchWorldON)
	{
#if EDITOR_ON
		debugMenu.update();
#endif
	}

	if(GetAsyncKeyState(VK_ESCAPE) && !noToggle)
	{
		noToggle=true;
#if EDITOR_ON
		debugMenu.toggleVisibility();
#endif
	}
	else if(!GetAsyncKeyState(VK_ESCAPE) && noToggle)
	{
		noToggle=false;
	}

	if(gameGLWindow->switchScene>=0)
	{
		sceneClicked(gameGLWindow->switchScene);
		gameGLWindow->switchScene=-1;
	}

	if(firstRun)
	{
		loadGameProject("Game.data");
		sceneClicked(0);
		firstRun=false;
	}
}

void MeWidg::loadModel()
{
	gameGLWindow->loadModel();
}

void MeWidg::loadALight()
{
	gameGLWindow->addAmbientLight();
}

void MeWidg::loadDLight()
{
	gameGLWindow->addDiffuseLight();
}

void MeWidg::loadInstancedModel()
{
	gameGLWindow->loadInstantModel();
}

void MeWidg::saveScene()
{
	QString targetSceneName=QFileDialog::getSaveFileName(this, "Save File", ".gcDT", "*.gcDT");
	if(targetSceneName=="")
		return;

	std::string sceneName=targetSceneName.toLocal8Bit().constData();

	gameGLWindow->saveScene(sceneName);

	if(currentScene>=0)
		updateScene(sceneName);
}

void MeWidg::updateScene(std::string sceneName)
{
	delete tempBuffer[currentScene];
	std::ifstream input(sceneName, std::ios::binary | std::ios::in);
	std::streampos begin, end;
	input.seekg(0, input.end);
	end=input.tellg();
	input.seekg(0, input.beg);
	begin=input.tellg();
	size_t numBytes=(end-begin);
	tempBuffer[currentScene]=new char[numBytes];
	input.read(tempBuffer[currentScene], numBytes);

	input.close();

	sceneClicked(currentScene);
}

void MeWidg::exportGame()
{
#if EDITOR_ON
	QString targetSceneName=QFileDialog::getExistingDirectory(this, "Save Game");

	if(targetSceneName=="")
		return;

	std::string gameSaveName=targetSceneName.toLocal8Bit().constData();
	gameSaveName+="\\Game";

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	std::string gameSetup=targetSceneName.toLocal8Bit().constData();
	gameSetup+="\\Data";

	_mkdir(gameSetup.c_str());

	bool ok;
	QString textName=QInputDialog::getText(this, tr("Name of Your Game"), tr("Name:"), QLineEdit::Normal, QDir::home().dirName(), &ok);
	
	if(!ok || textName.isEmpty())
		return;

	std::string shortcutName=targetSceneName.toLocal8Bit().constData();
	std::string tempShortcut=textName.toLocal8Bit().constData();
	shortcutName+='\\'+tempShortcut;

	std::string executableName=targetSceneName.toLocal8Bit().constData();
	executableName+="\\Data\\Game.exe";

	std::string sceneName=targetSceneName.toLocal8Bit().constData();
	sceneName+="\\Data\\Game.data";

	wchar_t buffer[4096];
	GetModuleFileName(NULL, buffer, 4096);
	std::wstring temp(buffer);
	std::string::size_type finalPath=std::string(temp.begin(), temp.end()).find_last_of("\\/");
	std::string fPath=std::string(temp.begin(), temp.end()).substr(0, finalPath);
	fPath+="\\Export/*";

	wchar_t* wString=new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, fPath.c_str(), -1, wString, 4096);

	if((hFind=FindFirstFile((wString), &FindFileData))==INVALID_HANDLE_VALUE)
		return;

	do
	{
		std::wstring tempName=FindFileData.cFileName;
		const std::string file_name(tempName.begin(), tempName.end());
		const std::string full_file_name=fPath.substr(0, fPath.size()-2)+'\\'+file_name;

		if(file_name.size()>4 && file_name.substr(file_name.size()-4, file_name.size())==".dll")
		{
			std::wstring pass(full_file_name.begin(), full_file_name.end());
			std::wstring passTwo(gameSetup.begin(), gameSetup.end());
			passTwo+='\\';
			passTwo+=tempName;
			std::string cmd="copy ";
			std::string tempPass='\"'+std::string(pass.begin(), pass.end())+'\"';
			pass=std::wstring(tempPass.begin(), tempPass.end());
			std::string tempPassTwo='\"'+std::string(passTwo.begin(), passTwo.end())+'\"';
			passTwo=std::wstring(tempPassTwo.begin(), tempPassTwo.end());
			cmd+=std::string(pass.begin(), pass.end())+' ';
			cmd+=std::string(passTwo.begin(), passTwo.end());
			system(cmd.c_str());
		}

		if(file_name.size()>7 && file_name=="Game.exe")
		{
			std::wstring pass(full_file_name.begin(), full_file_name.end());
			std::wstring passTwo(gameSetup.begin(), gameSetup.end());
			passTwo+='\\';
			passTwo+=tempName;
			std::string cmd="copy ";
			std::string tempPass='\"'+std::string(pass.begin(), pass.end())+'\"';
			pass=std::wstring(tempPass.begin(), tempPass.end());
			std::string tempPassTwo='\"'+std::string(passTwo.begin(), passTwo.end())+'\"';
			passTwo=std::wstring(tempPassTwo.begin(), tempPassTwo.end());
			cmd+=std::string(pass.begin(), pass.end())+' ';
			cmd+=std::string(passTwo.begin(), passTwo.end());
			system(cmd.c_str());
		}
	}while(FindNextFile(hFind, &FindFileData));

	std::string filesPath=fPath.substr(0, fPath.size()-9)+"\\Files/*";
	std::string shaderPath=fPath.substr(0, fPath.size()-9)+"\\Shaders/*";

	MultiByteToWideChar(CP_ACP, 0, filesPath.c_str(), -1, wString, 4096);

	if((hFind=FindFirstFile((wString), &FindFileData))==INVALID_HANDLE_VALUE)
		return;

	gameSetup=gameSetup.substr(0, gameSetup.size()-5)+"\\Files";
	_mkdir(gameSetup.c_str());
	do
	{
		std::wstring tempName=FindFileData.cFileName;
		const std::string file_name(tempName.begin(), tempName.end());
		const std::string full_file_name=filesPath.substr(0, filesPath.size()-2)+'\\'+file_name;

		if(file_name==".")
			continue;
		if(file_name=="..")
			continue;

		std::wstring pass(full_file_name.begin(), full_file_name.end());
		std::wstring passTwo(gameSetup.begin(), gameSetup.end());
		passTwo+='\\';
		passTwo+=tempName;
		std::string cmd="copy ";
		std::string tempPass='\"'+std::string(pass.begin(), pass.end())+'\"';
		pass=std::wstring(tempPass.begin(), tempPass.end());
		std::string tempPassTwo='\"'+std::string(passTwo.begin(), passTwo.end())+'\"';
		passTwo=std::wstring(tempPassTwo.begin(), tempPassTwo.end());
		cmd+=std::string(pass.begin(), pass.end())+' ';
		cmd+=std::string(passTwo.begin(), passTwo.end());
		system(cmd.c_str());
	}while(FindNextFile(hFind, &FindFileData));

	MultiByteToWideChar(CP_ACP, 0, shaderPath.c_str(), -1, wString, 4096);

	if((hFind=FindFirstFile((wString), &FindFileData))==INVALID_HANDLE_VALUE)
		return;

	gameSetup=gameSetup.substr(0, gameSetup.size()-6)+"\\Shaders";
	_mkdir(gameSetup.c_str());

	do
	{
		std::wstring tempName=FindFileData.cFileName;
		const std::string file_name(tempName.begin(), tempName.end());
		const std::string full_file_name=shaderPath.substr(0, shaderPath.size()-2)+'\\'+file_name;

		if(file_name==".")
			continue;
		if(file_name=="..")
			continue;

		std::wstring pass(full_file_name.begin(), full_file_name.end());
		std::wstring passTwo(gameSetup.begin(), gameSetup.end());
		passTwo+='\\';
		passTwo+=tempName;
		std::string cmd="copy ";
		std::string tempPass='\"'+std::string(pass.begin(), pass.end())+'\"';
		pass=std::wstring(tempPass.begin(), tempPass.end());
		std::string tempPassTwo='\"'+std::string(passTwo.begin(), passTwo.end())+'\"';
		
		passTwo=std::wstring(tempPassTwo.begin(), tempPassTwo.end());
		cmd+=std::string(pass.begin(), pass.end())+' ';
		cmd+=std::string(passTwo.begin(), passTwo.end());
		system(cmd.c_str());
	}while(FindNextFile(hFind, &FindFileData));

	gameSetup=gameSetup.substr(0, gameSetup.size()-8);

	std::ofstream out(sceneName, std::ios::binary | std::ios::out);

	int writeData;

	writeData=sceneNum;
	out.write(R_CS(writeData));

	for(int i=0; i<sceneNum; i++)
	{
		writeData=tempNumBytes[i];
		out.write(R_CS(writeData));

		out.write(tempBuffer[i], writeData);
	}

	out.close();

	shortcutName+=".lnk";

	//Target_file is the name of the executable I want the shortcut I create for the exported game
	//to run
	CoInitialize(NULL);
	IShellLink* pShellLink;
	HRESULT hres=CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_ALL,IID_IShellLink, (void**)&pShellLink);
	std::wstring exeN(executableName.begin(), executableName.end());
	pShellLink->SetPath(exeN.c_str());
	IPersistFile *pPersistFile;
	hres=pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);
	std::wstring shortN(shortcutName.begin(), shortcutName.end());
	hres=pPersistFile->Save(shortN.c_str(), TRUE);
	pPersistFile->Release();
	CoUninitialize();
#endif
}

void MeWidg::saveProject()
{
	QString targetSceneName=QFileDialog::getSaveFileName(this, "Save File", ".ProjDT", "*.ProjDT");

	if(targetSceneName=="")
		return;

	std::string sceneName=targetSceneName.toLocal8Bit().constData();

	std::ofstream out(sceneName, std::ios::binary | std::ios::out);

	int writeData;

	writeData=sceneNum;
	out.write(R_CS(writeData));

	for(int i=0; i<sceneNum; i++)
	{
		writeData=tempNumBytes[i];
		out.write(R_CS(writeData));

		out.write(tempBuffer[i], writeData);
	}

	out.close();
}

void MeWidg::loadGameProject(std::string projName)
{
#if EDITOR_OFF
	wchar_t buffer[4096];
	GetModuleFileName(NULL, buffer, 4096);
	std::wstring tempSTR(buffer);
	std::string::size_type finalPath=std::string(tempSTR.begin(), tempSTR.end()).find_last_of("\\/");
	std::string fPath=std::string(tempSTR.begin(), tempSTR.end()).substr(0, finalPath);
	fPath+="\\"+projName;
	std::ifstream input(fPath, std::ios::binary | std::ios::in);
#else
	std::ifstream input(projName, std::ios::binary | std::ios::in);
#endif

	std::streampos begin, end;
	input.seekg(0, input.end);
	end=input.tellg();
	input.seekg(0, input.beg);
	begin=input.tellg(); 
	size_t numBytes=(end-begin);
	char* temp=new char[numBytes];
	input.read(temp, numBytes);

	int displacement=0;

	int sceneNum=*reinterpret_cast<int*>(temp+displacement);
	displacement+=sizeof(int);

	for(int i=0; i<sceneNum; i++)
	{
		tempNumBytes[i]=*reinterpret_cast<int*>(temp+displacement);
		displacement+=sizeof(int);
		tempBuffer[i]=reinterpret_cast<char*>(temp+displacement);
		displacement+=sizeof(char)*tempNumBytes[i];
	}

	input.close();
}

void MeWidg::loadProject()
{
	projectMenu->clear();
	QAction* action;
	projectMenu->addAction(action=new QAction("Add Scene", this));
	connect(action, SIGNAL(triggered()), this, SLOT(addScene()));

	mb->addSeparator();

	QString targetSceneName=QFileDialog::getOpenFileName(this, "Load File", ".ProjDT", "*.ProjDT");
	if(targetSceneName=="")
		return;

	std::string sceneName=targetSceneName.toLocal8Bit().constData();

	std::ifstream input(sceneName, std::ios::binary | std::ios::in);
	std::streampos begin, end;
	input.seekg(0, input.end);
	end=input.tellg();
	input.seekg(0, input.beg);
	begin=input.tellg(); 
	size_t numBytes=(end-begin);
	char* temp=new char[numBytes];
	input.read(temp, numBytes);

	int displacement=0;

	int sceneNum=*reinterpret_cast<int*>(temp+displacement);
	displacement+=sizeof(int);

	for(int i=0; i<sceneNum; i++)
	{
		tempNumBytes[i]=*reinterpret_cast<int*>(temp+displacement);
		displacement+=sizeof(int);
		tempBuffer[i]=reinterpret_cast<char*>(temp+displacement);
		displacement+=sizeof(char)*tempNumBytes[i];

		action;
		projectMenu->addAction(action=new QAction((std::to_string(i)).c_str(), this));
		connect(action, SIGNAL(triggered()), this, SLOT(selectScene()));
	}

	input.close();
}

void MeWidg::loadScene()
{
	QString targetSceneName=QFileDialog::getOpenFileName(this, "Load File", ".gcDT", "*.gcDT");
	if(targetSceneName=="")
		return;

	std::string sceneName=targetSceneName.toLocal8Bit().constData();

	gameGLWindow->loadScene(sceneName);
	currentScene=-1;
}

void MeWidg::addScene()
{
	QString targetSceneName=QFileDialog::getOpenFileName(this, "Load File", ".gcDT", "*.gcDT");
	if(targetSceneName=="")
		return;

	std::string sceneName=targetSceneName.toLocal8Bit().constData();

	std::ifstream input(sceneName, std::ios::binary | std::ios::in);
	std::streampos begin, end;
	input.seekg(0, input.end);
	end=input.tellg();
	input.seekg(0, input.beg);
	begin=input.tellg();
	size_t numBytes=(end-begin);
	tempBuffer[sceneNum]=new char[numBytes];
	tempNumBytes[sceneNum]=numBytes;
	input.read(tempBuffer[sceneNum], numBytes);

	input.close();

	QAction* action;
	projectMenu->addAction(action=new QAction((std::to_string(sceneNum)).c_str(), this));
	connect(action, SIGNAL(triggered()), this, SLOT(selectScene()));
	sceneNum++;
}

void MeWidg::selectScene()
{
	QAction *checkAction=(QAction*)sender();
	int checkNum=checkAction->text()[0].digitValue();
	sceneClicked(checkAction->text()[0].digitValue());
}

void MeWidg::sceneClicked(int num)
{
	gameGLWindow->loadSceneFromProject(tempBuffer[num]);
	currentScene=num;
}