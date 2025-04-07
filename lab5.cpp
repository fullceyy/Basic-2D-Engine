#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <cmath>
using namespace std;

class Engine {
    private:
            SDL_Renderer* renderer;
            SDL_Window* window;
    public:
    /** engine constructor to init the SDL2 sub systems and window with renderer. */
        Engine(){ 
            if(!Init()){
                cout << "Engine couldn't initialize!" << endl;
                return;
            }
            cout << "Engine Init was successful" << endl; 
        };

         /** destructor which will call Destroy function on exit / engine termination. */
        ~Engine(){
            Destroy();
        };

        /** initialize SDL subsystems, create window and renderer and specify important flags / arguments. */
        bool Init(){
            /** subsystems, in our case VIDEO subsystem is initialized. */
            if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0){
                cout << "SDL_Init_video" << SDL_GetError() << endl;
                return false;
            }

            /** window creation with SDL_CreateWindow and SDL_Window* window pointer declared before.
              * Parameters taken : specified height and width of the window, positioning of the window. */
            window = SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_ALLOW_HIGHDPI);

            if(window == NULL){cout << "SDL_CreateWindow" << SDL_GetError() << endl; return false;}

            /** renderer creation with SDL_CreateRenderer and SDL_Renderer* renderer pointer declated before. 
              * Parameters taken : SDL_Window* window. */
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            
            if(renderer == NULL){cout << "SDL_CreateRenderer" << SDL_GetError() << endl; return false;}

            return true;
        };

        /* destroys window, renderer and quits SDL, IMG(or any added lib's). 
         * Parameters taken : SDL_Renderer* renderer, SDL_Window* window. */
        void Destroy(){  
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            IMG_Quit();
            SDL_Quit();
        }

        /*getter method for 'renderer'. */
        SDL_Renderer* getRenderer(){ return renderer;};
        /*getter method for 'window'. */
        SDL_Window* getWindow(){return window;};
};


/** 
 * @class Base
 * @brief An abstract base class defining a common interface for update and draw operations, actual name GameObject.
 * 
 * This class works as a base for other classes that require functionality, 
 * such as Drawing and Transforming an Object. 
 * Derived classes must provide implementations for the `update` and `draw` methods.
 */
class Base {
    public:
        virtual ~Base() = default;
        virtual void update() = 0;
        virtual void draw() = 0;
};

/**
 * @class UpdateAbility
 * @brief A mix - in update class which extends the `Base` class, providing a default implementation for the update function, actual name `UpdateObject`.
 * 
 * This class inherits virtually from the `Base` class, 
 * allowing it to be used as a part of virtual multiple inheritance hierarchy.
 * It provides a default empty implementation of the `update()` method, which can be overriden in the derived classes.  
 */
class UpdateAbility : public virtual Base {
    public:
    /**
     * @brief Default virtual destructor.
     * 
     * Makes sure the cleanup is done.
     */
        virtual ~UpdateAbility() = default;
        virtual void update() override {};
};

/**
 * @class DrawAbility
 * @brief This class extends the `Base` class, and provides the ability to `draw` and `update`, 
 * actual name `DrawObject`.
 * 
 * Provides `draw()` and `update()` methods, which can be overriden in the derived classes to provide custom behaviour for created objects. 
 */
class DrawAbility : public virtual Base {
    public:
    /**
     * @brief Default virtual destructor.
     * 
     * Makes sure the cleanup is done.
     */
        virtual ~DrawAbility() = default;
        virtual void draw() override {};
        virtual void update() override {};
};

/**
 * @class Transformability
 * @brief This class provides transformation abilities.
 *
 * Provides the ability to transform an object through `rotate`, `scale`, 
 * and `translate` operations. It is an abstract class that defines pure virtual functions
 * for these transformations, which MUST be implemented by derived classes.
 */
class Transformability : public virtual Base{
    public:
        virtual ~Transformability() = default;
         /**
         * @brief Rotates the object by the given angle.
         * 
         * This pure virtual method should be overridden by derived classes to implement 
         * specific rotation behavior.
         *
         * @param angle The angle by which the object is to be rotated.
         */
        virtual void rotate(float angle) = 0;
         /**
         * @brief Scales the object by the given value.
         * 
         * This pure virtual method should be overridden by derived classes to implement 
         * specific scale behavior.
         *
         * @param factor The value by which the object is to be scaled.
         */
        virtual void scale(float factor) = 0;
         /**
         * @brief Translates the object by the given `X` and `Y` coordinates.
         * 
         * This pure virtual method should be overridden by derived classes to implement 
         * specific rotation behavior.
         *
         * @param dx translates an objects `X` coordinate.
         * @param dy translates an objects `Y` coordinate.
         */
        virtual void translate(int dx, int dy) = 0;
};

/**
 * @class shapeObj
 * @brief A class representing a shape object that can be transformed and / or drawn. Inherits from Transformability and DrawAbility.
 *
 * The `shapeObj` class is more specific implementation of the `Transformability` and `DrawAbility`
 * classes. It combines the ability to perform geometric transformations (rotate, scale, translate)
 * with the ability to draw the object. It overrides the virtual methods from both the `Transformability`
 * and `DrawAbility` classes to provide specific behavior for each shape.
 */
