/***********************************************************************
 *
 * Author: Deyuan Li
 * Contact: magicleadery@gmail.com
 * Last Update: May 28th, 2014
 * Feature(s) added: provide different speed
 * Purpose: The snake game using OpenGL
 * 
 * BEFORE install the game, make sure OpenGL libary is pre-installed.
 * If not, following link may be helpful.
 * http://www.opengl.org/wiki/Getting_Started
 * http://www.wikihow.com/Install-Mesa-(OpenGL)-on-Linux-Mint
 *
 * Install: make snake
 * Delete:  make clean
 * Run: ./snake <SPEED>
 * - SPEED should be in the range of [1,10]
 * Control:
 * keyboard: left, right, up, down to change the direction of the snake.
 * 
 * Any advice would be appreciated! ^_^
 *
************************************************************************/
#include"snake.h"
#include"graph.cc"
Snake snake;
Apple apples;
Graph graph(mapSize);
int route[mapSize];
vector<int> shortRoute;
Axis::Axis(int a, int b)
{
    x = a;
    y = b;
}
Axis::Axis()
{
    x = 1 * speed;
    y = 0 * speed;
}
void Axis::setPosition(int a, int b)
{
    x = a;
    y = b;
}
bool Axis::equal(Axis p)
{
    if(x==p.getX() && y==p.getY())
        return true;
    else
        return false;
}
void Axis::increase(int a, int b)
{
    x += a;
    y += b;
}
void Axis::decrease(int a, int b)
{
    x -= a;
    y -= b;
}
Axis::Axis(int a)
{
    x = (a%((BORDERRIGHT-BORDERLEFT)/THICKNESS+1))*THICKNESS+BORDERLEFT;
    y = (a/((BORDERRIGHT-BORDERLEFT)/THICKNESS+1))*THICKNESS+BORDERBOTTOM;
}
int Axis::toSerial()
{
    return((y-BORDERBOTTOM)/THICKNESS*((BORDERRIGHT-BORDERLEFT)/THICKNESS+1)
            +(x-BORDERLEFT)/THICKNESS);
}
Apple::Apple()
{
    apple_all.setPosition(2*THICKNESS,2*THICKNESS);
}
void Apple::generate()
{
    int x,y;
    do
    {
        x = (rand()%((int)((BORDERRIGHT-BORDERLEFT+THICKNESS)/THICKNESS)))*THICKNESS+BORDERLEFT;
        y = (rand()%((int)((BORDERTOP-BORDERBOTTOM+THICKNESS)/THICKNESS)))*THICKNESS+BORDERBOTTOM;
    }
    while(snake.isBody(Axis(x,y))||snake.isHead(Axis(x,y)));
    apple_all.setPosition(x,y);
}
Axis Apple::getPosition()
{
    //TODO return the position of the apple;
    return apple_all;
}

void Apple::display()
{

    cout<<"-----------------------"<<apple_all.getX()<<","<<apple_all.getY()<<"-----------------------"<<endl;
    glRectf(apple_all.getX()+2, apple_all.getY()-2, apple_all.getX()-2+THICKNESS, apple_all.getY()+2-THICKNESS);
}

bool Snake::isBody(Axis a)
{
    list<Axis>::iterator i = body.begin();
    ++i;
    for(;i!=body.end();++i)
    {
        if(a.equal(*i))
            return true;
    }
    return false;
}
bool Snake::isHead(Axis a)
{
    if(a.equal(body.front()))
        return true;
    else
        return false;
}

Snake::Snake()
{
    body.push_back(Axis( 0, 0));
    body.push_back(Axis( -THICKNESS, 0));
    body.push_back(Axis(-2*THICKNESS, 0));
    length = 3;
}

void Snake::display()
{
    for(auto i=body.begin(); i!=body.end(); ++i)
    {
        glRectf(i->getX(), i->getY(), i->getX()+THICKNESS, i->getY()-THICKNESS);
    }
}

