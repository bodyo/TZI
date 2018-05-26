#ifndef RSACIPHER_H
#define RSACIPHER_H

#include <QWidget>

class QPushButton;
class OpenAndCodedText;
class QSpinBox;
class QLabel;

class RSACipher : public QWidget
{
    Q_OBJECT
public:
    explicit RSACipher(QWidget *parent = nullptr);

signals:

public slots:
    void code();
    void decode();

private:
    static int phi(int n);
    static int xGCD(int a, int b, int &x, int &y);
    static int gcd(int a, int b);
    static long int calculateD( long int e, long int t);
    long encrypt(int i);
    long crypt(long i);

private:
    OpenAndCodedText *m_openText;
    QPushButton *m_decode;
    QPushButton *m_code;
    QSpinBox *m_spinBoxP;
    QSpinBox *m_spinBoxQ;
    QLabel *m_openTextLabel;

    std::pair<long, long> openKey;
    std::pair<long, long> closedKey;
};

template <typename T>
T modpow(T base, T exp, T modulus) {
  base %= modulus;
  T result = 1;
  while (exp > 0) {
    if (exp & 1) result = (result * base) % modulus;
    base = (base * base) % modulus;
    exp >>= 1;
  }
  return result;
}

#endif // RSACIPHER_H