class shapeObj : public Transformability, public DrawAbility {
    public:
        virtual ~shapeObj() = default;
        virtual void draw() override {};
        virtual void rotate(float angle) override {};       
        virtual void scale(float factor) override {};       
        virtual void translate(int dx, int dy) override {};
};

/**
 * @class Point2D
 * @brief A class representing a 2D point that can be transformed and drawn.
 *
 * The `Point2D` class virtually inherits from `shapeObj`, which provides transformation and drawing methods.
 * It represents a 2D point with specific x and y coordinates. The class provides methods for setting
 * and getting the coordinates, as well as drawing and transforming the point when inherited behavior is used.
 */
class Point2D : public virtual shapeObj {
    private:
        SDL_Renderer* renderer; ///< SDL_Renderer used for rendering the point in the window.
        int xInputed; ///< `X` - coordinate of the point.
        int yInputed; ///< `Y` - coordinate of the point.
    public:
        /**
         * @brief Constructor to create a 2D point with specified coordinates.
         *
         * The constructor initializes the point with the given x and y coordinates. 
         * If no coordinates are provided, it defaults to (0, 0).
         *
         * @param x The x-coordinate of the point (default is 0).
         * @param y The y-coordinate of the point (default is 0).
         */
        Point2D(int x = 0, int y = 0) : xInputed(x), yInputed(y){};

        /**
         * @brief Sets the coordinates of the point object.
         *
         * This method allows updating the x and y coordinates of the point.
         *
         * @param x The new x-coordinate of the point.
         * @param y The new y-coordinate of the point.
         */
        void setPoint(int x, int y){
            this->xInputed = x;
            this->yInputed = y;
        }

        /**
         * @brief Gets the x-coordinate of the point.
         *
         * This method returns x coordinate of the point.
         * 
         * @return The x-coordinate of the point.
         */
        int getX(){
         return xInputed;   
        }

        /**
         * @brief Gets the y-coordinate of the point.
         *
         * This method returns y coordinate of the point.
         * 
         * @return The y-coordinate of the point.
         */
        int getY(){
            return yInputed;
        }
};

/**
 * @class LineSegment
 * @brief A class representing a 2D line segment between two points with drawing and transformation capabilities.
 *
 * The `LineSegment` class represents a line segment defined by two `Point2D` objects: the `start` and `end` points.
 * It provides functionality for setting / getting the points(their coordinates), drawing the segment from defined points, 
 * and applying transformations such as translation, rotation, and scaling. 
 * The class also uses an `SDL_Renderer` and an `SDL_Color` to render the line segment to a window with a given color.
 */
class LineSegment : public virtual shapeObj {
private:
    Point2D start; ///< The start point of the lineSegment.
    Point2D end; ///< The end point of the lineSegment.
    SDL_Renderer* renderer; ///< SDL_Renderer used for rendering the point in the window.
    SDL_Color color; ///< SDL_Color used to specify the color of the lineSegment object.

public:
    /**
     * @brief Constructor for lineSegment object.
     *
     * Initializes a line segment with the given start and end points, renderer, and color.
     *
     * @param start The `Point2D` start point of the line segment.
     * @param end The `Point2D` end point of the line segment.
     * @param renderer The SDL_Renderer used for drawing the segment.
     * @param color The color of the line segment.
     */
    LineSegment(Point2D &start, Point2D &end, SDL_Renderer* renderer, SDL_Color color)
        : start(start), end(end), renderer(renderer), color(color) {}

    /**
     * @brief Sets the start point of the line segment object.
     *
     * This method sets / updates the start point's coordinates to the given x and y values.
     *
     * @param xStart The new x-coordinate of the start point.
     * @param yStart The new y-coordinate of the start point.
     */
    void setStart(int xStart, int yStart) {
        start.setPoint(xStart, yStart);
    }
        /**
     * @brief Sets the end point of the line segment.
     *
     * This method updates the end point's coordinates to the given x and y values.
     *
     * @param xEnd The new x-coordinate of the end point.
     * @param yEnd The new y-coordinate of the end point.
     */
    void setEnd(int xEnd, int yEnd) {
        end.setPoint(xEnd, yEnd);
    }
    /**
     * @brief Gets the x-coordinate of the start point.
     *
     * This method returns the x-coordinate of the start point of the line segment.
     *
     * @return The x-coordinate of the start point.
     */
    int getStartX() { return start.getX(); }
    /**
     * @brief Gets the y-coordinate of the start point.
     *
     * This method returns the y-coordinate of the start point of the line segment.
     *
     * @return The y-coordinate of the start point.
     */
    int getStartY() { return start.getY(); }
    /**
     * @brief Gets the x-coordinate of the end point.
     *
     * This method returns the x-coordinate of the end point of the line segment.
     *
     * @return The x-coordinate of the end point.
     */
    int getEndX() { return end.getX(); }
    /**
     * @brief Gets the y-coordinate of the end point.
     *
     * This method returns the y-coordinate of the end point of the line segment.
     *
     * @return The y-coordinate of the end point.
     */
    int getEndY() { return end.getY(); }

