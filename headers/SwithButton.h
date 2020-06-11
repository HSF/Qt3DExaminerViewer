#ifndef SWITHBUTTON_H
#define SWITHBUTTON_H

#include <QLabel>
#include <QPropertyAnimation>

class SwitchButton : public QWidget
{
  Q_OBJECT
    Q_DISABLE_COPY(SwitchButton)

public:
  explicit SwitchButton(QWidget* parent = nullptr, QString label1="Yes", QString label2="No");
  ~SwitchButton() override;

  //-- QWidget methods
  void mousePressEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent* event) override;
  void setInitialState(bool);

  //-- Getters
  bool value() const;

signals:
  void valueChanged(bool newvalue);

private:
  class SwitchCircle;
  class SwitchBackground;

private:
  bool _value;
  int  _duration;

  QLinearGradient _lg;
  QLinearGradient _lg2;

  QColor _pencolor;
  QColor _offcolor;
  QColor _oncolor;
  int    _tol;
  int    _borderradius;

  // This order for definition is important (these widgets overlap)
  QLabel*           _labeloff;
  SwitchBackground* _background;
  QLabel*           _labelon;
  SwitchCircle*     _circle;

  QPropertyAnimation* __btn_move;
  QPropertyAnimation* __back_move;
};
#endif // SWITHBUTTON_H


class SwitchButton::SwitchBackground : public QWidget
{
  Q_OBJECT
    Q_DISABLE_COPY(SwitchBackground)

public:
  explicit SwitchBackground(QWidget* parent = nullptr, QColor color = QColor(154, 205, 50), bool rect = false);
  ~SwitchBackground() override;

  //-- QWidget methods
  void paintEvent(QPaintEvent* event) override;

private:
  bool            _rect;
  int             _borderradius;
  QColor          _color;
  QColor          _pencolor;
  QLinearGradient _lg;
};


class SwitchButton::SwitchCircle : public QWidget
{
  Q_OBJECT
    Q_DISABLE_COPY(SwitchCircle)

public:
  explicit SwitchCircle(QWidget* parent = nullptr, QColor color = QColor(255, 255, 255), bool rect = false);
  ~SwitchCircle() override;

  //-- QWidget methods
  void paintEvent(QPaintEvent* event) override;

private:
  bool            _rect;
  int             _borderradius;
  QColor          _color;
  QColor          _pencolor;
  QRadialGradient _rg;
  QLinearGradient _lg;
};