void Snake::move()
{
    if(counter == counterThre)
    {
        direction.setPosition(nextDirection.getX(),nextDirection.getY());
        nextStep.setPosition(body.begin()->getX()+direction.getX(), body.begin()->getY()+direction.getY());
        /*    if((int)nextStep.getX()%(int)THICKNESS == 0 && (int)nextStep.getY()%(int)THICKNESS == 0)
              {*/
        cout<<"MOVING TO ["<<nextStep.getX()<<","<<nextStep.getY()<<"] ("<<nextStep.toSerial() << ')' <<endl;
        for(auto i = body.begin(); i!=body.end(); ++i)
        {
            //    i->decrease(counter*direction.getX(),counter*direction.getY());
            cout<<i->getX()<<","<<i->getY()<<endl;
        }
        cout<<"Length: "<<snake.getLen()<<endl;
        // counter = 0;
        // Eat apple, size increases by one

        if(nextStep.equal(apples.getPosition()))
        {
            body.push_front(nextStep);
            ++length;
            apples.generate();
        }
        // Didn't eat the apple
        else
        {
            for(auto i = body.begin(); !(i->equal(body.back())); ++i)
            {
                // Hit the border or eat itself, GAME OVER
                if(i->equal(nextStep) || nextStep.getX()>BORDERRIGHT 
                        || nextStep.getX()<BORDERLEFT
                        || nextStep.getY()<BORDERBOTTOM
                        || nextStep.getY()>BORDERTOP)
                {
                    isDead();
                }
                // Move forward
            }

            body.push_front(nextStep);
            body.pop_back();
        }
        counter = 0;
        cout<<"MOVED"<<endl;
    }
    else
        ++counter;
    /*    }
          else
          {
          for(auto i = body.begin(); i != body.end(); ++i)
          {
          i->increase(direction.getX(),direction.getY());
          }
          ++counter;
          }*/
}
void Snake::autoDirect()
{
    if(counter == counterThre)
    {
        if(direction.getY() > 0 && body.begin()->getY() == BORDERTOP)
            changeDirection(1, 0);
        else if(direction.getX() > 0 && body.begin()->getY() == BORDERTOP)
            changeDirection(0, -1);
        else if(direction.getY() < 0 && body.begin()->getY() == (BORDERBOTTOM + THICKNESS)
                && body.begin()->getX() != BORDERRIGHT)
            changeDirection(1, 0);
        else if(direction.getX() > 0 && body.begin()->getY() == (BORDERBOTTOM + THICKNESS))
            changeDirection(0, 1);
        else if(direction.getX() > 0 && body.begin()->getX() == BORDERRIGHT)
            changeDirection(0, -1);
        else if(direction.getY() < 0 && body.begin()->getY() == BORDERBOTTOM)
            changeDirection(-1, 0);
        else if(direction.getX() < 0 && body.begin()->getX() == BORDERLEFT)
            changeDirection(0, 1);
    }
}
void Snake::autoDirectV2()
{
    if(counter == counterThre)
    {
        if(shortRoute.empty())
        {
            cout<<"Calculating the route..."<<endl;
            for(int i = 0; i < mapSize; ++i)
            {
                route[i] = 0;
            }
            graph.renew();
            setGraph(graph); 
            int snakeHead = body.begin()->toSerial();
            int dest = apples.getPosition().toSerial();
            cout<<"Head,Apple:"<<snakeHead<<' '<<dest<<endl;
            if(BFS(graph,route,snakeHead,dest))
            {
                
                shortRoute.push_back(dest);
                while(route[dest] != dest)
                {
                    shortRoute.push_back(route[dest]);
                    dest = route[dest];
                }
                shortRoute.pop_back();
                
                for(vector<int>::reverse_iterator i = shortRoute.rbegin(); i != shortRoute.rend(); ++i)
                    cout << *i << endl;
            }
        }
        if(!shortRoute.empty())
        {
            Axis nextStep(shortRoute.back());
            shortRoute.pop_back();
            changeDirection((nextStep.getX()-body.begin()->getX())/speed, (nextStep.getY()-body.begin()->getY())/speed);
        }
        else
        {
            isDead();
        }
    }
}



int Snake::getLen()
{
    return length;
}

void Snake::isDead()
{
    //TODO GAME OVER
    cout<<"GAME OVER"<<endl;
    exit(0);
}

void Snake::changeDirection(int a, int b)
{
    //TODO change the movement direction
    if((direction.getX() + a*speed == 0) || (direction.getY() + b*speed == 0))
        return;
    else
        nextDirection.setPosition(a*speed,b*speed);
}



void init(void) 
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_FLAT);
}


void display()
{
//    if(counterThre == 0)
//        cin>>counterThre;
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    //glRotatef(spin, 0.0, 0.0, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    // outline
    /*
       glRectf(-25.0, 25.0, 25.0 ,24.0);
       glRectf(-25.0, 25.0, -24.0, -25.0);
       glRectf(-25.0, -24.0, 25.0, -25.0);
       glRectf(24.0, 25.0, 25.0, -25.0);*/
    snake.display();
    apples.display();
    glFlush();
    glPopMatrix();
    glutSwapBuffers();
}

void spinDisplay(int)
{
    //The first version of auto-directing
    //snake.autoDirect();
    //The second version of auto-directing
    //snake.autoDirectV2();
    snake.move();
    glutPostRedisplay();
    //    snake.autoDirect();
    glutTimerFunc(20, spinDisplay, 0);
}

void reshape(int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(BORDERLEFT, BORDERRIGHT+THICKNESS, BORDERBOTTOM-THICKNESS, BORDERTOP, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouse(int button, int state, int x, int y) 
{
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
                spinDisplay(0);
            break;
        case GLUT_MIDDLE_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(NULL);
            break;
        default:
            break;
    }
}

void keyboard(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            snake.changeDirection(0, 1); break;
        case GLUT_KEY_DOWN:
            snake.changeDirection(0, -1); break;
        case GLUT_KEY_LEFT:
            snake.changeDirection(-1, 0); break;
        case GLUT_KEY_RIGHT:
            snake.changeDirection(1, 0); break;
    }
}

void setGraph(Graph &G)
{
    for(int i = 0; i < mapSize; ++i)
    {
        if(snake.isBody(Axis(i)))
            G.visit(i);
    }
    for(int i = 0; i < xLength; ++i)
        for(int j = 0; j < yLength; ++j)
        {
            if(G.isVisited(j*xLength+i) == false)
            {
                if(j > 0 && G.isVisited(xLength*(j-1)+i) == false)
                    G.setEdge(xLength*j+i,xLength*(j-1)+i,1);
                if(j < yLength-1 && G.isVisited(xLength*(j+1)+i) == false)
                    G.setEdge(xLength*j+i,xLength*(j+1)+i,1);
                if(i > 0 && G.isVisited(xLength*j+i-1) == false)
                    G.setEdge(xLength*j+i,xLength*j+i-1,1);
                if(i < xLength-1 && G.isVisited(xLength*j+i+1) == false)
                    G.setEdge(xLength*j+i,xLength*j+i+1,1);
            }
        }
}


/* 
 *  Request double buffer display mode.
 *  Register mouse input callback functions
 */
int main(int argc, char** argv)
{
    counterThre = 11 - atoi(argv[1]);
    if(counterThre <= 0 || counterThre > 10)
        return 0;
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (400, 400); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard); 
//    glutMouseFunc(mouse);
    spinDisplay(0);
    glutMainLoop();
    return 0;
}