    /**
     * @brief Draws the line segment to the renderer.
     *
     * This method sets the drawing color and uses the SDL_Renderer to render the line segment between the start and
     * end points.
     */
    void draw() override {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(renderer, start.getX(), start.getY(), end.getX(), end.getY());
        SDL_RenderPresent(renderer);
    }
    /**
     * @brief Draws the line segment to the renderer.
     *
     * This method is similar to `draw()`, but allows drawing the line segment without overriding the inherited
     * behavior, since the base classes have no `drawSegment()` method.
     */
    void drawSegment() {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(renderer, start.getX(), start.getY(), end.getX(), end.getY());
        SDL_RenderPresent(renderer);
    }

    /**
     * @brief Translates the line segment by given offset.
     *
     * This method translates the line segment object by a given vertical(dy) and horizontal(dx) offset in parameters of the function,
     * and redraws the segment, derived function from the `Transformability` class.
     * Is done using the `setPoint()` function and getter methods of `Point2D` class.
     * 
     * @param dx The horizontal offset.
     * @param dy The vertical offset.
     */
    void translate(int dx, int dy) override {
        start.setPoint(start.getX() + dx, start.getY() + dy);
        end.setPoint(end.getX() + dx, end.getY() + dy);
        drawSegment();
    }
    /**
     * @brief Rotates the line segment object by a given angle.
     *
     * This method rotates the line segment around its center point by the specified angle (in degrees, then converts it into radians) 
     * and then redraws the segment.
     *
     * @param angle The angle by which to rotate the line segment, speicify it in degrees.
     */
    void rotate(float angle) override {
        int x1 = start.getX();
        int y1 = start.getY();
        int x2 = end.getX();
        int y2 = end.getY();

        float centerX = (x1 + x2) / 2.0f;
        float centerY = (y1 + y2) / 2.0f;

        float radians = angle * M_PI / 180.0f;

        int newX1 = static_cast<int>(centerX + (x1 - centerX) * cos(radians) - (y1 - centerY) * sin(radians));
        int newY1 = static_cast<int>(centerY + (x1 - centerX) * sin(radians) + (y1 - centerY) * cos(radians));
        int newX2 = static_cast<int>(centerX + (x2 - centerX) * cos(radians) - (y2 - centerY) * sin(radians));
        int newY2 = static_cast<int>(centerY + (x2 - centerX) * sin(radians) + (y2 - centerY) * cos(radians));

        start.setPoint(newX1, newY1);
        end.setPoint(newX2, newY2);
        drawSegment();
    }
        /**
     * @brief Scales the line segment object by a given factor.
     *
     * This method scales the line segment around its center point by the specified factor(float value) and then
     * redraws the segment.
     *
     * @param factor The factor by which to scale the line segment.
     */
    void scale(float factor) override {
        float centerX = (start.getX() + end.getX()) / 2.0f;
        float centerY = (start.getY() + end.getY()) / 2.0f;

        int newX1 = static_cast<int>(centerX + (start.getX() - centerX) * factor);
        int newY1 = static_cast<int>(centerY + (start.getY() - centerY) * factor);

        int newX2 = static_cast<int>(centerX + (end.getX() - centerX) * factor);
        int newY2 = static_cast<int>(centerY + (end.getY() - centerY) * factor);

        start.setPoint(newX1, newY1);
        end.setPoint(newX2, newY2);
        drawSegment();
    }
};

/**
 * @class Rectangle
 * @brief A class representing a rectangle with drawing and transformation capabilities.
 *
 * The `Rectangle` class provides methods to create a rectangle
 * and perform transformations such as translation, rotation, and scaling. The rectangle
 * is rendered using an `SDL_Renderer` and a specified color.
 */
class Rectangle : public virtual shapeObj {
    private:
        int x; ///< x-coordinate declaration for rectangle object(position in horizontal axis).
        int y; ///< y-coordinate declaration for rectangle object(position in vertical axis).
        int w; ///< width declaration for rectangle object(property).
        int h; ///< height declaration for rectangle object(property).
        SDL_Renderer* renderer; ///< SDL_Renderer used for rendering the rectangle in the window.
        SDL_Color* color; ///< SDL_Color pointer used to specify the color of the rectangle object.
    public:
        virtual ~Rectangle() {};

        Rectangle(){};

        /**
         * @brief Initializes / creates rectangle with given properties(position, dimensions, color) and renderer.
         * 
         * @param x The x-coordinate of the top left corner of the rectangle object.
         * @param y The y-coordinate of the top left corner of the rectangle object.
         * @param w The width of the rectangle.
         * @param h The height of the rectangle.
         * @param color The color which will be used to create a rectangle(outline color).
         * @param renderer SDL_renderer used for rendering the rectangle in the window.
         */
        void createObject(int x, int y, int w, int h, SDL_Color* color, SDL_Renderer *renderer){
            cout << "Object Rectangle Created" << endl;
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
            this->color = color;
            this->renderer = renderer;
        }

