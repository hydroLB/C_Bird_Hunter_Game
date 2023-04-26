#include "graphics.h"
using namespace std;

GLdouble width, height;
int wd;
const color skyBlue(77/255.0, 213/255.0, 240/255.0);
const color grassGreen(26/255.0, 176/255.0, 56/255.0);
const color white(1, 1, 1);
const color brickRed(201/255.0, 20/255.0, 20/255.0);
const color darkBlue(1/255.0, 110/255.0, 214/255.0);
const color purple(119/255.0, 11/255.0, 224/255.0);
const color black(0, 0, 0);
const color magenta(1, 0, 1);
const color orange(1, 163/255.0, 22/255.0);
const color cyan (0, 1, 1);

vector<unique_ptr<Shape>> clouds;
Rect grass;
Rect user;
FreeShape gun;
Circle gunWheel;
vector<unique_ptr<Shape>> bullets;
vector<Rect> birds;
int SIDE_LENGTH = 1,
    bulletNum, bulletX, bulletY, gunAngle,
    birdX, birdY,
    birdSpeed,
    curScreen,
    kills;


enum Screen{start=0, play=1, end=2};
string message1 = "Welcome to Migration Mitigation! Press 's' to begin",
       message2 = "Thank you for playing",
       message3,
       message4 = "Press 'p' to play again or esc to exit",
       message5 = "Use the arrow keys to move and space to shoot!";

void initClouds() {
    // Note: the Rect objects that make up the flat bottom of the clouds
    // won't appear until you implement the Rect::draw method.
    clouds.clear();
    dimensions cloudBottom(30, 30);
    // First cloud
    clouds.push_back(make_unique<Circle>(white, 300, 100, 20));
    clouds.push_back(make_unique<Circle>(white, 330, 100, 20));
    clouds.push_back(make_unique<Circle>(white, 320, 90, 20));
    clouds.push_back(make_unique<Rect>(white, 315, 105, cloudBottom));
    // Second cloud
    clouds.push_back(make_unique<Circle>(white, 100, 80, 20));
    clouds.push_back(make_unique<Circle>(white, 130, 80, 20));
    clouds.push_back(make_unique<Circle>(white, 120, 70, 20));
    clouds.push_back(make_unique<Rect>(white, 115, 85, cloudBottom));
    // Third cloud
    clouds.push_back(make_unique<Circle>(white, 450, 50, 20));
    clouds.push_back(make_unique<Circle>(white, 480, 50, 20));
    clouds.push_back(make_unique<Circle>(white, 470, 40, 20));
    clouds.push_back(make_unique<Rect>(white, 465, 55, cloudBottom));

}

void initBullets() {
    dimensions bulletSize(2,2);
    bullets.clear();

    bullets.push_back(make_unique<Circle>(black, bulletX, bulletY, 4));
    bullets.push_back(make_unique<Rect>(black, bulletX, bulletY,bulletSize));

    bullets.push_back(make_unique<Circle>(black, bulletX, bulletY, 4));
    bullets.push_back(make_unique<Rect>(black, bulletX, bulletY,bulletSize));

    bullets.push_back(make_unique<Circle>(black, bulletX, bulletY, 4));
    bullets.push_back(make_unique<Rect>(black, bulletX, bulletY,bulletSize));

    bullets.push_back(make_unique<Circle>(black, bulletX, bulletY, 4));
    bullets.push_back(make_unique<Rect>(black, bulletX, bulletY,bulletSize));

    bullets.push_back(make_unique<Circle>(black, bulletX, bulletY, 4));
    bullets.push_back(make_unique<Rect>(black, bulletX, bulletY,bulletSize));

    bullets.push_back(make_unique<Circle>(black, bulletX, bulletY, 4));
    bullets.push_back(make_unique<Rect>(black, bulletX, bulletY,bulletSize));

}

void initGrass() {
    grass.setCenter(250, 450);
    grass.setSize(width, height/3);
    grass.setColor(grassGreen);
}

void initGun() {
    gunAngle = 0;
    gun.setCenter({24,360});
    gun.setPoints({60,355},{60,365},{20,355},{20,365});
    gunWheel.setColor(0,0,0,.9);
    gunWheel.setRadius(12);
    gunWheel.setCenter(24, 360);
}

void initUser() {
    user.setCenter(10,10.0);
    user.setSize(20.0, 20.0);
    user.setColor(white);
}

void initBirds() {
    dimensions birdSize(56/2, 30/2);
    int speed;
    birdSpeed = 40;
    for(int i=0;i<10;i++) {
        birdX = rand() % 500 + 700;
        birdY = rand() % 320;
        speed = ((rand() % birdSpeed + birdSpeed/5) / 10.0);
        birds.push_back(Rect(0, 0, 0, 0, birdX, birdY, birdSize));
        birds[i].setSpeed(speed);
        birds[i].setFalling(false);
        cout << birds[i].getSpeed();
    }
}

