#ifndef CAESARCIPHER_H
#define CAESARCIPHER_H

#include <QWidget>

class QTextEdit;
class QPushButton;
class OpenAndCodedText;
class QSpinBox;

class CaesarCipher : public QWidget
{
public:
    CaesarCipher(QWidget *parent = nullptr);

private:
    void setupUi();

private slots:
    void decode();

private:
    OpenAndCodedText *m_openText;
    QPushButton *m_decode;
    QSpinBox *m_spinBox;
};

#endif // CAESARCIPHER_H
