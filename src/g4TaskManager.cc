//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#include    "g4TaskManager.hh"

using namespace std;
//////////////////////////////////////////////////////////////////////////////////////////
/// Constructor
//////////////////////////////////////////////////////////////////////////////////////////
MyTaskManager::MyTaskManager(int argc, char *argv[])
{
    p_argc = argc;
    p_argv = argv;

    cCF = new ControlFileHandler();
    cCF->ReadControlFile(argv[1]);

}   //  ::MyTaskManager()


//////////////////////////////////////////////////////////////////////////////////////////
/// Destructor
//////////////////////////////////////////////////////////////////////////////////////////
MyTaskManager::~MyTaskManager()
{
    delete cCF;

}   //  ~MyTaskManager()


//////////////////////////////////////////////////////////////////////////////////////////
/// Assign task
//////////////////////////////////////////////////////////////////////////////////////////
void    MyTaskManager::AssignTask()
{
    if(cCF->IsInputFileOK())
    {
        Control = cCF->GetAllParameters();
    }
    else
    {
        exit(-1);
    }

    // Initialize Geant4 run manager
    G4RunManager    *runManager = new G4RunManager();

    // Geant4 run manager needs three user initialization classes
    runManager->SetUserInitialization(new MyDetectorConstruction());                    ///< 1. Detector construction class

    // Add physics lists
    //G4VModularPhysicsList *physicsList = new QBBC();
    G4VModularPhysicsList *physicsList = new FTFP_BERT();
    physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());
    G4OpticalPhysics      *optPhysics = new G4OpticalPhysics();

    auto opticalParams  = G4OpticalParameters::Instance();
    opticalParams->SetWLSTimeProfile("delta");

    physicsList->RegisterPhysics(optPhysics);
    runManager->SetUserInitialization(physicsList);                                     ///< 2. Physics list class

    // Action initialization 
    runManager->SetUserInitialization(new MyActionInitialization(Control));             ///< 3. Action initialization class

    runManager->Initialize();

    // Initialize Geant4 User Interactive executive
    G4UIExecutive *ui = nullptr;

    // Start the UI manager
    G4UImanager     *UImanager  = G4UImanager::GetUIpointer();

    // Initialize Visual axecutive
    G4VisManager    *visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();

    // If the Program runs in the Graphical Mode
    if(Control.GraphicalMode)
    {
        ui = new G4UIExecutive(p_argc, p_argv);
        
        // Check if the directory exist
        if( !ValidateDirectory((G4String)Control.MacroPath)) exit(-1);

        G4String vis_name = (G4String)Control.MacroPath+"/vis.mac";
        G4String command = "/control/execute " + vis_name;
        
        if( !ValidateFile(vis_name) ) exit(-1);

        // Initialize Geant4 visualization executive
        UImanager->ApplyCommand(command);
        ui->SessionStart();

        delete ui;
    }

    // If the simulation takes place in the non-graphical mode
    else
    {
        // Check if the directory exist
        if( !ValidateDirectory((G4String)Control.MacroPath)) exit(-1);

        G4String    filename = (G4String) Control.MacroPath + "/run.mac";
        G4String    command  = "/control/execute " + filename;

        CreateRunMacro(filename);

        // Initiate execution without graphical interface
        UImanager->ApplyCommand(command);
    }

    delete visManager;
    delete runManager;

}   //  ::AssignTask()


//////////////////////////////////////////////////////////////////////////////////////////
/// Create the Macro file for run
//////////////////////////////////////////////////////////////////////////////////////////
void    MyTaskManager::CreateRunMacro(G4String filename)
{
    // Create the run.mac
    FILE *runfile = fopen(filename, "w");

    if( !ValidateFile(filename) ) exit(-1);

    printf("\n[RUN MACRO]  : %s", filename.c_str());

    fprintf(runfile, "/run/initialize \n");
    fprintf(runfile, "/control/verbose 1\n");
    fprintf(runfile, "/run/verbose 1\n");
    fprintf(runfile, "/run/beamOn %d\n", Control.NBeam);

    fclose(runfile);

}   //  ::CreateRunMacro()


//////////////////////////////////////////////////////////////////////////////////////////
/// Validate a file exist or not
//////////////////////////////////////////////////////////////////////////////////////////
bool    MyTaskManager::ValidateFile(G4String path)
{
    struct stat info;

    // Check if the file exists
    if (stat(path, &info) != 0) {
        // Cannot access path
        printf(COLOR_WARNING "Warning: File '%s' does not exist\n" COLOR_RESET, path.c_str());
        printf(COLOR_WARNING "Exiting the program\n" COLOR_RESET);
        return 0;
    } else if (info.st_mode & S_IFREG) {
        // Path is a regular file
        return 1;
    } else {
        printf(COLOR_WARNING "Warning: '%s' is not a regular file\n" COLOR_RESET, path.c_str());
        printf(COLOR_WARNING "Exiting the program\n" COLOR_RESET);
        // Path is not a regular file
        return 0;
    }
}   //  ::ValidateFile()


//////////////////////////////////////////////////////////////////////////////////////////
/// Validate a directory
//////////////////////////////////////////////////////////////////////////////////////////
bool    MyTaskManager::ValidateDirectory(G4String dir_name)
{
    struct  stat info;

    // Check if the dorectory exists
    if(stat(dir_name, &info) != 0)
    {
        // Cannot access path
        printf(COLOR_WARNING "Warning: Directory '%s' does not exist\n" COLOR_RESET, dir_name.c_str());
        printf(COLOR_WARNING "Exiting the program\n" COLOR_RESET);
        return 0;
    }
    else if(info.st_mode & S_IFDIR)
    {
        // Path is a directory
        return 1;
    }
    else
    {
        // Path is not a directory
        printf(COLOR_WARNING "Warning: '%s' is not a Directory\n" COLOR_RESET, dir_name.c_str());
        printf(COLOR_WARNING "Exiting the program\n" COLOR_RESET);
        return 0;
    }

}   //  ::ValidateDirectory()


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
