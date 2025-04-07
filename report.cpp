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
        Engine(){ 
            if(!Init()){
                cout << "Engine couldn't initialize!" << endl;
                return;
            }
            cout << "Engine Init was successful" << endl; 
        };

        ~Engine(){
            Destroy();
        };

        bool Init(){
            if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0){
                cout << "SDL_Init_video" << SDL_GetError() << endl;
                return false;
            }
            window = SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_ALLOW_HIGHDPI);

            if(window == NULL){cout << "SDL_CreateWindow" << SDL_GetError() << endl; return false;}

            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            
            if(renderer == NULL){cout << "SDL_CreateRenderer" << SDL_GetError() << endl; return false;}

            return true;
        };

        void Destroy(){  
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            IMG_Quit();
            SDL_Quit();
        }

        SDL_Renderer* getRenderer(){ return renderer;};

        SDL_Window* getWindow(){return window;};
};


class Base {
    public:
        virtual ~Base() = default;
        virtual void update() = 0;
        virtual void draw() = 0;
};


class UpdateAbility : public virtual Base {
    public:

        virtual ~UpdateAbility() = default;
        virtual void update() override {};
};


class DrawAbility : public virtual Base {
    public:

        virtual ~DrawAbility() = default;
        virtual void draw() override {};
        virtual void update() override {};
};

class Transformability : public virtual Base{
    public:
        virtual ~Transformability() = default;

        virtual void rotate(float angle) = 0;

        virtual void scale(float factor) = 0;

        virtual void translate(int dx, int dy) = 0;
};

class ShapeObj : public Transformability, public DrawAbility {
    public:
        virtual ~ShapeObj() = default;
        virtual void draw() override {};
        virtual void rotate(float angle) override {};       
        virtual void scale(float factor) override {};       
        virtual void translate(int dx, int dy) override {};
};

class Point2D : public virtual ShapeObj {
    private:
        SDL_Renderer* renderer; ///< SDL_Renderer used for rendering the point in the window.
        int xInputed, yInputed;
    public:
        Point2D(int x = 0, int y = 0) : xInputed(x), yInputed(y){};
        void setPoint(int x, int y){
            this->xInputed = x;
            this->yInputed = y;
        }
        int getX(){
         return xInputed;   
        }
        int getY(){
            return yInputed;
        }
};

class LineSegment : public virtual ShapeObj {
private:
    Point2D start, end;
    SDL_Renderer* renderer; ///< SDL_Renderer used for rendering the point in the window.
    SDL_Color color; ///< SDL_Color used to specify the color of the lineSegment object.
public:
    LineSegment(Point2D &start, Point2D &end, SDL_Renderer* renderer, SDL_Color color)
        : start(start), end(end), renderer(renderer), color(color) {}
    void setStart(int xStart, int yStart) {
        start.setPoint(xStart, yStart);
    }
    void setEnd(int xEnd, int yEnd) {
        end.setPoint(xEnd, yEnd);
    }
    int getStartX() { return start.getX(); }
    int getStartY() { return start.getY(); }
    int getEndX() { return end.getX(); }
    int getEndY() { return end.getY(); }

    void draw() override {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(renderer, start.getX(), start.getY(), end.getX(), end.getY());
        SDL_RenderPresent(renderer);
    }
    void drawSegment() {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(renderer, start.getX(), start.getY(), end.getX(), end.getY());
        SDL_RenderPresent(renderer);
    }

    void translate(int dx, int dy) override {
        start.setPoint(start.getX() + dx, start.getY() + dy);
        end.setPoint(end.getX() + dx, end.getY() + dy);
        drawSegment();
    }
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

class Rectangle : public virtual ShapeObj {
    private:
        int x, y, w, h;
        SDL_Renderer* renderer; ///< SDL_Renderer used for rendering the rectangle in the window.
        SDL_Color* color; ///< SDL_Color pointer used to specify the color of the rectangle object.
    public:
        virtual ~Rectangle() {};

        Rectangle(){};

        void createObject(int x, int y, int w, int h, SDL_Color* color, SDL_Renderer *renderer){
            cout << "Object Rectangle Created" << endl;
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
            this->color = color;
            this->renderer = renderer;
        }

