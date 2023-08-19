
#include "Cosmo.h"
#include "Cosmo_UI.hpp"
#include "Cosmo_Convolution.h"
#include "Cosmo_IO.h"
#include "Cosmo_FileManager.hpp"

void exitCallback()
{
    // Clean up dynamically allocated memory
    for (int i = 0; i < 8; ++i) 
    {
        delete project.dataPass[i];
        std::cout << "project deleted" << std::endl;
        terminate(err);
    }
}


int main(int argc, char* argv[])
{
    sessionSetup();
    Fl_Double_Window* mainWin = mainWindow();

    // Schedule the initial update after 1 second, for time Visual 
   Fl::add_timeout(timeout, updateTime);

   atexit(exitCallback);

   mainWin->end();
   mainWin->show(argc, argv);
   return Fl::run();
   return 0;
}