        /**
        * @brief Draws the rectangle on the SDL renderer.
        *
        * This method sets the drawing color and renders the rectangle's outline using SDL.
        * Creates a SDL_Rect tmp and uses values which were assigned in the `createObject()` function.
        * Set's a color from the `createObject()` and draws Rectangle to the renderer.
        */
        void draw() {
            SDL_Rect tmp = {x, y, w, h};
            SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
            SDL_RenderDrawRect(renderer, &tmp);
            SDL_RenderPresent(renderer);
        }

        /**
        * @brief Translates the rectangle by a given offset.
        *
        * This method moves the rectangle by the specified horizontal (dx) and vertical (dy) offsets
        * and redraws it.
        *
        * @param dx The horizontal offset to move the rectangle.
        * @param dy The vertical offset to move the rectangle.
        */
        void translate(int dx, int dy){
            x += dx;
            y += dy;
            draw();
        }

        /**
        * @brief Rotates the rectangle around its center by a given angle.
        *
        * This method computes the rotated positions of the rectangle's corners and renders
        * the transformed rectangle using lines connecting these points.
        *
        * @param angle The angle by which to rotate the rectangle, in degrees(which is converted to radians later).
        */
        void rotate(float angle){
            float centerX = x + (w / 2);
            float centerY = y + (h / 2);

        // Convert angle to radians
        float radians = angle * M_PI / 180.0f;

        int points[4][2] = {
            {x, y},              // Top-left
            {x + w, y},          // Top-right
            {x + w, y + h},      // Bottom-right
            {x, y + h}           // Bottom-left
        };

        for (int i = 0; i < 4; ++i) {
            float dx = points[i][0] - centerX;
            float dy = points[i][1] - centerY;
            points[i][0] = round(centerX + (dx * (cos(radians)) - dy * (sin(radians))));
            points[i][1] = round(centerY + (dx * (sin(radians)) + dy * (cos(radians))));
        }


        SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);

        
        SDL_RenderDrawLine(renderer, points[0][0], points[0][1], points[1][0], points[1][1]); // Top edge
        SDL_RenderDrawLine(renderer, points[1][0], points[1][1], points[2][0], points[2][1]); // Right edge
        SDL_RenderDrawLine(renderer, points[2][0], points[2][1], points[3][0], points[3][1]); // Bottom edge
        SDL_RenderDrawLine(renderer, points[3][0], points[3][1], points[0][0], points[0][1]); // Left edge

        SDL_RenderPresent(renderer);
        }

        /**
         * @brief Scales the rectangle by a given factor around it's center.
         *
         * This method adjusts the rectangle's dimensions and position relative to its center
         * based on the specified scaling factor and redraws it.
         *
         * @param factor The scaling factor (greater than 1 to enlarge, less than 1 to shrink).
         */
        void scale(float factor){
            float scaledWidth = w * factor;
            float scaledHeight = h * factor;    

            float tmpX = (x + w / 2) - scaledWidth / 2;
            float tmpY = (y + h / 2) - scaledHeight / 2;

            x = static_cast<int>(tmpX);
            y = static_cast<int>(tmpY);
            w = static_cast<int>(scaledWidth);
            h = static_cast<int>(scaledWidth);
            draw();
        }

        virtual void update() override {}
};

/**
 * @class Line
 * @brief A class representing a Line with drawing and transformation capabilities.
 *
 * The `Line` class provides methods to create a Line
 * and perform transformations such as translation, rotation, and scaling. The Line
 * is rendered using an `SDL_Renderer` and a specified color.
 */
class Line : public virtual shapeObj {
    private:
        int xStart; ///< Starting x-coordinate of the Line object.
        int yStart; ///< Starting y-coordinate of the Line object.
        int xEnd; ///< Ending x-coordinate of the Line object.
        int yEnd; ///< Ending y-coordinate of the Line object.
        SDL_Renderer* renderer; ///< SDL_Renderer used for rendering the Line in the window.
        SDL_Color* color; ///< SDL_Color pointer used to specify the color of the Line object.
    public:
        virtual ~Line(){};

        Line(){}

        /**
         * @brief Initializes / creates Line with given properties(position, dimensions, color) and renderer.
         * 
         * @param x1 The start x-coordinate of the Line object.
         * @param y1 The start y-coordinate of the Line object.
         * @param x2 The end x-coordinate of the Line object.
         * @param y2 The end y-coordinate of the Line object.
         * @param color The color which will be used to create a Line(outline color).
         * @param renderer SDL_renderer used for rendering the Line in the window.
         */
        void createObject(int x1, int y1, int x2, int y2, SDL_Color* color, SDL_Renderer* renderer){
            this->xStart = x1;
            this->yStart = y1;           
            this->xEnd = x2;
            this->yEnd = y2;
            this->renderer = renderer;
            this->color = color;
        }

        /**
        * @brief Draws the Line on the SDL renderer.
        *
        * This method sets the drawing color and renders the Line based on its start and end coordinates.
        */
        void draw(){
            SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
            SDL_RenderDrawLine(renderer, xStart, yStart, xEnd, yEnd);      
            SDL_RenderPresent(renderer);
        }

