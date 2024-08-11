//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#include    "g4ControlFileHandler.hh"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////
/// Constructor
//////////////////////////////////////////////////////////////////////////////////////////
ControlFileHandler::ControlFileHandler()
{

}   //  ::ControlFileHandler()


//////////////////////////////////////////////////////////////////////////////////////////
/// Destructor
//////////////////////////////////////////////////////////////////////////////////////////
ControlFileHandler::~ControlFileHandler()
{

}   //  ::~ControlFileHandler()


//////////////////////////////////////////////////////////////////////////////////////////
/// Is input file ok?
//////////////////////////////////////////////////////////////////////////////////////////
bool    ControlFileHandler::IsInputFileOK()
{
    return p_valid;
}   //  ::IsInputFileOK()


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void    ControlFileHandler::ReadControlFile(char *filename)
{
    p_Filename = filename;

    ResetControlFile();

    ExtractParameters();

}   //  ::ReadControlFile()


//////////////////////////////////////////////////////////////////////////////////////////
/// Return the structure of the Control file
//////////////////////////////////////////////////////////////////////////////////////////
ControlFile ControlFileHandler::GetAllParameters()
{
    return sCF;
}   //  ::GetAllParameters


//////////////////////////////////////////////////////////////////////////////////////////
/// Return the structure of the Control file
//////////////////////////////////////////////////////////////////////////////////////////
ControlFile ControlFileHandler::GetAllParameters(char *filename)
{

    ControlFileHandler::ReadControlFile(filename);
    return sCF;

}   //  ::GetAllParameters


//////////////////////////////////////////////////////////////////////////////////////////
/// Extract parameters
//////////////////////////////////////////////////////////////////////////////////////////
void    ControlFileHandler::ExtractParameters()
{
    char    fword[20];
    char    commentline[300];
    //  Open input control file
    ifstream infile(p_Filename);

    if(infile.fail())
    {
        printf(COLOR_ERROR "\n IN %s::%s(%d)" COLOR_RESET, __FILE__, __func__, __LINE__);
        printf(COLOR_ERROR "\n **** ERROR : OPENING CONTROL FILE '%s' ****\n\n" COLOR_RESET, p_Filename);

        p_valid         =   0;

        return;
    }

    while(1)
    {
        infile >> fword;

        if( !infile.good() ) 
        {
            break;
        }

        if(strcmp(fword, "Task") == 0)
        {
            infile >> p_Task;

            infile.getline(commentline, 200);

        }
        else if(strcmp(fword, "InputPath") == 0)
        {
            infile >> p_Path[p_NPath];

            infile.getline(commentline, 200);
            
            if(ControlFileHandler::p_ValidateDir(p_Path[p_NPath]) == 0)  
            {
                p_valid = 0;
                return;
            }

            p_NPath++;
        }
        else if(strcmp(fword, "OutputPath") == 0)
        {
            infile >> p_Output;

            infile.getline(commentline, 200);

            if(ControlFileHandler::p_ValidateDir(p_Output) == 0)
            {
                p_valid = 0;
                return;
            }

        }
        else if(strcmp(fword, "PrimaryEnergy") == 0)
        {
            infile >> p_PrimaryE1 >> p_PrimaryE2;

            infile.getline(commentline, 200);
        }
        else if(strcmp(fword, "PrimaryParticle") == 0)
        {
            infile >> p_PrimaryName;

            infile.getline(commentline, 200);
        }
        else if(strcmp(fword, "GraphicalMode") == 0)
        {
            infile >> p_GraphicMode;

            infile.getline(commentline, 200);
        }
        else if(strcmp(fword, "MacroPath") == 0)
        {
            infile >> p_MacroPath;

            infile.getline(commentline, 200);

            if(ControlFileHandler::p_ValidateDir(p_MacroPath) == 0)
            {
                p_valid = 0;
                return;
            }
        }
        else if(strcmp(fword, "NBeam") == 0)
        {
            infile >> p_NBeam;

            infile.getline(commentline, 200);
        }
        else
        {
            printf(COLOR_ERROR "\n IN %s::%s(%d)" COLOR_RESET, __FILE__, __func__, __LINE__);
            printf(COLOR_ERROR "\n **** ERROR : INVALID WORD ****\n\n" COLOR_RESET);

            p_valid                 =   0;

            return;

        }
    }
    p_valid = 1;

    infile.close();

    FillControlFile();

    PrintControlFile();

}   //  ::ExtractParameters()


