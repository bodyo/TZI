#include "vigenerecipher.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableView>

#include "opentextv2.h"
#include "alphabetmodel.h"

VigenereCipher::VigenereCipher(QWidget *parent)
    : QWidget(parent),
      m_openText(new OpenAndCodedText({"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
                               "N", "O", "P", "Q", "R", "S", "T", "U",
                               "V", "W", "X", "Y", "Z", " ", ".", ",", ";", "-", "'"}, this)),
      m_decode(new QPushButton("Decode", m_openText)),
      m_code(new QPushButton("Code", m_openText)),
      m_lineEdit(new QLineEdit(this))
{
    connect(m_decode, &QPushButton::pressed, this, &VigenereCipher::decode);
    connect(m_code, &QPushButton::pressed, this, &VigenereCipher::code);
    m_lineEdit->setPlaceholderText("Key");

    auto decodeLayout = new QHBoxLayout();
    decodeLayout->addWidget(m_decode);
    decodeLayout->addWidget(m_code);

    m_openText->buttonsLayout()->addWidget(m_lineEdit);
    m_openText->buttonsLayout()->addLayout(decodeLayout);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_openText);
}

void VigenereCipher::decode()
{
    auto textToCode = m_openText->codedText()->toPlainText().toUpper();
    auto keyWord = m_lineEdit->text().toUpper();

    auto tableData = static_cast<AlphabetModel *>(m_openText->tableView()->model())->data();
    auto n = tableData.size();

    int keywordIndex = 0;

    QString text;
    for (auto iter : textToCode)
    {
        auto openText = tableData.indexOf(iter);
        auto key = tableData.indexOf(keyWord.at(keywordIndex));
        auto y = (openText + n - key) % n;
        keywordIndex++;

        text.append(tableData.at(y));

        if ((keywordIndex + 1) == keyWord.length())
            keywordIndex = 0;
    }

    m_openText->openText()->setText(text);
}

void VigenereCipher::code()
{
    auto textToCode = m_openText->openText()->toPlainText().toUpper();
    auto keyWord = m_lineEdit->text().toUpper();

    auto tableData = static_cast<AlphabetModel *>(m_openText->tableView()->model())->data();
    auto n = tableData.size();

    int keywordIndex = 0;

    QString codedText;
    for (auto iter : textToCode)
    {
        auto openText = tableData.indexOf(iter);
        auto key = tableData.indexOf(keyWord.at(keywordIndex));
        auto y = (openText + key) % n;
        keywordIndex++;

        codedText.append(tableData.at(y));

        if ((keywordIndex + 1) == keyWord.length())
            keywordIndex = 0;
    }

    m_openText->codedText()->setText(codedText);
}
