//
// Created by floodd on 26/01/2026.
//

#include "Image.h"

#include <iostream>

/**
 * This function creates the image object, and then loads in the image from the given filename.
 *
 * @param filename: The name of the file to load
 * /
 */
MyImage::MyImage(string filename): fileName(filename) {
    this->load(filename);
}

/**
 *  This function will reload the original image.
 */
void MyImage::reload() {
    this->load(fileName);
}

/**
 *  This function is responsible for drawing the image scaling to fit on the screen. It also draws a grey
 *  background behind the image to allow for scaling.
 */
void MyImage::draw(sf::RenderTarget &target, sf::RenderStates states)const {

    float x = 0, y = 0;
    int type = this->size.x > this->size.y? LANDSCAPE: PORTRAIT;
    float step=1;
    float startX=this->position.x, startY=this->position.y;
    float dimW = this->targetSize.x;
    float dimH = this->targetSize.y;


    if (this->size.x < this->targetSize.x && this->size.y < this->targetSize.y) {

        dimW = this->size.x;
        startY += (this->targetSize.y - this->size.y)/2.0f;
        dimH = this->size.y;
        startX += (this->targetSize.x- this->size.x)/2.0f;;
    }
    else {
        if (this->size.x / this->targetSize.x > this->size.y / this->targetSize.y) {
            step = this->size.x / this->targetSize.x;
            dimH = this->size.y/step;
            startY += (this->targetSize.y - dimH)/2;;
        }
        else {
            step = this->size.y / (float)this->targetSize.y;
            dimW = this->size.x/step;
            startX +=  (this->targetSize.x - dimW)/2;
        }
    }
    float xStep = 0, yStep=0;
    for (float i = 0; i < dimH; ++i) {
        for (float j = 0; j < dimW; ++j)
        {
            x = (int)xStep;
            y = (int)yStep;
            int pix = (y*this->size.x) + x;
            sf::VertexArray circle(sf::PrimitiveType::Points, 1);
            circle[0].color = sf::Color(this->pixels[pix].r,this->pixels[pix].g,this->pixels[pix].b);
            circle[0].position = {j+startX,i+startY};

            target.draw(circle);
            xStep+=step;
        }
        xStep = 0;
        yStep+=step;
    }
}

/**
* The draw function will scale the image to the maximum defined by this function
*
 * @param target : the target size of the image when displayed.
 */
void MyImage::setTargetSize(sf::Vector2f target) {
    this->targetSize = target;
}

/**
 *
 * @return The size of the original image defined in the PPM file.
 */
sf::Vector2f MyImage::getSize() {
    return this->size;
}

/**
 * This function opens the given ppm file and attempts to read in the data to a continuous vector of pixels defined as
 * RGB structs.
 *
 * @param filename the file to load.
 */
void MyImage::load(string filename) {
    ifstream ifs;

    string dummy;
    ifs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
    try {
        if (ifs.fail()) { throw("Can't open input file"); }
        std::string header;
        int w, h, b;
        // Reads in the first string to check it starts with "P6" defining a ppm file. If not throws an exception as file
        // can't be read.
        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0) throw("Can't read input file");
        getline(ifs, dummy);
        /* Checks to see if there is a comment after the first line */
        char c;
        c = ifs.peek();
        if (c == '#') {
            getline(ifs, dummy);
        }
        // Read in the width and height of the image.
        ifs >> w >> h >> b;
        // set the size within the image object
        this->size = {(float)w,(float)h};
        ifs.ignore(256, '\n'); // skip empty lines in necessary until we get to the binary data
        unsigned char pix[3];
        this->pixels.clear();// remove any existing pixel data
        // read each pixel one by one and convert bytes to floats
        for (int i = 0; i < w * h; ++i) {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            RGB pixel;
            pixel.r = pix[0];
            pixel.g = pix[1];
            pixel.b = pix[2];
            this->pixels.push_back(pixel);
        }

        ifs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ifs.close();
    }
}
void MyImage::save()
{
    string filename = "Output/SavedImage.ppm";
    if (this->size.x == 0 || this->size.y == 0) { fprintf(stderr, "Can't save an empty image\n"); return; }
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
        if (ofs.fail()) throw("Can't open output file");
        ofs << "P6\n" << this->size.x << " " <<  this->size.y << "\n255\n";
        unsigned char r, g, b;
        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i <  this->size.x *  this->size.y; ++i) {
            r = static_cast<unsigned char>(this->pixels[i].r);
            g = static_cast<unsigned char>(this->pixels[i].g);
            b = static_cast<unsigned char>(this->pixels[i].b);
            ofs << r << g << b;
        }
        ofs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
    }
}

