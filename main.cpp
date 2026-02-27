// Sample program main.cpp
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <filesystem>
#include "Image.h"
#include "Button.h"
using namespace std;
using namespace sf;

void loadImages(vector<MyImage*> &images);

const int imgW = 640, imgH = 450;
int main()
{

    int selectedImage = 0;
    vector<MyImage*> images;
    loadImages(images);
    RenderWindow window(VideoMode(Vector2u(1000, 800)), "SFML works!");
    CircleShape shape(100.f);
    MyImage *img = new MyImage("../Images/boxes_1.ppm");



    Font font("../ARIAL.TTF");

    vector<Button*> buttons;
    buttons.push_back(new Button(font, {660, 20}, {120, 30}, "Reload", [&images, &selectedImage](){images[selectedImage]->reload();}));
    buttons.push_back(new Button(font, {660, 60}, {120, 30}, "Only Red", [&images, &selectedImage](){images[selectedImage]->filterRed();}));
    buttons.push_back(new Button(font, {660, 100}, {120, 30}, "Only Green", [&images, &selectedImage](){images[selectedImage]->filterGreen();}));
    buttons.push_back(new Button(font, {660, 140}, {120, 30}, "Only Blue", [&images, &selectedImage](){images[selectedImage]->filterBlue();}));
    buttons.push_back(new Button(font, {660, 180}, {120, 30}, "Grey Scale", [&images, &selectedImage](){images[selectedImage]->greyScale();}));
    buttons.push_back(new Button(font, {660, 220}, {120, 30}, "Flip Horizontal", [&images, &selectedImage](){images[selectedImage]->flipHorizontal();}));
    buttons.push_back(new Button(font, {660, 260}, {120, 30}, "Flip Vertical", [&images, &selectedImage](){images[selectedImage]->flipVertical();}));
    buttons.push_back(new Button(font, {660, 300}, {120, 30}, "Advanced Feature 1", [&images, &selectedImage](){images[selectedImage]->advancedFeature1();}));
    buttons.push_back(new Button(font, {660, 340}, {120, 30}, "Advanced Feature 2", [&images, &selectedImage](){images[selectedImage]->advancedFeature2();}));
    buttons.push_back(new Button(font, {660, 380}, {120, 30}, "Advanced Feature 3", [&images, &selectedImage](){images[selectedImage]->advancedFeature3();}));
    buttons.push_back(new Button(font, {660, 420}, {120, 30}, "Save", [&images, &selectedImage](){images[selectedImage]->save();}));
    buttons.push_back(new Button(font, {660, 420}, {120, 30}, "Save", [&images, &selectedImage](){images[selectedImage]->save();}));


    buttons.push_back(new Button(font, {190, imgH+30}, {120, 30}, "<", [&images, &selectedImage](){if (selectedImage > 0) selectedImage--;}));
    buttons.push_back(new Button(font, {330, imgH+30}, {120, 30}, ">", [&images, &selectedImage]() {
        if (selectedImage < images.size()-1) selectedImage++;
    }));


    while (window.isOpen())
    {
        optional<Event> event;
        while ((event = window.pollEvent()))
        {
            if (event->is<Event::Closed>()) // Check if the event is a window close event
            {
                window.close();
                return 0;
            }
            if (auto evt = event->getIf<Event::MouseButtonPressed>()) {
                Vector2i pos = evt->position;
                for (Button *btn: buttons) {
                    if (btn->contains(pos)) {
                        btn->click();
                    }
                }

            }

        }
        window.clear();

        window.draw(*(images[selectedImage]));
        for (Button *btn: buttons) {
            window.draw(*btn);
        }
        window.display();
    }

    return 0;
}

void loadImages(vector<MyImage*> &images) {
    namespace stdfs = std::filesystem ;


    // http://en.cppreference.com/w/cpp/experimental/fs/directory_iterator
    const stdfs::directory_iterator end{} ;

    for( stdfs::directory_iterator iter{"../Images"} ; iter != end ; ++iter )
    {
        // http://en.cppreference.com/w/cpp/experimental/fs/is_regular_file
        if( stdfs::is_regular_file(*iter) ) // comment out if all names (names of directories tc.) are required
        {
            MyImage *img = new MyImage(iter->path().string());
            if (img->getSize().x > 0 && img->getSize().y >0) {
                img->setTargetSize({imgW, imgH});
                img->setPosition({3,3});
                images.push_back(img);

            }
        }
    }


}
