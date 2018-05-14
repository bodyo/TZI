#include "simplereplacementcipher.h"

#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QTableView>
#include <QHeaderView>

#include <QDebug>

#include "alphabetmodel.h"
#include "opentextv2.h"
#include "relationtableview.h"

SimpleReplacementCipher::SimpleReplacementCipher(QWidget *parent)
    : QWidget(parent),
      m_relationTableModel(new RelationTableModel({{"A","D"}, {"B"," "}, {"C","E"}, {"D","M"}, {"E","G"}, {"F","O"}, {"G","K"},
                                                   {"H","H"}, {"I","L"}, {"J","I"}, {"K","F"}, {"L","P"}, {"M","H"},
                                                   {"N","Q"}, {"O","O"}, {"P","U"}, {"Q","W"}, {"R","Y"}, {"S","X"}, {"T"," "}, {"U","S"},
                                                   {"V","-"}, {"W","V"}, {"X","X"}, {"Y","N"}, {"Z","A"}, {" ","B"}, {".","C"}, {",","T"}, {";","S"}, {"-","R"}, {"'","'"}})),
      m_relationsTableView(new QTableView(this)),
      m_openText(new OpenAndCodedText({"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
                               "N", "O", "P", "Q", "R", "S", "T", "U",
                               "V", "W", "X", "Y", "Z", " ", ".", ",", ";", "-", "'"}, this)),
      m_decode(new QPushButton("Decode", m_openText))
{
    connect(m_decode, &QPushButton::pressed, this, &SimpleReplacementCipher::decode);

    m_relationsTableView->setModel(m_relationTableModel);

    m_relationsTableView->horizontalHeader()->setStretchLastSection(true);
    m_relationsTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    auto decodeLayout = new QHBoxLayout();
    decodeLayout->addWidget(m_decode);

    m_openText->buttonsLayout()->addLayout(decodeLayout);
    m_openText->addWidgetToAlphabetLayout(m_relationsTableView);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_openText);
}

void SimpleReplacementCipher::decode()
{
    QMultiMap<QString, QString> conformityMap;
    for (auto &pair : m_relationTableModel->getData())
        conformityMap.insert(pair.first, pair.second);

    auto codedText = m_openText->codedText()->toPlainText();
    QString decodetText;
    for (auto &character : codedText)
        decodetText.append(conformityMap.value(character, character));

    m_openText->openText()->setText(decodetText);
}