        /**
         * @brief Translates the Line by a given offset.
         *
         * This method moves the Line by the specified horizontal (dx) and vertical (dy) offsets
         * and redraws it.
         *
         * @param dx The horizontal offset to move the Line.
         * @param dy The vertical offset to move the Line.
         */        
        void translate(int dx, int dy){
            xStart += dx;
            xEnd += dx;
            yStart += dy;
            yEnd += dy;
            draw();
        }

        /**
         * @brief Rotates the Line around its center by a given angle.
         *
         * This method computes the rotated positions of the Line's start and end points and
         * renders the transformed Line.
         *
         * @param angle The angle by which to rotate the Line, in degrees(converted to the radians later).
         */
        void rotate(float angle){
            float centerX = (xStart + xEnd) / 2;
            float centerY = (yStart + yEnd) / 2;

            float radians = angle * M_PI / 180.0f;

            int points[2][2] = {
                {xStart, yStart}, 
                {xEnd, yEnd}
            };

            for(int i = 0; i < 2; ++i){
                float dx = points[i][0] - centerX;
                float dy = points[i][1] - centerY;
                points[i][0] = round(centerX + (dx * (cos(radians)) - dy * (sin(radians))));
                points[i][1] = round(centerY + (dx * (sin(radians)) + dy * (cos(radians))));
            }
            
            SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);

            SDL_RenderDrawLine(renderer, points[0][0], points[1][0], points[0][1], points[1][1]);
            SDL_RenderPresent(renderer);

        }

        /**
        * @brief Scales the Line by a given factor.
        *
        * This method adjusts the Line's start and end coordinates relative to its origin
        * based on the specified scaling factor and redraws it.
        *
        * @param factor The scaling factor (greater than 1 to enlarge, less than 1 to shrink).
        */
        void scale(float factor){  
        float tmpX = xStart * factor;
        float tmpY = yStart * factor;
        float tmpX2 = xEnd * factor;
        float tmpY2 = yEnd * factor;

        xStart = static_cast<int>(tmpX);
        yStart = static_cast<int>(tmpY);
        xEnd = static_cast<int>(tmpX2);
        yEnd = static_cast<int>(tmpY2);
        draw();
    }

        virtual void update() override{};
};

/**
 * @class BitmapManager
 * @brief This class will be handling fundamental bitmap operations such as creation, loading, deleting, saving and copying to other bmp.
 * 
 * Will be using SDL's SDL_Surface structure to manipulate bitmap operations.
 */
class BitmapManager {
    private:
        SDL_Surface* imageSurface = NULL; ///< Pointer to the SDL_Surface representing the bitmap image.
    public:
        /**
         * @brief Destructor for the `BitmapManager` class.
         * 
         * Cleans up and deletes any associated resources by freeing the `imageSurface`, freeing the surface is important mechanic in the Manager. 
         */
        virtual ~BitmapManager(){deleteBitmapObj();}

        BitmapManager(){}

        /**
         * @brief Loads bitmap image from a given string filename into the surface(`imageSurface`).
         * 
         * If a surface is already loaded, it frees that surface so we can keep using it.
         * 
         * @param filename Reference to a string containing the path and file itself that we ought to load.
         * @return `true` if the image is successfully loaded, `false` if it couldn't either find it with given path,
         * or the file doesn't exist.
         */
        bool loadBitmapContent(string& filename){
            if(imageSurface != NULL){
                SDL_FreeSurface(imageSurface);
                imageSurface == NULL;
            }
            
            imageSurface = IMG_Load(filename.c_str());
            if(imageSurface != NULL){
                return true;
            }
            return false;
        }

        /**
         * @brief Creates a bitmap object / new blank bitmap surface with the specified height, width and bit depth.
         * 
         * If a surface is already loaded, it frees that surface so we can keep using it.
         * Creates a surface with the specified format.
         * 
         * @param bWidth The width of the bitmap.
         * @param bHeight The height of the bitmap.
         * @param depth The bit depth of the bitmap(set to 24 by default).
         * @return `true` if the surface is successfully created, `false` if it isn't.
         */
        bool createBitmapObj(int bWidth, int bHeight, int depth = 24){
            if(imageSurface != NULL){
                SDL_FreeSurface(imageSurface);
                imageSurface = NULL;
            }
            imageSurface = SDL_CreateRGBSurfaceWithFormat(0, bWidth, bHeight, depth, SDL_PIXELFORMAT_RGB24);

            if(imageSurface != NULL){
                return true;
            }
            return false;
        }

        /**
         * @brief deletes the bitmap object / current bitmap surface, by freeing the surface.
         */
        void deleteBitmapObj(){
            if(imageSurface != NULL){
                SDL_FreeSurface(imageSurface);
            }
        }
        /**
        * @brief Saves the current bitmap surface to a specified file.
        *
        * This method saves the bitmap content to the specified file.
        *
        * @param filename Reference to a string containing the path where the bitmap will be saved.
        * @return `true` if the file is successfully saved, `false` otherwise.
        */
        bool saveToFile(string& filename){ // can either save to already exisitng or specify the path myself in parameter;  
            if(imageSurface != NULL){ // checks if it exists
                if(SDL_SaveBMP(imageSurface, filename.c_str()) == 0){
                    /*or should I make it < 0, which will mean save filed*/
                    return true; // file was saved
                }
            }
            return false;
        }

