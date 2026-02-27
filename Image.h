#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <cstring>

using namespace std;



#ifndef IMAGEPROJECT_IMAGE_H
#define IMAGEPROJECT_IMAGE_H
struct RGB {
    char r, g, b;
    RGB(char r, char g, char b):r(r), g(g), b(b){}
    RGB():r(0),g(0),b(0){}
};

class MyImage : public sf::Drawable
{
    vector<RGB> pixels;
    sf::Vector2f size;
    sf::Vector2f targetSize;;
    const int PORTRAIT = 1;
    const int LANDSCAPE = 2;
    sf::Vector2f position;
    string fileName ;

    void load(string filename);
public:
    MyImage(string filename);
    void draw(sf::RenderTarget &target, sf::RenderStates states)const;
    void  setTargetSize(sf::Vector2f target);
    void setPosition(sf::Vector2f pos){this->position = pos;}
    sf::Vector2f getSize();
    void save();
    friend ostream& operator<<(ostream &os, MyImage &img) {
        return os << img.fileName << " " << img.size.x <<" " << img.size.y<<endl;;

    }
    // functions to implement
    void flipHorizontal();
    void flipVertical();
    void advancedFeature1();
    void advancedFeature2();
    void advancedFeature3();
    void reload();
    void filterRed();
    void filterGreen();
    void filterBlue();
    void greyScale();
};


#endif //IMAGEPROJECT_IMAGE_H