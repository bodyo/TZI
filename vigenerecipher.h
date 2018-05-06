#ifndef VIGENERECIPHER_H
#define VIGENERECIPHER_H

#include <QWidget>

class QPushButton;
class OpenAndCodedText;
class QLineEdit;

class VigenereCipher : public QWidget
{
    Q_OBJECT
public:
    VigenereCipher(QWidget *parent = nullptr);

private slots:
    void decode();
    void code();

private:
    OpenAndCodedText *m_openText;
    QPushButton *m_decode;
    QPushButton *m_code;
    QLineEdit *m_lineEdit;
};

#endif // VIGENERECIPHER_H