        /**
         * @brief Copies the bitmap content to another `BitmapManager` object.
         *
         * Performs a blit operation to copy the content of this surface to the destination surface.
         *
         * @param copyDestination Reference to another `BitmapManager` object where the content will be copied.
         * @return `true` if the copy operation is successful, `false` otherwise.
         */
        bool copyTo(BitmapManager& copyDestination){
            if(imageSurface && copyDestination.imageSurface){
                SDL_BlitSurface(imageSurface, nullptr, copyDestination.imageSurface, nullptr);
                return true;
            }
            return false; // copying failed cuz there is either no surface or no copyDest.
        }

        /**
         * @brief Getter method for surface.
         * 
         * Provides an access to the surface.
         */
        SDL_Surface* getSurface() {
            return imageSurface;
        }
};

/**
 * @class BitmapObject
 * @brief A class representing a bitmap object with posibiliity of custom draw and transform behaviour.
 *
 * The `BitmapObject` class allows creating and managing a bitmap object that can be drawn, translated,
 * rotated, scaled, and cropped. It integrates with SDL to render textures and performs transformations
 * on the bitmap directly.
 * 
 * The bitmapObject is heavily reliant on the SDL's source and destination rectangle. In the spritesheet,
 * it will be setting a position of where the source rectangle should be reading and copying it into destination rectangle in the renderer.
 */
class BitmapObject : public DrawAbility, public Transformability{
    private:
        BitmapManager bt; ///< Bitmapmanager instance to manage the bitmap surface and loading operations.
        SDL_Surface* tmpSurface; ///< Temporary surface used for loading and converting textures.
        SDL_Texture* texture; ///< SDL_Texture object used for rendering the bitmap.
        SDL_Renderer* renderer; ///< SDL_Renderer used for rendering the bitmapObject.
        string& filename; ///< Reference to the filename BMP will be loaded from.
        int objPosX; ///< object's x-coordinate position.
        int objPosY; ///< object's y-coordinate position.
        int objWidth; ///< bitmap object's width.
        int objHeight; ///< bitmap object's height.

         // This ones are destRect related, also no use for objWidth and objHeight
        int spritePosX; ///< X-coordinate of the source rectangle in the spritesheet / sprite related operation(crops by X-axis).
        int spritePosY; ///< Y-coordinate of the source rectangle in the spritesheet / sprite realted operation(crops by Y-axis).
        int spritePosW; ///< Width of the source rectangle in the spritesheet / sprite related operation(crops by width).
        int spritePosH; ///< Width of the source rectangle in the spritesheet / sprite related operation(crops by width).
        SDL_Rect destRect; ///< Destination rectangle for rendering the bitmap on the screen.
        SDL_Rect srcRect; ///< Source rectangle for cropping the bitmap from texture.
    public:
        /**
         * @brief destructor for the `BitmapObject` class to destroy the created Texture from Surface.
         */
        virtual ~BitmapObject(){if(texture) 
        SDL_DestroyTexture(texture);}

       /**
        * @brief Constructor for the `BitmapObject` class.
        *
        * Initializes the bitmap object with the specified properties and renderer.
        * Using the provided `filename` we load Bitmap Content.
        * After we use the getter method to get the access to the created surface and create Texture from it.
        *
        * @param filename Reference to a string containing the path to the bitmap file to be loaded.
        * @param renderer Pointer to the SDL_Renderer used for rendering the bitmap.
        * @param x X-coordinate of the object's position in the renderer / window.
        * @param y Y-coordinate of the object's position in the renderer / window.
        * @param w Width of the bitmap object.
        * @param h Height of the bitmap object.
        */
        BitmapObject(string& filename, SDL_Renderer* renderer, int x, int y, int w, int h) : filename(filename), renderer(renderer), objPosX(x), objPosY(y), objWidth(w), objHeight(h){
            if(bt.loadBitmapContent(filename)){
                tmpSurface = bt.getSurface();
                texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
                SDL_FreeSurface(tmpSurface);
            } else {
                cout << "texture loading failed!" << endl;
            }
        }

        /**
         * @brief Draws the bitmap object to the screen using the renderer.
         *
         * Uses the source and destination rectangles to determine (cropping / position of specified bmp) and rendering locations.
         * Checks if the texture loaded and created or not.
         * destRect is set by the objects position, provided in the constructor and dimensions of the sprite.
         * srcRect is set by the `setSrcRect()` function, which is called each time we have to change the frame of the sprite.
         */
        void draw() override {
            if(texture != NULL){
                destRect = {objPosX, objPosY, spritePosW, spritePosH};
                srcRect = {spritePosX, spritePosY, spritePosW, spritePosH};
                cout << spritePosX << endl;
                cout << spritePosY << endl;
                cout << spritePosW << endl;
                cout << spritePosH << endl;
                SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
            }
        }

        /**
         * @brief Translates the bitmap object by the specified offsets.
         * 
         * Moves the object by modifying it's position coordinates and redraws it.
         * 
         * @param dx The horizontal offset to move the object.
         * @param dy The vertical offset to move the object.
         */
        void translate(int dx, int dy) override {
            objPosX += dx;
            objPosY += dy;
            draw();
        }

