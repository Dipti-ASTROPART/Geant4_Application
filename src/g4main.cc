//////////////////////////////////////////////////////////////////////////////////////////
/*
 *       Filename:  g4main.hh
 *    Description:  Main file to control the flow of Geant4 simulation 
 *        Version:  1.0
 *        Created:  07/05/24 03:03:30 PM IST
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  Diptiranjan Pattanaik
 *   Organization:  INDEPENDENT
*/
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#include        <iostream>
#include        "g4Constants.hh"
#include        "g4TaskManager.hh"
#include        <iostream>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{

    //Start stopwatch to calculate analysis time
    TStopwatch  watch;
    watch.Start();



    time_t  curtime;
    struct  tm  *starttime, *stoptime;
    curtime =   time(NULL);
    starttime =   localtime(&curtime);


    //Validating the presence of control file as an argument and start analysis by task manager class
    if( argv[1] == NULL ||  argc == 1)
    {
        printf("\n\t%s**** ERROR : CONTROL FILE NOT PASSED ****\n\n%s", COLOR_RED, RESET);
        exit(-1);
    }
    else
    {
        MyTaskManager *cTM = new MyTaskManager(argc, argv);
        cTM->AssignTask();

        delete cTM;
    }



    std::cout << "\n" << std::endl;
    std::cout << BOLD << COLOR_MAGENTA << "=====================================================" << RESET << std::endl;
    std::cout << BOLD << COLOR_BG_BLUE <<  "              Geant4 Simulation Program              " << RESET << std::endl;
    std::cout << BOLD << COLOR_MAGENTA << "=====================================================" << RESET << std::endl;
    std::cout << COLOR_GREEN << "Author:     " << RESET  << BOLD<< COLOR_YELLOW << "Diptiranjan Pattanaik" << RESET << std::endl;
    std::cout << COLOR_GREEN << "Contact:    " << RESET  << BOLD<< COLOR_YELLOW << "dipti.astropart@gmail.com" << RESET << std::endl;
    std::cout << COLOR_GREEN << "Institute:  " << RESET  << BOLD<< COLOR_YELLOW << "Tata Institute of Fundamental Research" << RESET << std::endl;
    std::cout << COLOR_GREEN << "Build Date: " << RESET << COLOR_YELLOW << BUILD_DATE << RESET << std::endl;
    std::cout << COLOR_GREEN << "Aim:        " << RESET << COLOR_YELLOW << "This program simulates different detectors" << std::endl;
    std::cout << BOLD << COLOR_MAGENTA << "=====================================================" << RESET << std::endl;

    //Display stop date and time
    curtime     = time(NULL);
    stoptime    = localtime(&curtime);

    // Print start time of the program
    printf("\n%sPrograms started at  : %04d-%02d-%02d %02d:%02d:%02d%s", COLOR_CYAN,
                                                                                starttime -> tm_year + 1900,
                                                                                starttime -> tm_mon + 1,
                                                                                starttime -> tm_mday,
                                                                                starttime -> tm_hour,
                                                                                starttime -> tm_min,
                                                                                starttime -> tm_sec,
                                                                                COLOR_RESET);
    // Print Stop time of the programs
    printf("\n%sProgram stopped at   : %04d-%02d-%02d %02d:%02d:%02d%s", COLOR_CYAN,
                                                                                stoptime -> tm_year + 1900,
                                                                                stoptime -> tm_mon + 1,
                                                                                stoptime -> tm_mday,
                                                                                stoptime -> tm_hour,
                                                                                stoptime -> tm_min,
                                                                                stoptime -> tm_sec,
                                                                                COLOR_RESET);

    watch.Stop();

    double  p_Time = watch.RealTime();
    int hour, min;
    double sec;

    hour    =   (int) (p_Time / 3600);
    p_Time  =   ((p_Time / 3600) - hour) * 60;
    min     =   (int) p_Time;
    sec     =   (p_Time - min) * 60;
    printf("\n%sTotal execution time : %02dhr %02dmin %0.2fsec\n\n%s", COLOR_YELLOW, hour, min, sec, COLOR_RESET);

    return 1;

}   //  main()


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
