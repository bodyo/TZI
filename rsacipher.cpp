#include "rsacipher.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QTextEdit>
#include <QTableView>
#include <QLabel>
#include <QDebug>

#include "opentextv2.h"
#include "alphabetmodel.h"

RSACipher::RSACipher(QWidget *parent)
    : QWidget(parent),
    m_openText(new OpenAndCodedText({"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
                             "N", "O", "P", "Q", "R", "S", "T", "U",
                             "V", "W", "X", "Y", "Z", " ", ".", ",", ";", "-", "'"}, this)),
    m_decode(new QPushButton("Decode", m_openText)),
    m_code(new QPushButton("Code", m_openText)),
    m_spinBoxP(new QSpinBox(this)),
    m_spinBoxQ(new QSpinBox(this)),
    m_openTextLabel(new QLabel(this))
{
    connect(m_decode, &QPushButton::pressed, this, &RSACipher::decode);
    connect(m_code, &QPushButton::pressed, this, &RSACipher::code);
    m_openTextLabel->setMaximumHeight(30);

    auto decodeLayout = new QHBoxLayout();
    decodeLayout->addWidget(m_decode);
    decodeLayout->addWidget(m_code);

//    m_spinBoxP->setValue(29);
//    m_spinBoxQ->setValue(71);

    auto inputLayout = new QHBoxLayout();
    inputLayout->addWidget(m_spinBoxP);
    inputLayout->addWidget(m_spinBoxQ);
    m_openText->buttonsLayout()->addLayout(inputLayout, 4);
    m_openText->buttonsLayout()->addWidget(m_openTextLabel);
    m_openText->buttonsLayout()->addLayout(decodeLayout, 4);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_openText);
}

void RSACipher::code()
{
    auto p = m_spinBoxP->value();
    auto q = m_spinBoxQ->value();

    auto N = p * q;

    auto eiler = (p-1)*(q-1);

    int exp = 0;
    for (int i = 2; i < eiler; ++i)
        if (gcd(i, eiler) == 1)
        {
            exp = i;
            break;
        }
    auto D = calculateD(exp, eiler);
    auto openText = m_openText->openText()->toPlainText();
    auto tableData = static_cast<AlphabetModel *>(m_openText->tableView()->model())->data();

    openKey.first = exp;
    openKey.second = N;

    closedKey.first = D;
    closedKey.second = N;

    m_openTextLabel->setText(QString("Open exponent = %1 : Product = %2\n"
                                     "Secret exponent = %3 : Product = %2").arg(openKey.first).arg(openKey.second).arg(closedKey.first));

    QString codedText;
    for (auto &val : openText)
    {
        codedText.append(QString::number(encrypt(tableData.indexOf(val))));
        codedText.append(" ");
    }
    codedText.remove(codedText.size()-1, 1);

    m_openText->codedText()->setText(codedText);
}

void RSACipher::decode()
{
    auto tableData = static_cast<AlphabetModel *>(m_openText->tableView()->model())->data();

    QString openText;
    for (auto ch : m_openText->codedText()->toPlainText().split(" "))
    {
        auto i = ch.toLong();
        openText.append(tableData.at(crypt(i)));
    }

    m_openText->openText()->setText(openText);
}

int RSACipher::phi(int n)
{
    int result = n;
    for (int i=2; i*i<=n; ++i)
        if (n % i == 0) {
            while (n % i == 0)
                n /= i;
            result -= result / i;
        }
    if (n > 1)
        result -= result / n;
    return result;
}

int RSACipher::xGCD(int a, int b, int &x, int &y)
{
    if(b == 0) {
       x = 1;
       y = 0;
       return a;
    }

    int x1, y1, gcd = xGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}

int RSACipher::gcd(int a, int b)
{
    int t;
    while (b != 0) {
      t = b;
      b = a % b;
      a = t;
    }
    return a;
}

long RSACipher::calculateD(long e, long t)
{
    long int d;
    long int k = 1;

    while ( 1 )
    {
        k = k + t;

        if ( k % e == 0)
        {
            d = (k / e);
            return d;
        }
    }

}

long RSACipher::encrypt(int i)
{
    return std::fmod(std::pow(i, openKey.first), openKey.second);
}

long RSACipher::crypt(long i)
{
    return modpow(i, closedKey.first, closedKey.second);
}
