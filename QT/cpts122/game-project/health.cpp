/* Project: Java Defender (Andy's Face In Space Defending the Base)
 * Programmers: Arrin Bevers, Kiernan, Ace Cassidy, Garrick Hutcheson
 * Date: 12/6/17
 * File: Health.cpp
 *
 * Citation: Abdullah Aghazada's Youtube Tutorial Series: "C++ Qt Game Tutorial"
 * Link to first Video of series: https://www.youtube.com/watch?v=9lqhMLFHj3A&list=PLMgDVIa0Pg8WrI9WmZR09xAbfXyfkqKWy
 * Used these videos to get us started learning how to use the Qt library to make a game.
 */


#include "Health.h"
#include <QFont>

Health::Health(QGraphicsItem *parent): QGraphicsTextItem(parent){
    // initialize the score to 0
    health = 10;

    // draw the text
    setPlainText(QString("Health: ") + QString::number(health)); // Health: 3
    setDefaultTextColor(Qt::red);
    setFont(QFont("times",16));
}

void Health::decrease(){
    health--;
    setPlainText(QString("Health: ") + QString::number(health)); // Health: 2
    if(health == 0 ){
        exit(1);
    }
}

int Health::getHealth(){
    return health;
}

