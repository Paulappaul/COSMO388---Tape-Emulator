#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Dial.H>
#include <FL/fl_draw.H>
#include <FL/Fl_PNG_Image.H> // Include Fl_PNG_Image header
#include <string>
#include <algorithm>

#define NOMINMAX // Add this line to prevent Windows API macros

class CustomDial : public Fl_Dial {
private:
    std::string backgroundImage;
    double value;

public:
    CustomDial(int x, int y, int w, int h, const char* label = 0);

    void setBackgroundImage(const std::string& imageFile);
    void setValue(double newValue);

    void draw() override;
    int handle(int event) override;
};

CustomDial::CustomDial(int x, int y, int w, int h, const char* label)
    : Fl_Dial(x, y, w, h, label), value(0.0) {
}

void CustomDial::setBackgroundImage(const std::string& imageFile) {
    backgroundImage = imageFile;
}

void CustomDial::setValue(double newValue) {
    value = std::min(1.0, std::max(0.0, newValue));
    redraw();
}

void CustomDial::draw() {
    // Drawing implementation for the background image...
    if (!backgroundImage.empty()) {
        Fl_PNG_Image bgImage(backgroundImage.c_str());

        // Calculate the center of the dial
        int centerX = x() + w() / 2;
        int centerY = y() + h() / 2;

        // Calculate the angle for rotation
        double rotationAngle = angle1() + value * (angle2() - angle1());

        fl_push_matrix();
        fl_translate(centerX, centerY);
        fl_rotate(rotationAngle);
        fl_translate(-centerX, -centerY);

        bgImage.draw(x(), y(), w(), h());

        fl_pop_matrix();
    }

    // Draw the dial's outline
    fl_color(FL_BLACK);
    fl_arc(x(), y(), w(), h(), 0, 360);
}

int CustomDial::handle(int event) {
    switch (event) {
    case FL_PUSH:
    case FL_DRAG: {
        // Calculate the new value based on mouse position
        double oldAngle = angle1() + value * (angle2() - angle1());
        double newAngle = atan2(Fl::event_y() - y() - h() / 2, Fl::event_x() - x() - w() / 2);
        double angleDiff = newAngle - oldAngle;

        // Convert the angle difference to a value change
        double valueChange = angleDiff / (angle2() - angle1());
        double newValue = value + valueChange;

        setValue(newValue);
        do_callback();
        return 1; // Event handled
    }
    default:
        return Fl_Dial::handle(event);
    }
}