        /**
         * @brief Rotates the bitmap object around its center by the specified angle.
         *
         * Uses SDL_RenderCopyEx for rotation. The angle is specified in degrees. Uses a work arond with SDL_Point,
         * which calculates the center of an object.
         *
         * @param angle The angle to rotate the object, in degrees.
         */
        void rotate(float angle) override {
            if(texture != NULL){
                SDL_Point pt;
                pt = {(destRect.x + destRect.w) / 2, (destRect.y + destRect.h) / 2};
                SDL_RenderCopyEx(renderer, texture, NULL, &destRect, angle, &pt, SDL_FLIP_NONE);
            } else {
                cerr << "Something with rotate in BitmapObject!" << endl;
            }
        }

        /**
         * @brief Sets the source rectangle for cropping the texture from the spritesheet.
         *
         * Specifies the area of the texture to be rendered.
         *
         * @param x X-coordinate of the source rectangle.
         * @param y Y-coordinate of the source rectangle.
         * @param w Width of the source rectangle.
         * @param h Height of the source rectangle.
         */
        void setSrcRect(int x, int y, int w, int h) {
            this->spritePosX = x;
            this->spritePosY = y;
            this->spritePosW = w;
            this->spritePosH = h;
            srcRect = {spritePosX, spritePosY, spritePosW, spritePosH};
            cout << "setSrcRect BitmapObj called" << endl;
        }

        /**
         * @brief Scales the bitmap object by the specified factor.
         *
         * Adjusts the object's size and position to scale it around its center.
         *
         * @param factor The scaling factor.
         */
        void scale(float factor){
            if(texture != NULL){
                float scaledWidth = objWidth * factor;
                float scaledHeight = objHeight * factor;
                float tmpX = (objPosX + objWidth / 2) - scaledWidth / 2;
                float tmpY = (objPosY + objHeight / 2) - scaledHeight / 2;
                objPosX = static_cast<int>(tmpX);
                objPosY = static_cast<int>(tmpY);
                objWidth = static_cast<int>(scaledWidth);
                objHeight = static_cast<int>(scaledHeight);
                draw();
            }
        }
};

/**
 * @class AnimatedObject
 * @brief Abstract base class for objects with animation capabilities.
 *
 * The `AnimatedObject` class provides a pure virtual method `animate` to be implemented
 * by derived classes. It serves as a base class for any object requiring animation functionality.
 */
class AnimatedObject {
    public:
        virtual ~AnimatedObject() = default;
        /**
         * @brief Pure virtual method to handle animation logic that will be dervied later.
         */
        virtual void animate() = 0;        
};


/**
 * @class SpriteObject
 * @brief A class representing an animated sprite object with drawable and animated capabilities.
 *
 * The `SpriteObject` class extends `BitmapObject` and `AnimatedObject` to provide functionalities
 * for drawing, loading and animating sprites. It uses an SDL texture and source rectangles to manage
 * frame-based animations.
 */
class SpriteObject : public BitmapObject, public AnimatedObject {
    private:
        string& filename; ///< Reference to the filename of the spritesheet to load.
        SDL_Renderer* renderer; ///< SDL_Renderer used for rendering the sprite object.
        int x; ///< X-coordinate of the sprite's position.
        int y; ///< Y-coordinate of the sprite's position.
        int w; ///< Width of the frame in the spritesheet.
        int h; ///< Height of the frame in the spritesheet.
        SDL_Rect srcRect; ///< Source rectangle for cropping the current frame from the spritesheet.
        int frameCount;  ///< Total number of frames in the spritesheet.
        int currentFrame; ///< The current frame of the spritesheet that is being displayed.
        int animationSpeed; ///< The speed of animation in milliseconds per frame.
        Uint32 lastFrameTime; ///< Timestamp of the last frame update.
    public:
        virtual ~SpriteObject(){}

        /**
         * @brief Constructor for the `SpriteObject` class.
         *
         * Initializes the sprite object with a sprite sheet, renderer, position, and frame dimensions(sprite's frame properties).
         *
         * @param filename Reference to a string containing the path to the sprite sheet.
         * @param renderer Pointer to the SDL_Renderer used for rendering the sprite.
         * @param x X-coordinate of the sprite's initial position.
         * @param y Y-coordinate of the sprite's initial position.
         * @param frameWidth Width of each animation frame in the sprite sheet.
         * @param frameHeight Height of each animation frame in the sprite sheet.
         */
        SpriteObject(string& filename, SDL_Renderer* renderer, int x, int y, int frameWidth, int frameHeight) : BitmapObject(filename, renderer, x, y, w, h), filename(filename), renderer(renderer), x(x), y(y), w(frameWidth), h(frameHeight){
            this->w = frameWidth;
            this->h = frameHeight;
            lastFrameTime = SDL_GetTicks();
            animationSpeed = 100; // 100 ms
            currentFrame = 0;
            frameCount = 9; // number of frames in the spritesheet for each direction while walking.
        }

        /**
         * @brief Calls in the `draw()` method from the `BitmapObject` class.
         */
        void draw() override {
            BitmapObject::draw();
        }

