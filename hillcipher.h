#ifndef HILLCIPHER_H
#define HILLCIPHER_H

#include <QWidget>

class QPushButton;
class OpenAndCodedText;
class QLineEdit;
template <int N, int M, typename T>
class QGenericMatrix;

class HillCipher : public QWidget
{
    Q_OBJECT
public:
    explicit HillCipher(QWidget *parent = nullptr);

public slots:
    void decode();
    void code();

private:
    static qreal determinant(QGenericMatrix<3,3,float> &matrix);
    static int gcd(int a, int b, int & x, int & y);
    static QGenericMatrix<3,3,float> reverseKeyMatrix(QGenericMatrix<3,3,float> &matrix);

private:
    OpenAndCodedText *m_openText;
    QPushButton *m_decode;
    QPushButton *m_code;
    QLineEdit *m_lineEdit;
};

#endif // HILLCIPHER_H