//////////////////////////////////////////////////////////////////////////////////////////
/// Fill control files
//////////////////////////////////////////////////////////////////////////////////////////
void    ControlFileHandler::FillControlFile()
{

    // Reset the control file parameters
    sCF.Task[0]             = '\0';
    sCF.Output[0]           = '\0';
    sCF.MacroPath[0]        = '\0';
    sCF.ParticleName[0]      = '\0';
    for(int i(0); i < 20; i++)
    {
        sCF.Path[i][0]       = '\0';
    }

    sCF.NPath               = 0;
    sCF.PrimaryEnergy1      = -1;
    sCF.PrimaryEnergy2      = -1;
    sCF.GraphicalMode       = 0;

    // Check the path 
    if(p_NPath == 0)
    {
        printf(COLOR_ERROR "\n IN %s::%s(%d)" COLOR_RESET, __FILE__, __func__, __LINE__);
        printf(COLOR_ERROR "\n **** ERROR : NO INPUT PATH HAS BEEN SPECIFIED IN FILE '%s' ****\n\n" COLOR_RESET, p_Filename);

        return;
    }

    // Check the validity of the control file
    if(p_valid == 0)
    {
        printf(COLOR_ERROR "\n IN %s::%s(%d)" COLOR_RESET, __FILE__, __func__, __LINE__);
        printf(COLOR_ERROR "\n **** ERROR : OPENIGN CONTROL FILE '%s' ****\n\n" COLOR_RESET, p_Filename);

        return;
    }


    // Assign values if control file is proper
    strcpy(sCF.Task,            p_Task);
    strcpy(sCF.Output,          p_Output);
    strcpy(sCF.MacroPath,       p_MacroPath);
    strcpy(sCF.ParticleName,    p_PrimaryName);

    sCF.NPath        = p_NPath;
    for(int i(0); i < p_NPath; i++)
    {
        strcpy(sCF.Path[i], p_Path[i]);
    }

    sCF.PrimaryEnergy1  = p_PrimaryE1;
    sCF.PrimaryEnergy2  = p_PrimaryE2;

    sCF.GraphicalMode   = p_GraphicMode; 

    sCF.NBeam           = p_NBeam;
}   //  ::FillControlFile()

//////////////////////////////////////////////////////////////////////////////////////////
/// Print the Controlfile parameters
//////////////////////////////////////////////////////////////////////////////////////////
void    ControlFileHandler::PrintControlFile()
{
    printf(COLOR_INPUT "\n");

    printf("\nTask : %s", sCF.Task);
    for(int i(0); i < sCF.NPath; i++)   
    {
        printf("\nInputDir :  %s", sCF.Path[i]);
    }
    printf("\nOutputDir : %s", sCF.Output);
    printf("\nPrimary Particle : %s", sCF.ParticleName);
    printf("\nPrimary Energy : %8.1f %8.1f", sCF.PrimaryEnergy1, sCF.PrimaryEnergy2);
    printf("\nGraphical Mode : %d", sCF.GraphicalMode);
    printf("\nMacro files path : %s", sCF.MacroPath);
    printf("\nNumber of particle beams : %d", sCF.NBeam);

    printf(COLOR_RESET "\n");
}   //  ::PrintControlFile()


//////////////////////////////////////////////////////////////////////////////////////////
/// Reset controlfile structure
//////////////////////////////////////////////////////////////////////////////////////////
void    ControlFileHandler::ResetControlFile()
{
    p_Task[0]           = '\0';
    p_Output[0]         = '\0';
    p_PrimaryName[0]    = '\0';
    p_MacroPath[0]      = '\0';
    for(int i(0); i < 20; i++)
    {
        p_Path[i][0]     = '\0';
    }

    p_NPath             = 0;
    p_NBeam             = 0;

}   //  ::ResetControlFile()


//////////////////////////////////////////////////////////////////////////////////////////
///    Directory validator
//////////////////////////////////////////////////////////////////////////////////////////
bool    ControlFileHandler::p_ValidateDir(const char dir[])
{

    bool    valid(1),
            b1(0),
            b2(0);

    if(strcmp(dir, ".") == 0)
    {
        b1                          =   1;
    }

    if(strcmp(dir, "./") == 0)
    {
        b2                          =   1;
    }

    if(b1 || b2)
    {
        printf(COLOR_ERROR "\n IN %s::%s(%d)" COLOR_RESET, __FILE__, __func__, __LINE__);
        printf(COLOR_ERROR "\n **** ERROR : PLEASE MENTION FULL PATH OF DIRECTORY ****\n\n" COLOR_RESET);

        valid                       =   0;
    }

    DIR     *pdir;

    pdir                            =   opendir(dir);

    if(!pdir)
    {
        printf(COLOR_ERROR "\n IN %s::%s(%d)" COLOR_RESET, __FILE__, __func__, __LINE__);
        printf(COLOR_ERROR "\n DIRECTORY : %s", dir);
        printf(COLOR_ERROR "\n **** ERROR : DIRECTORY DOES NOT EXIST ****\n\n" COLOR_RESET);

        valid                       =   0;
    }

    closedir(pdir);

    return valid;

}    //    ::p_ValidateDir()


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