        /**
         * @brief Updates the animation based on the direction and idle state.
         *
         * Adjusts the source rectangle to display the correct frame and row of the sprite sheet with simple calculations.
         *
         * @param direction The row in the sprite sheet corresponding to the current direction of animation.
         * (// 0: Up, 1: Left, 2: Down, 3: Right) according to the spritesheet layout.
         * @param idle If `true`, the sprite remains idle and displays the first frame of the specified direction.
         */
        void animate(int direction, bool idle){
            Uint32 currentTicks = SDL_GetTicks();
                if (currentTicks - lastFrameTime > animationSpeed) {
                    if(!idle){
                    currentFrame++;
                    if (currentFrame >= frameCount) {
                        currentFrame = 0; // Loop back to the first frame
                    }
                        // int row = direction;
                
                        setSrcRect(currentFrame * w, direction * h, w, h);
                    } else {
                        setSrcRect(0, direction * h, w, h);
                    }
                lastFrameTime = currentTicks;
            }
        }

        /**
         * @brief Derived class which must be present since the `animate()` method is pure virtual function.
         */
        void animate() override{

        }
};

/**
 * @class Player
 * @brief A class representing a player-controlled sprite in a game.
 * 
 * The `Player` class extends `SpriteObject` to include frame animations for a player object, 
 * has implementation of input events for movement and animation. It manages
 * direction the player is facing, travel speed, and idle state.
 */
class Player : public SpriteObject {
private:
    int moveSpeed; ///< Movement speed of the player.
    int direction; ///< 0: Up, 1: Left, 2: Down, 3: Right
    bool idle; ///< keeps track if the player just standing or moving.
public:
    virtual ~Player() = default;
    
    /**
     * @brief Constructs a Player object with specified properties.
     * 
     * direction is set to 2, which is looking down. Idle bool is set to true at the start.
     * 
     * @param filename The path to the sprite sheet for the player.
     * @param renderer SDL_Renderer used for rendering the player in the game window.
     * @param spawnX The x-coordinate where the player spawns.
     * @param spawnY The y-coordinate where the player spawns.
     * @param playerWidth The width of a single frame of the player sprite.
     * @param playerHeight The height of a single frame of the player sprite.
     * @param moveSpeed The speed at which the player moves.
     */
    Player(string& filename, SDL_Renderer* renderer, int spawnX, int spawnY, int playerWidth, int playerHeight, int moveSpeed)
        : SpriteObject(filename, renderer, spawnX, spawnY, playerWidth, playerHeight), moveSpeed(moveSpeed){
            direction = 2;
            idle = true;
    }

    /**
     * @brief Gets direction which the player is currently facing.
     * @return `direction`.
     */
    int getDirection(){
        return direction;
    }

    /**
     * @brief Sets direction in which the player is facing.
     */
    void setDirection(int direction){
        this->direction = direction;
    }
    
        /**
     * @brief Handles keyboard input events to control the player's movement and facing / walking direction.
     * 
     * @param event The SDL_Event object containing keyboard input data.
     * 
     * - **SDL_KEYDOWN**: Updates the direction and moves the player.
     * Idle is set to false since the movement key is pressed. Each SDLK case which represents a direction,
     * will call the `setDirection()` method and `translate()` by specified movespeed.
     * - **SDL_KEYUP**: Sets the player to idle(true).
     * 
     */
    void inputEventHandler(SDL_Event &event) {
        if (event.type == SDL_KEYDOWN) {
            idle = false;
            switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                // direction = 1;
                setDirection(1);
                    translate(-moveSpeed, 0);
                    break;
                case SDLK_RIGHT:
                // direction = 3;
                setDirection(3);
                    translate(moveSpeed, 0);
                    break;
                case SDLK_UP:
                // direction = 0;
                setDirection(0);
                    translate(0, -moveSpeed);
                    break;
                case SDLK_DOWN:
                // direction = 2;
                setDirection(2);
                    translate(0, moveSpeed);
                    break;
            }
        } if(event.type == SDL_KEYUP) {
            idle = true;
        }
    }
    
    /**
     * @brief First class to actually use update, what a surprise. 
     * Updates the player's animation and renders it on the screen
     * (calls animate with given idle bool and direction and then redraws).
     * - Calls the `animate` method to update the player's animation state based on direction and idle status.
     * - Calls the `draw` method to render the player on the screen.
     */
    void update(){
        animate(direction, idle);
        draw();
    }
};

int main(int argc, char* argv[]) {
    Engine engine;
    bool quit = false;
    SDL_Event e;

    const int FPS = 60; 
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    string filename = "img/ss.png";  // Use your sprite sheet image here
    Player p1(filename, engine.getRenderer(), 0, 0, 64, 64, 2);

    while (!quit) {
        SDL_SetRenderDrawColor(engine.getRenderer(), 255, 255, 255, 255);
        SDL_RenderClear(engine.getRenderer());
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            p1.inputEventHandler(e);
        }

        p1.animate();  // Call animate to update the current frame
        p1.update();   // Updates the animation if idle or not
        

        SDL_RenderPresent(engine.getRenderer());
        frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }
    }

    return 0;
}
