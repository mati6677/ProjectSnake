#include <QPainter> //funkcje rysujęce poszczególne elementy
#include <time.h> //czas systemowy
#include <QTimer> //timery do odśiwżania
#include <QString> //obsługa napisów na okienkach
#include <QMessageBox> //obsłoga okienek z napisami
#include "widget.h"


Widget::Widget(QWidget *parent) : QWidget(parent)
{
    //inicjalizacja rozmairu sceny
    resize(MARGIN*4+(AREA_COL+3)*BLOCK_SIZE,MARGIN*2+AREA_ROW*BLOCK_SIZE);
    //rozpoczęcie gry
    InitGame();
}

//domyślny destruktor
Widget::~Widget()
{

}

void Widget::InitGame()
{
    //inicjalizacja węża (początkowa długość to 3)
    for(int j=2;j>=0;j--)
        snake.push_back(QPoint(j,0));
    //początkowy kierunke węża po rozpoczęciu gry to w prawo
    dir=RIGHT;
    //inicjowaie punktów
    srand(time(0));
    GenerateFood();
    //początkowa ilość punktów
    score=0;
    //początkowy stan aktywności funkcji pauzy
    isPause=false;
    isWin=false;
    //inicjowanie timera
    gameTimer=new QTimer(this);
    connect(gameTimer,SIGNAL(timeout()),this,SLOT(SnakeUpdate()));
    gameTimer->start(TIME_INTERVAL);
}

void Widget::GenerateFood()
{
    //losowe miejsca pojawiania się punktów
    foodPoint.setX(rand()%AREA_COL);
    foodPoint.setY(rand()%AREA_ROW);
    //jeżeli pozycja węża jest inna niż pozycja wygenerwanego punktu, wygeneruj punkt
    if(snake.contains(foodPoint))
        GenerateFood();
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //scena gry (Brush - wypełenienie, Pen - obramowanie)
    painter.setBrush(Qt::yellow);
    painter.setPen(Qt::red);
    painter.drawRect(MARGIN,MARGIN,AREA_COL*BLOCK_SIZE,AREA_ROW*BLOCK_SIZE);
    //wąż
    painter.setBrush(Qt::red);
    painter.setPen(Qt::white);
    for(int i=0;i<snake.size();i++)
        painter.drawRect(MARGIN+snake[i].x()*BLOCK_SIZE,MARGIN+snake[i].y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
    //punkty(owoce)
    painter.setBrush(Qt::green);
    painter.setPen(Qt::green);
    painter.drawEllipse(MARGIN+foodPoint.x()*BLOCK_SIZE,MARGIN+foodPoint.y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
    //licznik punktów
    painter.setPen(Qt::black);
    //wybór czcionki i jej rozmiaru
    painter.setFont(QFont("Arial",14));
    painter.drawText(MARGIN*3+AREA_COL*BLOCK_SIZE,MARGIN+2*BLOCK_SIZE,"score: "+QString::number(score));
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    //oprogamowanie przycisków do poruszania się wężem
    case Qt::Key_Up:
        if(dir!=DOWN)
            dir=UP;
        break;
    case Qt::Key_Down:
        if(dir!=UP)
            dir=DOWN;
        break;
    case Qt::Key_Left:
        if(dir!=RIGHT)
            dir=LEFT;
        break;
    case Qt::Key_Right:
        if(dir!=LEFT)
            dir=RIGHT;
        break;
    //oprogramowanie przycisku do aktywacji pauzy
    case Qt::Key_P:
        PauseResumeGame();
        break;
    default:
        break;
    }
}

bool Widget::IsGameOver()
{
    int x=snake.front().x();
    int y=snake.front().y();
    //granice poruszania się wężem
    if(x<0||x>AREA_COL-1||y<0||y>AREA_ROW-1)
        return true;
    //sprawdzanie czy wąż znazł się poza granicją
    for(int i=3;i<snake.size();i++)
        if(snake[i]==snake.front())
            return true;
    return false;
}

void Widget::SnakeUpdate()
{
    //ruchy węża
    switch(dir)
    {
    case UP:
        snake.push_front(QPoint(snake.front().x(),snake.front().y()-1));
        break;
    case DOWN:
        snake.push_front(QPoint(snake.front().x(),snake.front().y()+1));
        break;
    case LEFT:
        snake.push_front(QPoint(snake.front().x()-1,snake.front().y()));
        break;
    case RIGHT:
        snake.push_front(QPoint(snake.front().x()+1,snake.front().y()));
        break;
    default:
        break;
    }
    //sprawdzenie, czy zebrano puknt(owoc)
    if(snake.contains(foodPoint))
    {
        //jeżeli zebrano owoc to zwiększ ilość pkt o 1
        score+=1;
        //wygeneruj nowy owoc
        GenerateFood();
        //sprawdzenie czy zebrano 33 punkty(owoce), jeżeli tak, zakończ grę
        if(score==33)
                isWin=true;
    }
    else
        snake.pop_back();
    //sprawdza czy aktywowano funkcję GameOver();
    if(IsGameOver())
    {
        GameOver();
        exit(0); //koniec gry (porażka)
    }
    if(isWin)
    {
        Winner();
        exit(0); //koniec gry (zwycięstwo)
    }
    //aktualizacja stanu gry
    update();
}

void Widget::GameOver()
{
    gameTimer->stop();
    //wyswietlenie informacji o porażce
    QMessageBox::information(this,"failed","game over!");
}

void Widget::Winner()
{
    gameTimer->stop();
    //wyswietlenie informacji o zwycięstwie
    QMessageBox::information(this,"well done","you win!");
}

void Widget::PauseResumeGame()
{
    //sprawdzenie czy aktywowano pauze
    if(!isPause)
    {
        isPause=!isPause;
        //jeżeli tak, zatrzymaj timer
        gameTimer->stop();
    }
    else
    {
        isPause=!isPause;
        //jeżeli zdezaktywowano wznów timer
        gameTimer->start(TIME_INTERVAL);
    }
}