//resets birds position and speed and kill count
void resetBirds() {
    birdSpeed = 40;
    kills=0;
    for (Rect &b:birds) {
        b.setCenter(rand() % 500 + 700, rand() % 320);
        b.setSpeed((rand() % birdSpeed + birdSpeed / 5) / 10.0);
        b.setFalling(false);
        b.setYVelocity(0);
    }
}

void init() {
    width = 500;
    height = 500;
    bulletNum = 0;
    bulletX = 24;
    bulletY = 360;
    srand(time(0));
    initClouds();
    initGrass();
    initUser();
    initGun();
    initBullets();
    initBirds();
    curScreen=0;
    kills=0;
}

//Shoots the gun
void shoot(std::vector<std::unique_ptr<Shape>>& buls, int &bNum, double xVel, double yVel) {
    if (buls[bNum]->getXVelocity() == 0 && buls[bNum]->getYVelocity() == 0) {
        buls[bNum]->setVelocity(xVel, yVel);
        buls[bNum + 1]->setVelocity(xVel, yVel);
        if (bNum < 10)
            bNum += 2;
        else
            bNum = 0;
    }
}

//draws a bird using pixel art where a hitbox is
void drawBird(Rect b) {
    char letter;
    bool draw;
    int xCoord, yCoord;
    xCoord = b.getLeftX();
    yCoord = b.getTopY();
    ifstream inFile("../scene.txt");
    inFile >> noskipws;
    while (inFile >> letter) {
        draw = true;
        switch (letter) {
            case 'g':
                glColor3f(0, .25, .5);
                break;
            case 'b':
                glColor3f(0, 0, .5);
                break;
            case 'y':
                glColor3f(1, 1, 0);
                break;
            case ' ':
                glColor4f(0, 0, 0, 0);
                break;
            default: // newline
                draw = false;
                xCoord = b.getLeftX();;
                yCoord += SIDE_LENGTH;
        }
        if (draw) {
            glBegin(GL_QUADS);
            glVertex2i(xCoord, yCoord);
            glVertex2i(xCoord + SIDE_LENGTH, yCoord);
            glVertex2i(xCoord + SIDE_LENGTH, yCoord + SIDE_LENGTH);
            glVertex2i(xCoord, yCoord + SIDE_LENGTH);
            glEnd();
            xCoord += SIDE_LENGTH;
        }
    }
    inFile.close();
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(skyBlue.red, skyBlue.green, skyBlue.blue, 1.0f);
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // Tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height); // DO NOT CHANGE THIS LINE (unless you are running Catalina on Mac)
    
    // Do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION); // DO NOT CHANGE THIS LINE
    glLoadIdentity(); // DO NOT CHANGE THIS LINE
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f); // DO NOT CHANGE THIS LINE

    // Clear the color buffer with current clearing color
    glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DO NOT CHANGE THIS LINE
    
    /*
     * Draw here
     */

    if (curScreen == Screen::start){

        grass.draw();

        for (unique_ptr<Shape> &s: clouds) {
            // #polymorphism
            s->draw();
        }

        for (Rect &b: birds) {
            drawBird(b);
            b.draw();
        }

        glColor4f(0, 0, 0, 1);
        glRasterPos2i(width/2 - (4 * message5.length()), 170);
        for (const char &letter: message5) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }

        glColor4f(0, 0, 0, 1);
        glRasterPos2i(width/2 - (4 * message1.length()), 150);
        for (const char &letter: message1) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }

    } else if (curScreen == Screen::end) {
        message3 = "You killed " + to_string(kills) + " birds!",
        glColor4f(0, 0, 0, 1);
        glRasterPos2i(width/2 - (4 * message2.length()), 100);
        for (const char &letter: message2) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }

        glRasterPos2i(width/2 - (4 * message3.length()), 130);
        for (const char &letter: message3) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }

        glRasterPos2i(width/2 - (4 * message4.length()), 160);
        for (const char &letter: message4) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }

    } else {

        grass.draw();

        for (unique_ptr<Shape> &s: clouds) {
            // #polymorphism
            s->draw();
        }


        for (Rect &b: birds) {
            drawBird(b);
            b.draw();
        }

        for (int i = 1; i < bullets.size(); i += 2) {
            for (Rect &bird: birds) {
                if (dynamic_cast<Rect &>(*bullets[i]).isOverlapping(bird)) {
                    bird.setFalling(true);
                }
            }
        }

        for (unique_ptr<Shape> &b: bullets) {
            // #polymorphism
            b->draw();
        }

        gun.draw({0.15, 0.15, 0.15, 1});
        gunWheel.draw();

        message5 = "Kills: " + to_string(kills);
        glColor4f(0, 0, 0, 1);
        glRasterPos2i(30, 480);
        for (const char &letter: message5) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }

    }
        glFlush();  // Render now
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y) {
    // escape
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }

    if (key == 's') {
        if (curScreen == 0) {
            curScreen = 1;
            resetBirds();
        }
    }

    if (key == 'p') {
        if (curScreen == 2) {
            curScreen = 1;
            resetBirds();
        }
    }

    //shoot the gun
    if (key == ' ') {
        shoot(bullets, bulletNum, (gun.getPoints()[1].x - gun.getPoints()[3].x)/10, (gun.getPoints()[1].y - gun.getPoints()[3].y)/10);
    }
    
    glutPostRedisplay();
}

