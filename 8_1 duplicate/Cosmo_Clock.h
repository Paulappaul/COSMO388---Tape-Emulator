#pragma once
#include "Cosmo.h"

void updateTime(void*)
{
    if (isRecording == true || playBackOn == true)
    {
        timeout = 1.0;
        second++;
        if (second >= 60) {
            second = 0;
            minute++;
            if (minute >= 60) 
            {
                minute = 0;
                hour++;
            }
        }


        // Redraw the window
        Fl::redraw();
    }
    else if (rewindOn == true)
    {
        timeout = 0.2;
        if(hour == 0 && minute == 0 && second == 0)
        {
            globalGif->stop();
            std::cout << "timer = 0" << std::endl;
            rewindOn = false;
            rewindStop = true;

        }
        else 
        {
            second--;
            if (second <= 0 && hour ==0 && second == 0)
            {
                rewindOn = false;
                rewindStop = true;
            }
            else if(second <=0)
            {
                rewindStop = false; 
                second = 60;
                minute--;
                if (minute <= 0) {
                    minute = 60;
                    hour--;
                }
            }


            // Redraw the window
            Fl::redraw();

        }
    }
    else if (ffOn == true)
    {
        timeout = 0.2; 

        if (hour == 0 && minute == 10 && second == 0) // BETA ALLOWS FOR 10 MIN
        {

            std::cout << "timer = MAX" << std::endl;
            ffOn = false;
            ffStop = true;

        }
        else
        {
            second++;
            if (second >= 60) {
                second = 0;
                minute++;
                if (minute >= 60) {
                    minute = 0;
                    hour++;
                }
            }


            // Redraw the window
            Fl::redraw();

        }



    }

    // Reschedule the update
    Fl::repeat_timeout(timeout, updateTime);
}



// Custom Fl_Box class to display the time
class TimeBox : public Fl_Box
{
public:
    TimeBox(int x, int y, int w, int h, const char* label = nullptr)
        : Fl_Box(x, y, w, h, label)
    {
    }

    void draw()
    {
        std::string timeStr = getTimeString();

        // Clear the box
        fl_draw_box(FL_BORDER_BOX, x(), y(), w(), h(), color());

        // Draw the time text
        fl_color(labelcolor());
        fl_font(labelfont(), labelsize());
        fl_draw(timeStr.c_str(), x(), y(), w(), h(), align());
    }

private:
    std::string getTimeString()
    {
        std::string timeStr;

        if (hour < 10)
            timeStr += "0";
        timeStr += std::to_string(hour);
        timeStr += ":";

        if (minute < 10)
            timeStr += "0";
        timeStr += std::to_string(minute);
        timeStr += ":";

        if (second < 10)
            timeStr += "0";
        timeStr += std::to_string(second);

        return timeStr;
    }
};
