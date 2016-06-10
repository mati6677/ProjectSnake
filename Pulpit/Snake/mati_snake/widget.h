#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPaintEvent> //obsługa funkcju rysowania
#include <QKeyEvent> //obsługa przycisków

const int BLOCK_SIZE=25; //wielkość pojedyńczego bloku
const int MARGIN=5; //rozmiar marginesu
const int AREA_ROW=15; //ilość bloków w wierszu
const int AREA_COL=15; //ilosć bloków w kolumnie

const int TIME_INTERVAL=150; //częstotliwosć odświeżania gry

enum Direction //przyciski do poruszania się wężem
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    virtual void paintEvent(QPaintEvent *event); //funkcja rysująca poszczególne elementy
    virtual void keyPressEvent(QKeyEvent *event); //funkcja oprogramywująca poszczególne przyciski

public:
    void InitGame(); //rozpoczęcie gry
    void PauseResumeGame(); //pauza i wznowienie gry
    void GameOver(); //koniec gry
    void Winner();
    void GenerateFood(); //losowe wyswietlanie punktów do zbierania
    bool IsGameOver(); //aktywacja funkcji GameOver()
    bool isWin; //aktywacja funkcji Winner();
    bool isPause; //aktywacja/dezaktywacja funkcji PauseResumeGame()

private slots:
    void SnakeUpdate(); //aktualizacja rozmiaru(długości) węża

private:
    QTimer *gameTimer; //timery do odświżania
    QPoint foodPoint; //punkty do zbierania
    QList<QPoint> snake; //struktura węża
    Direction dir; //poruszanie się
    int score; //licznik punktów
};

#endif // WIDGET_H
