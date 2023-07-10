#include <Fl.H>
#include <Fl_Window.H>
#include <Fl_Button.H>
#include <Fl_Slider.H>
#include <Fl_Dial.H>
#include <Fl_Box.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Timer.H>




class EQGroup : public Fl_Group 
{
public:
    EQGroup(int x, int y, int w, int h) : Fl_Group(x, y, w, h) {}

    void draw() override
    {
        draw_child(*child(0));  // Draw the lower dial first
        draw_child(*child(1));  // Draw the top dial on top
    }
};



void EQDials(int x, int y, int w, int h, int LowDialColor, int HighDialColor)

{


    EQGroup* groupName = new EQGroup(x, y, w, h);
    groupName->begin();

    Fl_Dial* EqLow1 = new Fl_Dial(x, y, w, h);
    Fl_Dial* EqQ = new Fl_Dial(x + 20, y + 20, w - 40, h - 40);

    EqLow1->type(FL_LINE_DIAL);

    if (LowDialColor == 0)
    {
        EqLow1->color(FL_DARK3);
    }

    if (HighDialColor == 1)
    {

        EqQ->color(FL_RED);

    }
    else if (HighDialColor == 2)
    {

        EqQ->color(fl_rgb_color(230, 148, 60));

    }
    else if (HighDialColor == 3)
    {
        EqQ->color(FL_YELLOW);

    }

        EqQ->value(0.55);

    groupName->end();

}

void Send_Dials(int x, int y, int w, int h, Fl_JPEG_Image* SendBG)
{
    Fl_Box* SendBox = new Fl_Box(FL_FLAT_BOX, x-10, y-10, 95, 200, 0);
    SendBox->image(SendBG);
    Fl_Dial* Send1_1 = new Fl_Dial(x, y, w, h, 0);
    Fl_Dial* Send2_1 = new Fl_Dial(x, y+100, w, h, 0);
    Send1_1->color(FL_GREEN);
    Send2_1->color(FL_BLUE);
    Send1_1->type(FL_LINE_DIAL);
    Send2_1->type(FL_LINE_DIAL);

}