        void draw() {
            SDL_Rect tmp = {x, y, w, h};
            SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
            SDL_RenderDrawRect(renderer, &tmp);
            SDL_RenderPresent(renderer);
        }

        void translate(int dx, int dy){
            x += dx;
            y += dy;
            draw();
        }

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

class Line : public virtual ShapeObj {
    private:
        int xStart, yStart, xEnd, yEnd;
        SDL_Renderer* renderer; ///< SDL_Renderer used for rendering the Line in the window.
        SDL_Color* color; ///< SDL_Color pointer used to specify the color of the Line object.
    public:
        virtual ~Line(){};
        Line(){}
        void createObject(int x1, int y1, int x2, int y2, SDL_Color* color, SDL_Renderer* renderer){
            this->xStart = x1;
            this->yStart = y1;           
            this->xEnd = x2;
            this->yEnd = y2;
            this->renderer = renderer;
            this->color = color;
        }

        void draw(){
            SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
            SDL_RenderDrawLine(renderer, xStart, yStart, xEnd, yEnd);      
            SDL_RenderPresent(renderer);
        }
      
        void translate(int dx, int dy){
            xStart += dx;
            xEnd += dx;
            yStart += dy;
            yEnd += dy;
            draw();
        }

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

// class Point : public virtual shapeObj {
//     private:
//         int xInit, yInit;
//         SDL_Renderer* renderer;
//         SDL_Color* color;
//     public:
//         virtual ~Point() = default;
//         Point(int _x, int _y, SDL_Color* color, SDL_Renderer* renderer) : xInit(_x), yInit(_y), color(color), renderer(renderer) {};

//         virtual void draw() override {
//             cout << "Point draw" << endl;
//             SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
//             SDL_RenderDrawPoint(renderer, xInit, yInit);
//         }

//         virtual void update() override {}; 
// };
// WAS LOST IN PRODUCTION SOMEHOW

class BitmapManager {
    private:
        SDL_Surface* imageSurface = NULL; ///< Pointer to the SDL_Surface representing the bitmap image.
    public:
        virtual ~BitmapManager(){deleteBitmapObj();}

        BitmapManager(){}

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
        void deleteBitmapObj(){
            if(imageSurface != NULL){
                SDL_FreeSurface(imageSurface);
            }
        }
        bool saveToFile(string& filename){
            if(imageSurface != NULL){
                if(SDL_SaveBMP(imageSurface, filename.c_str()) == 0){
                    return true;
                }
            }
            return false;
        }

        bool copyTo(BitmapManager& copyDestination){
            if(imageSurface && copyDestination.imageSurface){
                SDL_BlitSurface(imageSurface, nullptr, copyDestination.imageSurface, nullptr);
                return true;
            }
            return false; // copying failed cuz there is either no surface or no copyDest.
        }
        SDL_Surface* getSurface() {
            return imageSurface;
        }
};

class BitmapObject : public DrawAbility, public Transformability{
    private:
        BitmapManager bt;
        SDL_Surface* tmpSurface;
        SDL_Texture* texture; ///< SDL_Texture object used for rendering the bitmap.
        SDL_Renderer* renderer; ///< SDL_Renderer used for rendering the bitmapObject.
        string& filename; ///< Reference to the filename BMP will be loaded from.
        int objPosX, objPosY, objWidth, objHeight;
        int spritePosW, spritePosH, spritePosX, spritePosY;
        SDL_Rect destRect, srcRect;
    public:

        virtual ~BitmapObject(){if(texture) 
        SDL_DestroyTexture(texture);}

        BitmapObject(string& filename, SDL_Renderer* renderer, int x, int y, int w, int h) : filename(filename), renderer(renderer), objPosX(x), objPosY(y), objWidth(w), objHeight(h){
            if(bt.loadBitmapContent(filename)){
                tmpSurface = bt.getSurface();
                texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
                SDL_FreeSurface(tmpSurface);
            } else {
                cout << "texture loading failed!" << endl;
            }
        }

        void draw() override {
            if(texture != NULL){
                destRect = {objPosX, objPosY, spritePosW, spritePosH};
                srcRect = {spritePosX, spritePosY, spritePosW, spritePosH};
                SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
            }
        }

