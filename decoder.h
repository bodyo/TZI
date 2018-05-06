#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QTabWidget;

class Decoder : public QWidget
{
    Q_OBJECT

public:
    Decoder(QWidget *parent = nullptr);
    ~Decoder();

private:
    QTabWidget *m_tabWidget{nullptr};
};

#endif // WIDGET_H
