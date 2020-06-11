#include "headers/SwithButton.h"
#include <QPainter>
#include <QtCore/QDebug>

SwitchButton::SwitchButton(QWidget* parent, QString label1, QString label2)
  : QWidget(parent)
  , _value(false)
  , _duration(100)
{
    _background = new SwitchBackground(this, _oncolor);
    _background->resize(20, 20);
    _background->move(2, 2);

    _circle = new SwitchCircle(this, _offcolor);
    _circle->move(2, 2);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(QSize(90, 24));

    _lg = QLinearGradient(35, 30, 35, 0);
    _lg.setColorAt(0, QColor(210, 210, 210));
    _lg.setColorAt(0.25, QColor(255, 255, 255));
    _lg.setColorAt(0.82, QColor(255, 255, 255));
    _lg.setColorAt(1, QColor(210, 210, 210));

    _lg2 = QLinearGradient(50, 30, 35, 0);
    _lg2.setColorAt(0, QColor(230, 230, 230));
    _lg2.setColorAt(0.25, QColor(255, 255, 255));
    _lg2.setColorAt(0.82, QColor(255, 255, 255));
    _lg2.setColorAt(1, QColor(230, 230, 230));

    _pencolor = QColor(120, 120, 120);
    _offcolor = QColor(205, 154, 50);
    _oncolor = QColor(154, 205, 50);

    _tol = 0;
    _borderradius = 12;

    __btn_move = new QPropertyAnimation(this);
    __back_move = new QPropertyAnimation(this);
    __btn_move->setTargetObject(_circle);
    __btn_move->setPropertyName("pos");
    __back_move->setTargetObject(_background);
    __back_move->setPropertyName("size");

    _labeloff = new QLabel(this);
    _labelon = new QLabel(this);
    _labeloff->setText(label1);
    _labelon->setText(label2);
    _labeloff->move(31, 5);
    _labelon->move(15, 5);
    _labeloff->setStyleSheet("color: rgb(120, 120, 120); font-weight: bold;");
    _labelon->setStyleSheet("color: rgb(255, 255, 255); font-weight: bold;");
}

SwitchButton::~SwitchButton(){
    delete _circle;
    delete _background;
    delete _labeloff;
    delete _labelon;
    delete __btn_move;
    delete __back_move;
}

void SwitchButton::paintEvent(QPaintEvent*){
    QPainter* painter = new QPainter;
    painter->begin(this);
    painter->setRenderHint(QPainter::Antialiasing, true);

    QPen pen(Qt::NoPen);
    painter->setPen(pen);

    painter->setBrush(_pencolor);
    painter->drawRoundedRect(0, 0
    , width(), height()
    , 12, 12);

    painter->setBrush(_lg);
    painter->drawRoundedRect(1, 1
    , width() - 2, height() - 2
    , 10, 10);

    painter->setBrush(QColor(210, 210, 210));
    painter->drawRoundedRect(2, 2
    , width() - 4, height() - 4
    , 10, 10);

    painter->setBrush(_lg2);
    painter->drawRoundedRect(3, 3
      , width() - 6, height() - 6
      , 7, 7);
    painter->end();
}
bool SwitchButton::value() const{
    return _value;
}
void SwitchButton::mousePressEvent(QMouseEvent *event){
    if (!_value){
    _labelon->show();
    _labeloff->hide();
    }
    else{
     _labelon->hide();
     _labeloff->show();
    }

    int hback = 20;
    QSize initial_size(hback, hback);
    QSize final_size(width() - 4, hback);

    int xi = 2;
    int y  = 2;
    int xf = width() - 22;

    if (_value){
    final_size = QSize(hback, hback);
    initial_size = QSize(width() - 4, hback);

    xi = xf;
    xf = 2;
    }

    __btn_move->stop();
    __back_move->stop();

    __btn_move->setDuration(_duration);
    __back_move->setDuration(_duration);

    __btn_move->setStartValue(QPoint(xi, y));
    __btn_move->setEndValue(QPoint(xf, y));

    __back_move->setStartValue(initial_size);
    __back_move->setEndValue(final_size);

    __btn_move->start();
    __back_move->start();

    // Assigning new current value
    _value = !_value;
    if(event != nullptr)  emit valueChanged(_value);
}

void SwitchButton::setInitialState(bool isRight){
    if(isRight)
        mousePressEvent(nullptr);
}


SwitchButton::SwitchBackground::SwitchBackground(QWidget* parent, QColor color, bool rect)
    : QWidget(parent)
    , _rect(rect)
    , _borderradius(12)
    , _color(color)
    , _pencolor(QColor(170, 170, 170)){

    setFixedHeight(20);

    _lg = QLinearGradient(0, 25, 70, 0);
    _lg.setColorAt(0, QColor(154, 194, 50));
    _lg.setColorAt(0.25, QColor(154, 210, 50));
    _lg.setColorAt(0.95, QColor(154, 194, 50));

    if (_rect)
    _borderradius = 0;
}
SwitchButton::SwitchBackground::~SwitchBackground(){
}
void SwitchButton::SwitchBackground::paintEvent(QPaintEvent*){

    QPainter* painter = new QPainter;
    painter->begin(this);
    painter->setRenderHint(QPainter::Antialiasing, true);

    QPen pen(Qt::NoPen);
    painter->setPen(pen);
    painter->setBrush(QColor(154, 190, 50));
    painter->drawRoundedRect(0, 0
      , width(), height()
      , 10, 10);

    painter->setBrush(_lg);
    painter->drawRoundedRect(1, 1, width()-2, height()-2, 8, 8);
    painter->end();
}


SwitchButton::SwitchCircle::SwitchCircle(QWidget* parent, QColor color, bool rect)
    : QWidget(parent)
    , _rect(rect)
    , _borderradius(12)
    , _color(color)
    , _pencolor(QColor(120, 120, 120)){

    setFixedSize(20, 20);
    _rg = QRadialGradient(static_cast<int>(width() / 2), static_cast<int>(height() / 2), 12);
    _rg.setColorAt(0, QColor(255, 255, 255));
    _rg.setColorAt(0.6, QColor(255, 255, 255));
    _rg.setColorAt(1, QColor(205, 205, 205));

    _lg = QLinearGradient(3, 18, 20, 4);
    _lg.setColorAt(0, QColor(255, 255, 255));
    _lg.setColorAt(0.55, QColor(230, 230, 230));
    _lg.setColorAt(0.72, QColor(255, 255, 255));
    _lg.setColorAt(1, QColor(255, 255, 255));

}
SwitchButton::SwitchCircle::~SwitchCircle(){
}
void SwitchButton::SwitchCircle::paintEvent(QPaintEvent*){
    QPainter* painter = new QPainter;
    painter->begin(this);
    painter->setRenderHint(QPainter::Antialiasing, true);

    QPen pen(Qt::NoPen);
    painter->setPen(pen);
    painter->setBrush(_pencolor);

    painter->drawEllipse(0, 0, 20, 20);
    painter->setBrush(_rg);
    painter->drawEllipse(1, 1, 18, 18);

    painter->setBrush(QColor(210, 210, 210));
    painter->drawEllipse(2, 2, 16, 16);

    painter->setBrush(_lg);
    painter->drawEllipse(3, 3, 14, 14);

    painter->end();
}