        void translate(int dx, int dy) override {
            objPosX += dx;
            objPosY += dy;
            draw();
        }
        void rotate(float angle) override {
            if(texture != NULL){
                SDL_Point pt;
                pt = {(destRect.x + destRect.w) / 2, (destRect.y + destRect.h) / 2};
                SDL_RenderCopyEx(renderer, texture, NULL, &destRect, angle, &pt, SDL_FLIP_NONE);
            } else {
                cerr << "Something with rotate in BitmapObject!" << endl;
            }
        }

        void setSrcRect(int x, int y, int w, int h) {
            this->spritePosX = x;
            this->spritePosY = y;
            this->spritePosW = w;
            this->spritePosH = h;
            srcRect = {spritePosX, spritePosY, spritePosW, spritePosH};
            cout << "setSrcRect BitmapObj called" << endl;
        }
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

class AnimatedObject {
    public:
        virtual ~AnimatedObject() = default;

        virtual void animate() = 0;        
};


class SpriteObject : public BitmapObject, public AnimatedObject {
    private:
        string& filename; ///< Reference to the filename of the spritesheet to load.
        SDL_Renderer* renderer; ///< SDL_Renderer used for rendering the sprite object.
        int x, y, w, h;
        SDL_Rect srcRect;
        int frameCount, currentFrame, animationSpeed;
        Uint32 lastFrameTime;
    public:
        virtual ~SpriteObject(){}
        SpriteObject(string& filename, SDL_Renderer* renderer, int x, int y, int frameWidth, int frameHeight) : BitmapObject(filename, renderer, x, y, w, h), filename(filename), renderer(renderer), x(x), y(y), w(frameWidth), h(frameHeight){
            this->w = frameWidth;
            this->h = frameHeight;
            lastFrameTime = SDL_GetTicks();
            animationSpeed = 100; // 100 ms
            currentFrame = 0;
            frameCount = 9; // number of frames in the spritesheet for each direction while walking.
        }
        void draw() override {
            BitmapObject::draw();
        }

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

        void animate() override{

        }
};

class Player : public SpriteObject {
private:
    int moveSpeed; ///< Movement speed of the player.
    int direction; ///< 0: Up, 1: Left, 2: Down, 3: Right
    bool idle; ///< keeps track if the player just standing or moving.
public:
    virtual ~Player() = default;
    Player(string& filename, SDL_Renderer* renderer, int spawnX, int spawnY, int playerWidth, int playerHeight, int moveSpeed)
        : SpriteObject(filename, renderer, spawnX, spawnY, playerWidth, playerHeight), moveSpeed(moveSpeed){
            direction = 2;
            idle = true;
    }
    int getDirection(){
        return direction;
    }
    void setDirection(int direction){
        this->direction = direction;
    }
    void inputEventHandler(SDL_Event &event) {
        if (event.type == SDL_KEYDOWN) {
            idle = false;
            switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                setDirection(1);
                    translate(-moveSpeed, 0);
                    break;
                case SDLK_RIGHT:
                setDirection(3);
                    translate(moveSpeed, 0);
                    break;
                case SDLK_UP:
                setDirection(0);
                    translate(0, -moveSpeed);
                    break;
                case SDLK_DOWN:
                setDirection(2);
                    translate(0, moveSpeed);
                    break;
            }
        } if(event.type == SDL_KEYUP) {
            idle = true;
        }
    }
    
    void update(){
        animate(direction, idle);
        draw(); // calls spriteobj draw
        // which will then call bitmapobj draw
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

    SDL_Color white = {255, 255, 255, 255};

    string filename = "img/ss.png";  // Use your sprite sheet image here
    Player p1(filename, engine.getRenderer(), 0, 0, 64, 64, 2);
    Rectangle rect;

    rect.createObject(10, 10, 300, 300, &white, engine.getRenderer());

    while (!quit) {
        SDL_SetRenderDrawColor(engine.getRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(engine.getRenderer());
        frameStart = SDL_GetTicks();


        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            p1.inputEventHandler(e);
        }

        rect.draw();

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