void MyImage::filterRed()
{
    for (int i = 0; i < pixels.size(); ++i) {
        pixels[i].g = 0;
        pixels[i].b = 0;
    }
}
void MyImage::filterGreen() {
        for (int i = 0; i < pixels.size(); ++i) {
            pixels[i].r = 0;
            pixels[i].b = 0;
        }
    }

void MyImage::filterBlue() {
    for (int i = 0; i < pixels.size(); ++i) {
        pixels[i].r = 0;
        pixels[i].g = 0;
    }
}

void MyImage::greyScale() {
    for (int i = 0; i < pixels.size(); ++i) {
        unsigned char r = static_cast<unsigned char>(pixels[i].r);
        unsigned char g = static_cast<unsigned char>(pixels[i].g);
        unsigned char b = static_cast<unsigned char>(pixels[i].b);
        unsigned char gray = static_cast<unsigned char>(0.299 * r + 0.587 * g + 0.114 * b);
        pixels[i].r = gray;
        pixels[i].g = gray;
        pixels[i].b = gray;
    }

}

void MyImage::flipHorizontal() {
    const int w = static_cast<int>(this->size.x);
    const int h = static_cast<int>(this->size.y);
    if (w <= 1 || h <= 0 || this->pixels.empty()) return;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w / 2; ++x) {
            const int left  = (y * w) + x;
            const int right = (y * w) + (w - 1 - x);
            std::swap(this->pixels[left], this->pixels[right]);
        }
    }
}
void MyImage::flipVertical() {
    cout << "Flip Vertical" << endl;
}

 //ADVANCED FEATURE 1: Rotate 90 degrees clockwise//

void MyImage::advancedFeature1() {
    const int oldW = static_cast<int>(this->size.x);
    const int oldH = static_cast<int>(this->size.y);
    if (oldW <= 0 || oldH <= 0 || this->pixels.empty()) return;

    const int newW = oldH;
    const int newH = oldW;

    std::vector<RGB> rotated;
    rotated.resize(static_cast<size_t>(newW) * static_cast<size_t>(newH));

    // Mapping (clockwise):
    // newX = oldY
    // newY = (oldW - 1 - oldX)
    for (int oldY = 0; oldY < oldH; ++oldY) {
        for (int oldX = 0; oldX < oldW; ++oldX) {
            const int newX = oldY;
            const int newY = (oldW - 1 - oldX);

            const int oldIdx = (oldY * oldW) + oldX;
            const int newIdx = (newY * newW) + newX;

            rotated[newIdx] = this->pixels[oldIdx];
        }
    }

    this->pixels.swap(rotated);
    this->size = {static_cast<float>(newW), static_cast<float>(newH)};
}
void MyImage::advancedFeature2() {
    cout << "Advanced FEature 2" << endl;
}
void MyImage::advancedFeature3() {
    const int w = static_cast<int>(this->size.x);
    const int h = static_cast<int>(this->size.y);
    if (w <= 0 || h <= 0 || this->pixels.empty()) return;


    const int cropW = static_cast<int>(w * 0.60f); // 60% width
    const int cropH = static_cast<int>(h * 0.60f); // 60% height


    int startX = (w - cropW) / 2;
    int startY = (h - cropH) / 2;


    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;

    int endX = startX + cropW;
    int endY = startY + cropH;

    if (endX > w) endX = w;
    if (endY > h) endY = h;

    const int newW = endX - startX;
    const int newH = endY - startY;

    if (newW <= 0 || newH <= 0) return;


    std::vector<RGB> cropped;
    cropped.resize(static_cast<size_t>(newW) * static_cast<size_t>(newH));

    for (int y = 0; y < newH; ++y) {
        for (int x = 0; x < newW; ++x) {
            const int oldX = startX + x;
            const int oldY = startY + y;

            const int oldIdx = (oldY * w) + oldX;
            const int newIdx = (y * newW) + x;

            cropped[newIdx] = this->pixels[oldIdx];
        }
    }


    this->pixels.swap(cropped);
    this->size = { static_cast<float>(newW), static_cast<float>(newH) };
}

