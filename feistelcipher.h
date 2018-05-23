#ifndef FEISTELCIPHER_H
#define FEISTELCIPHER_H

#include <QWidget>

class QPushButton;
class OpenAndCodedText;
class QLineEdit;
class QSpinBox;

class FeistelCipher : public QWidget
{
    Q_OBJECT
public:
    explicit FeistelCipher(QWidget *parent = nullptr);

private:
    int f(int L, int n, int size);

public slots:
    void decode();
    void code();

private:
    OpenAndCodedText *m_openText;
    QPushButton *m_decode;
    QPushButton *m_code;
    QLineEdit *m_lineEdit;
    QSpinBox *m_spinBox;
};

#endif // FEISTELCIPHER_H
