#include "feistelcipher.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableView>
#include <QSpinBox>
#include <functional>
#include <QDebug>

#include "opentextv2.h"
#include "alphabetmodel.h"

FeistelCipher::FeistelCipher(QWidget *parent)
    : QWidget(parent),
      m_openText(new OpenAndCodedText({"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
                               "N", "O", "P", "Q", "R", "S", "T", "U",
                               "V", "W", "X", "Y", "Z", " ", ".", ",", ";", "-", "'"}, this)),
      m_decode(new QPushButton("Decode", m_openText)),
      m_code(new QPushButton("Code", m_openText)),
      m_lineEdit(new QLineEdit(this)),
      m_spinBox(new QSpinBox(this))
{
    connect(m_decode, &QPushButton::pressed, this, &FeistelCipher::decode);
    connect(m_code, &QPushButton::pressed, this, &FeistelCipher::code);
    m_lineEdit->setPlaceholderText("Key");

    auto decodeLayout = new QHBoxLayout();
    decodeLayout->addWidget(m_decode);
    decodeLayout->addWidget(m_code);

    m_spinBox->setValue(10);
    m_lineEdit->setText("7+");

    auto inputLayout = new QHBoxLayout();
    inputLayout->addWidget(m_lineEdit);
    inputLayout->addWidget(m_spinBox);
    m_openText->buttonsLayout()->addLayout(inputLayout);
    m_openText->buttonsLayout()->addLayout(decodeLayout);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_openText);
}

int FeistelCipher::f(int L, int k, int size)
{
    return (L+k)%size;
}

void FeistelCipher::code()
{
    auto tableData = static_cast<AlphabetModel *>(m_openText->tableView()->model())->data();
    auto rounds = m_spinBox->value();

    auto openText = m_openText->openText()->toPlainText().toUpper();

    auto inputK = m_lineEdit->text();
    auto isIncrease = inputK.back() == '+';
    inputK.remove(inputK.size()-1, 1);
    auto K = inputK.toInt();

    std::function<void(int&)> kFunc = [](int &k){
        ++k;
    };
    if (!isIncrease)
    {
        kFunc = [](int &k){
            --k;
        };
    }

    QString codedText;
    for (auto iter = openText.begin(); iter != openText.end(); iter+=2)
    {
        int L;
        int R;
        K = inputK.toInt();
        bool needToQuit = false;

        if (iter == openText.end())
        {
            L = tableData.indexOf(" ");
            R = tableData.indexOf(" ");
            needToQuit = true;
        }
        else if (iter+1 == openText.end())
        {
            L = tableData.indexOf(*iter);
            R = tableData.indexOf(" ");
            needToQuit = true;
        }
        else
        {
            L = tableData.indexOf(*iter);
            R = tableData.indexOf(*(iter +1));
        }

        int func = 0, temp = 0;
        for (int i = 0; i < rounds; ++i)
        {
            func = f(L, K, tableData.size());
            temp = L;
            L = R ^ func;
            R = temp;
            kFunc(K);
        }
        codedText.push_back(tableData.at(L));
        codedText.push_back(tableData.at(R));
        if (needToQuit)
            break;
    }

    m_openText->codedText()->setText(codedText);
}

void FeistelCipher::decode()
{
    auto tableData = static_cast<AlphabetModel *>(m_openText->tableView()->model())->data();
    auto rounds = m_spinBox->value();

    auto codedText = m_openText->codedText()->toPlainText().toUpper();

    auto inputK = m_lineEdit->text();
    auto isIncrease = inputK.back() == '+';
    inputK.remove(inputK.size()-1, 1);
    auto K = inputK.toInt() + rounds - 1;

    std::function<void(int&)> kFunc = [](int &k){
        --k;
    };
    if (!isIncrease)
    {
        kFunc = [](int &k){
            ++k;
        };
    }

    QString openText;
    for (auto iter = codedText.begin(); iter != codedText.end(); iter+=2)
    {
        int L;
        int R;
        K = inputK.toInt() + rounds - 1;
        bool needToQuit = false;

        if (iter == codedText.end())
        {
            L = tableData.indexOf(" ");
            R = tableData.indexOf(" ");
            needToQuit = true;
        }
        else if (iter+1 == codedText.end())
        {
            L = tableData.indexOf(*iter);
            R = tableData.indexOf(" ");
            needToQuit = true;
        }
        else
        {
            L = tableData.indexOf(*iter);
            R = tableData.indexOf(*(iter +1));
        }

        std::swap(R,L);
        int func = 0, temp = 0;
        for (int i = 0; i < rounds; ++i)
        {
            func = f(L, K, tableData.size());
            temp = L;
            L = R ^ func;
            R = temp;
            kFunc(K);
        }
        openText.push_back(tableData.at(R));
        openText.push_back(tableData.at(L));
        if (needToQuit)
            break;
    }

//    qDebug() << keyVec
    m_openText->openText()->setText(openText);
}