void kbdS(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_DOWN:
            if (gunAngle <= 50 && gunAngle > 0) {
                gun.rotate(PI / 100.0, true);
                gunAngle--;
            }
            break;
        case GLUT_KEY_LEFT:
            
            break;
        case GLUT_KEY_RIGHT:
            
            break;
        case GLUT_KEY_UP:
            if (gunAngle < 50 && gunAngle >= 0) {
                gun.rotate(PI / 100.0, false);
                gunAngle++;
            }
            break;
    }
    
    glutPostRedisplay();
}

void cursor(int x, int y) {
    // Set the user's center point to be the coordinates
    // passed in as parameters to this function. This will make
    // the user block move with the mouse.
    user.setCenter(x, y);
    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {

    glutPostRedisplay();
}

void cloudTimer(int dummy) {

    for (unique_ptr<Shape> &s : clouds) {
        // Move all the clouds to the left
        s->move(-1,0);
        // If a shape has moved off the screen
        if (s->getCenterX() < -20) {
            // Set it to the right of the screen so that it passes through again
            s->setCenterX(520);
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(50, cloudTimer, dummy);
}

//controls the bird sprite
void birdTimer(int dummy) {

    for (Rect &b: birds) {
        if (b.getFalling()) {
            // Move the birds down if falling
            b.move(0, b.getYVelocity());
            // If a shape has moved off the screen
            b.setYVelocity(b.getYVelocity() + 1);
            if (b.getCenterY() > 500) {
                // Set it to the right of the screen so that it passes through again
                b.setCenter(rand() % 500 + 700, rand() % 320);
                b.setSpeed((rand() % birdSpeed + birdSpeed/5) / 10.0);
                b.setFalling(false);
                b.setYVelocity(0);
                birdSpeed++;
                kills++;
            }
        } else  if (curScreen == 1){
            // Move all the birds to the left
            b.move(-b.getSpeed(), 0);
            // If a shape has moved off the screen
            if (b.getCenterX() < -50) {
                curScreen=2;
            }
        } else {
            b.move(-b.getSpeed(), 0);
            if (b.getCenterX() < -50) {
                b.setCenter(rand() % 500 + 700, rand() % 320);
                //get a random number here
                b.setSpeed((rand() % birdSpeed + birdSpeed / 5) / 10.0);
            }
        }

    }

    glutPostRedisplay();
    glutTimerFunc(40, birdTimer, dummy);
}

void buildingTimer(int dummy) {

    for (unique_ptr<Shape> &b : bullets) {
        b->move(b->getXVelocity(), b->getYVelocity());
        if(b->getXVelocity()!=0 || b->getYVelocity()!=0)
            b->setVelocity(b->getXVelocity(), b->getYVelocity() + .02);
        if (b->getCenterX() < 0) {
            b->setVelocity(0,0);
            b->setCenter(bulletX, bulletY);
        } else if (b->getCenterX() > (width)) {
            b->setVelocity(0,0);
            b->setCenter(bulletX, bulletY);
        }
        if (b->getCenterY() > (370)) {
            b->setVelocity(0,0);
            b->setCenter(bulletX, bulletY);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(5, buildingTimer, dummy);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    
    init();
    
    glutInit(&argc, argv);          // Initialize GLUT
    
    glutInitDisplayMode(GLUT_RGBA);
    
    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(100, 200); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Bird Shooter" /* title */ );
    
    // Register callback handler for window re-paint event
    glutDisplayFunc(display);
    
    // Our own OpenGL initialization
    initGL();
    
    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);
    
    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);
    
    // handles mouse movement
    glutPassiveMotionFunc(cursor);
    
    // handles mouse click
    glutMouseFunc(mouse);
    
    // handles timer
    glutTimerFunc(0, cloudTimer, 0);
    glutTimerFunc(0, buildingTimer, 0);
    glutTimerFunc(0, birdTimer, 0);
    
    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}
