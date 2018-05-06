#include "decoder.h"

#include <QTabWidget>
#include <QHBoxLayout>

#include "caesarcipher.h"
#include "opentext.h"
#include "simplereplacementcipher.h"
#include "vigenerecipher.h"

Decoder::Decoder(QWidget *parent)
    : QWidget(parent),
      m_tabWidget(new QTabWidget(this))
{

    auto openTextWidget = new OpenText({"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
                                        "N", "O", "P", "Q", "R", "S", "T", "U",
                                        "V", "W", "X", "Y", "Z", " ", ".", ",", ";", "-", "'"}, this);
    openTextWidget->addTextLieldToLayout();
    m_tabWidget->addTab(openTextWidget, "Open Text");

    m_tabWidget->addTab(new CaesarCipher(this), "Caesar Cipher");
    m_tabWidget->addTab(new SimpleReplacementCipher(this), "Substitution cipher");
    m_tabWidget->addTab(new VigenereCipher(this), "Vigenere cipher");

    auto layout = new QHBoxLayout(this);
    layout->addWidget(m_tabWidget);
    setLayout(layout);
}

Decoder::~Decoder()
{

}
