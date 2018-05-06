#include "caesarcipher.h"

#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QTableView>

#include <QDebug>

#include "alphabetmodel.h"
#include "opentextv2.h"

CaesarCipher::CaesarCipher(QWidget *parent)
    : QWidget(parent),
      m_openText(new OpenAndCodedText({"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
                               "N", "O", "P", "Q", "R", "S", "T", "U",
                               "V", "W", "X", "Y", "Z", " ", ".", ",", ";", "-", "'"}, this)),
      m_decode(new QPushButton("Decode", m_openText)),
      m_spinBox(new QSpinBox(this))
{
    setupUi();

    connect(m_decode, &QPushButton::pressed, this, &CaesarCipher::decode);
}

void CaesarCipher::setupUi()
{
    auto mainLayout = new QVBoxLayout(this);

    auto decodeLayout = new QHBoxLayout();
    decodeLayout->addWidget(m_decode);
    decodeLayout->addWidget(m_spinBox);

    m_openText->buttonsLayout()->addLayout(decodeLayout);
    mainLayout->addWidget(m_openText);
}

void CaesarCipher::decode()
{
    auto tableData = static_cast<AlphabetModel *>(m_openText->tableView()->model())->data();

    auto codedText = m_openText->codedText()->toPlainText();
    QString decodedText;

    auto key = m_spinBox->value();
    auto n = tableData.size(); //bodyo = 10
    for (auto iter : codedText)
    {
        auto openText = tableData.indexOf(iter);
        auto y = (openText + key) % n;

        decodedText.append(tableData.at(y));
    }
    m_openText->openText()->setText(decodedText);
}