int main(int argc, char* argv[]) 
{


    Fl_Window* window = new Fl_Window(1700, 1000, "Cosmo38");
    window->color(fl_rgb_color(54, 50, 49));

    //Logo
    Fl_JPEG_Image* logoImage = new Fl_JPEG_Image("C:\\Users\\alcin\\Desktop\\COSMO38.JPG");
    Fl_Box* logo = new Fl_Box(1425, 10, 100, 80);
    logo->image(logoImage);

    



    //Transport Display
    Fl_Button* Play = new Fl_Button(1460, 700, 100, 100);
    Play->label("@>");
    Fl_Button* Stop = new Fl_Button(1360, 700, 100, 100);
    Stop->label("@square");

    Fl_Button* Rewind = new Fl_Button(1260, 700, 100, 100);
    Rewind->label("@<<");
    Fl_Button* FastForward = new Fl_Button(1560, 700, 100, 100);
    FastForward->label("@>>");



    // Load PNG image
    Fl_JPEG_Image* pngImage = new Fl_JPEG_Image("C:\\Users\\alcin\\Desktop\\VU METER.JPG");
    Fl_JPEG_Image* reelImage = new Fl_JPEG_Image("C:\\Users\\alcin\\Desktop\\reels.JPG");
    if (pngImage->fail())
    {
        delete pngImage;
        return 1;
    }

    // VU Meters
    Fl_Box* pngBox = new Fl_Box(0, 0, 200, 90, 0);
    Fl_Box* channel2 = new Fl_Box(0, 0, 500, 90, 0);
    Fl_Box* channel3 = new Fl_Box(0, 0, 800, 90, 0);
    Fl_Box* channel4 = new Fl_Box(0, 0, 1100, 90, 0);
    Fl_Box* channel5 = new Fl_Box(0, 0, 1400, 90, 0);
    Fl_Box* channel6 = new Fl_Box(0, 0, 1700, 90, 0);
    Fl_Box* channel7 = new Fl_Box(0, 0, 2000, 90, 0);
    Fl_Box* channel8 = new Fl_Box(0, 0, 2300, 90, 0);
    Fl_Box * Reel2Reel = new Fl_Box(0, 0, 2910, 500, 0);

    pngBox->image(pngImage);
    channel2->image(pngImage);
    channel3->image(pngImage);
    channel4->image(pngImage);
    channel5->image(pngImage);
    channel6->image(pngImage);
    channel7->image(pngImage);
    channel8->image(pngImage);
    Reel2Reel->image(reelImage);

   // Fl_JPEG_Image* reelImage = new Fl_JPEG_Image("C:\\Users\\alcin\\Desktop\\COSMOREEL.JPG");
   // Fl_Box* reel1 = new Fl_Box(1285, 250, 100, 80);
    //Fl_Box* reel2 = new Fl_Box(1530, 250, 100, 80);
    //reel1->image(reelImage);
   // reel2->image(reelImage);



    Fl_Slider* Channel1 = new Fl_Slider(50, 670, 95, 300);
    Fl_Slider* Channel2 = new Fl_Slider(200, 670, 95, 300);
    Fl_Slider* Channel3 = new Fl_Slider(350, 670, 95, 300);
    Fl_Slider* Channel4 = new Fl_Slider(500, 670, 95, 300);
    Fl_Slider* Channel5 = new Fl_Slider(650, 670, 95, 300);
    Fl_Slider* Channel6 = new Fl_Slider(800, 670, 95, 300);
    Fl_Slider* Channel7 = new Fl_Slider(950, 670, 95, 300);
    Fl_Slider* Channel8 = new Fl_Slider(1100, 670, 95, 300);

    //Column1
    Fl_JPEG_Image* columnTest = new Fl_JPEG_Image("C:\\Users\\alcin\\Desktop\\dials.jpg");
    Fl_Box* Column1 = new Fl_Box(FL_FLAT_BOX, 50, 90, 95, 325, 0);
    Column1->image(columnTest);
    Column1->type(FL_FLAT_BOX);
    Column1->color(fl_rgb_color(233, 234, 230));;
    EQDials(60, 100, 75, 75, 0, 1);
    EQDials(60, 212, 75, 75, 0, 2);
    EQDials(60, 325, 75, 75, 0, 3);
 
    //Column2 
    Fl_Box* Column2 = new Fl_Box(FL_FLAT_BOX, 200, 90, 95, 325, 0);
    Column2->image(columnTest);
    EQDials(210, 100, 75, 75, 0, 1);
    EQDials(210, 212, 75, 75, 0, 2);
    EQDials(210, 325, 75, 75, 0, 3);

    //Column3

    Fl_Box* Column3 = new Fl_Box(FL_FLAT_BOX, 350, 90, 95, 325, 0);
    Column3->image(columnTest);
    EQDials(360, 100, 75, 75, 0, 1);
    EQDials(360, 212, 75, 75, 0, 2);
    EQDials(360, 325, 75, 75, 0, 3);

    //Column4
    Fl_Box* Column4 = new Fl_Box(FL_FLAT_BOX, 500, 90, 95, 325, 0);
    Column4->image(columnTest);
    EQDials(510, 100, 75, 75, 0, 1);
    EQDials(510, 212, 75, 75, 0, 2);
    EQDials(510, 325, 75, 75, 0, 3);

    //Column5
    Fl_Box* Column5 = new Fl_Box(FL_FLAT_BOX, 650, 90, 95, 325, 0);
    Column5->image(columnTest);
    EQDials(660, 100, 75, 75, 0, 1);
    EQDials(660, 212, 75, 75, 0, 2);
    EQDials(660, 325, 75, 75, 0, 3);

    //Column6
    Fl_Box* Column6 = new Fl_Box(FL_FLAT_BOX, 800, 90, 95, 325, 0);
    Column6->image(columnTest);
    EQDials(810, 100, 75, 75, 0, 1);
    EQDials(810, 212, 75, 75, 0, 2);
    EQDials(810, 325, 75, 75, 0, 3);

    //Column7
    Fl_Box* Column7 = new Fl_Box(FL_FLAT_BOX, 950, 90, 95, 325, 0);
    Column7->image(columnTest);
    EQDials(960, 100, 75, 75, 0, 1);
    EQDials(960, 212, 75, 75, 0, 2);
    EQDials(960, 325, 75, 75, 0, 3);

    //Column8
    Fl_Box* Column8 = new Fl_Box(FL_FLAT_BOX, 1100, 90, 95, 325, 0);
    Column8->image(columnTest);
    EQDials(1110, 100, 75, 75, 0, 1);
    EQDials(1110, 212, 75, 75, 0, 2);
    EQDials(1110, 325, 75, 75, 0, 3);

    Fl_JPEG_Image* SendBG = new Fl_JPEG_Image("C:\\Users\\alcin\\Desktop\\eqdial.jpg");

    //SENDS 1 AND 2 
    Send_Dials(60, 450, 75, 75, SendBG);
    Send_Dials(210, 450, 75, 75, SendBG);
    Send_Dials(360, 450, 75, 75, SendBG);
    Send_Dials(510, 450, 75, 75, SendBG);
    Send_Dials(660, 450, 75, 75, SendBG);
    Send_Dials(810, 450, 75, 75, SendBG);
    Send_Dials(960, 450, 75, 75, SendBG);
    Send_Dials(1110, 450, 75, 75, SendBG);
  


    window->end();
    window->show(argc, argv);
    return Fl::run();
}






