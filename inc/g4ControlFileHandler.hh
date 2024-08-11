//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef     g4ControlFileHandler_HH
#define     g4ControlFileHandler_HH

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#include    <iostream>
#include    "g4Structures.hh"
#include    "g4Constants.hh"
#include    "g4CPP_ROOT_Headers.hh"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
class   ControlFileHandler
{
    public:
                                    ControlFileHandler();
                                    ~ControlFileHandler();
        void                        ReadControlFile(char *filename);
        ControlFile                 GetAllParameters();
        ControlFile                 GetAllParameters(char *filename);
        bool                        IsInputFileOK();
    private:
        bool                        p_GraphicMode,
                                    p_valid;
        char                        *p_Filename;
        char                        p_Path[20][300],
                                    p_Output[300],
                                    p_MacroPath[300],
                                    p_Task[50],
                                    p_PrimaryName[50];
        int                         p_NPath,
                                    p_NBeam;
        double                      p_PrimaryE1,
                                    p_PrimaryE2;
        ControlFile                 sCF;
        bool                        p_ValidateDir(const char dir[]);
        void                        ExtractParameters(),
                                    PrintControlFile(),
                                    FillControlFile(),
                                    ResetControlFile();
};

#endif


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
