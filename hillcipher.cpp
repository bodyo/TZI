#include "hillcipher.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableView>
#include <QGenericMatrix>
#include <QMatrix3x3>
#include <vector>

#include "opentextv2.h"
#include "alphabetmodel.h"

HillCipher::HillCipher(QWidget *parent)
    : QWidget(parent),
      m_openText(new OpenAndCodedText({"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
                               "N", "O", "P", "Q", "R", "S", "T", "U",
                               "V", "W", "X", "Y", "Z", " ", ".", ",", ";", "-", "'"}, this)),
      m_decode(new QPushButton("Decode", m_openText)),
      m_code(new QPushButton("Code", m_openText)),
      m_lineEdit(new QLineEdit(this))
{
    connect(m_decode, &QPushButton::pressed, this, &HillCipher::decode);
    connect(m_code, &QPushButton::pressed, this, &HillCipher::code);
    m_lineEdit->setPlaceholderText("Key");

    auto decodeLayout = new QHBoxLayout();
    decodeLayout->addWidget(m_decode);
    decodeLayout->addWidget(m_code);

    m_openText->buttonsLayout()->addWidget(m_lineEdit);
    m_openText->buttonsLayout()->addLayout(decodeLayout);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_openText);
}

void HillCipher::decode()
{
    std::array<float, 9> initMatrix;//{{14, 7, 4, 17, 20, 10, 1, 14, 7}};
    auto key = m_lineEdit->text();
    if (key.isEmpty())
        return;

    auto tableData = static_cast<AlphabetModel *>(m_openText->tableView()->model())->data();
    for (size_t i = 0; i < initMatrix.size(); ++i)
        initMatrix[i] = tableData.indexOf(key.at(i));

    QGenericMatrix<3,3,float> matrix3x3(initMatrix.data());

    qDebug() << matrix3x3;

    QTransform matrix(initMatrix.at(0),initMatrix.at(1),initMatrix.at(2),
                      initMatrix.at(3),initMatrix.at(4),initMatrix.at(5),
                      initMatrix.at(6),initMatrix.at(7),initMatrix.at(8));

    std::vector<QGenericMatrix<3,1,float>> matrixesWithText;

    // fill the text in matrix
    auto codedText = m_openText->codedText()->toPlainText();
    for (auto iter = codedText.begin(); iter != codedText.end(); iter+=3)
    {
        std::array<float, 3> matrix1x3;
        matrix1x3[0] = tableData.indexOf(*iter);
        if (iter+1 != codedText.end())
            matrix1x3[1] = tableData.indexOf(*(iter+1));
        else
        {
            matrix1x3[1] = matrix1x3[2] = tableData.indexOf(" ");
            matrixesWithText.push_back(QGenericMatrix<3,1,float>(matrix1x3.data()));
            break;
        }
        if (iter+2 != codedText.end())
            matrix1x3[2] = tableData.indexOf(*(iter+2));
        else
        {
            matrix1x3[2] = tableData.indexOf(" ");
            matrixesWithText.push_back(QGenericMatrix<3,1,float>(matrix1x3.data()));
            break;
        }

        matrixesWithText.push_back(QGenericMatrix<3,1,float>(matrix1x3.data()));
    }

    int x = 0, y;
    auto determinant = matrix.determinant();
    gcd(determinant, tableData.size(), x, y);

    float reverceDeterminantElement = 0;
    if ((determinant < 0 && x > 0)||(determinant > 0 && x > 0))
        reverceDeterminantElement = x;
    else if (determinant > 0 && x < 0)
        reverceDeterminantElement = tableData.size() + x;
    else if (determinant < 0 && x < 0)
        reverceDeterminantElement = -reverceDeterminantElement;

    auto reverseMatrix = reverseKeyMatrix(matrix3x3);

    auto iter = reverseMatrix.data();
    for (int i = 0; i < 9; ++i)
    {
        *iter = std::fmod(*iter, tableData.size());
        *iter *= reverceDeterminantElement;
        *iter = std::fmod(*iter, tableData.size());
        iter++;
    }

    auto finalMatrix = reverseMatrix.transposed();

    iter = finalMatrix.data();
    for (int i = 0; i < 9; ++i)
    {
        if (*iter < 0)
            *iter = tableData.size() + *iter;
        iter++;
    }

    for (auto &matrix : matrixesWithText)
    {
        matrix = matrix * finalMatrix;
        matrix(0,0) = std::fmod(matrix(0,0), tableData.size());
        matrix(0,1) = std::fmod(matrix(0,1), tableData.size());
        matrix(0,2) = std::fmod(matrix(0,2), tableData.size());
    }

    QString openText;

    for (auto matrix3x1 : matrixesWithText)
    {
        openText.push_back(tableData.at(matrix3x1(0,0)));
        openText.push_back(tableData.at(matrix3x1(0,1)));
        openText.push_back(tableData.at(matrix3x1(0,2)));
    }

    m_openText->openText()->setText(openText);
}

void HillCipher::code()
{
    std::array<float, 9> initMatrix;//{{14, 7, 4, 17, 20, 10, 1, 14, 7}};
    auto key = m_lineEdit->text();
    if (key.isEmpty())
        return;

    auto tableData = static_cast<AlphabetModel *>(m_openText->tableView()->model())->data();
    for (size_t i = 0; i < initMatrix.size(); ++i)
        initMatrix[i] = tableData.indexOf(key.at(i));

    QGenericMatrix<3,3,float> matrix3x3(initMatrix.data());

    std::vector<QGenericMatrix<3,1,float>> matrixesWithText;

    // fill the text in matrix
    auto openText = m_openText->openText()->toPlainText();
    for (auto iter = openText.begin(); iter != openText.end(); iter+=3)
    {
        std::array<float, 3> matrix1x3;
        matrix1x3[0] = tableData.indexOf(*iter);
        if (iter+1 != openText.end())
            matrix1x3[1] = tableData.indexOf(*(iter+1));
        else
        {
            matrix1x3[1] = matrix1x3[2] = tableData.indexOf(" ");
            matrixesWithText.push_back(QGenericMatrix<3,1,float>(matrix1x3.data()));
            break;
        }
        if (iter+2 != openText.end())
            matrix1x3[2] = tableData.indexOf(*(iter+2));
        else
        {
            matrix1x3[2] = tableData.indexOf(" ");
            matrixesWithText.push_back(QGenericMatrix<3,1,float>(matrix1x3.data()));
            break;
        }

        matrixesWithText.push_back(QGenericMatrix<3,1,float>(matrix1x3.data()));
    }

    // code matrix with text by matrixKey
    for (auto &matrix : matrixesWithText)
    {
        matrix = matrix * matrix3x3;
        matrix(0,0) = std::fmod(matrix(0,0), tableData.size());
        matrix(0,1) = std::fmod(matrix(0,1), tableData.size());
        matrix(0,2) = std::fmod(matrix(0,2), tableData.size());
    }

    QString codedText;

    for (auto matrix3x1 : matrixesWithText)
    {
        codedText.push_back(tableData.at(matrix3x1(0,0)));
        codedText.push_back(tableData.at(matrix3x1(0,1)));
        codedText.push_back(tableData.at(matrix3x1(0,2)));
    }

    m_openText->codedText()->setText(codedText);
}

qreal HillCipher::determinant(QGenericMatrix<3, 3, float> &matrix)
{
    auto additional = matrix(0,0) * matrix(1,1) * matrix(2,2);
    additional += matrix(0,1) * matrix(1,2) * matrix(2,0);
    additional += matrix(1,0) * matrix(2,1) * matrix(0,2);
    additional -= matrix(2,0) * matrix(1,1) * matrix(0,2);
    additional -= matrix(2,1) * matrix(1,2) * matrix(0,0);
    additional -= matrix(0,1) * matrix(1,0) * matrix(2,2);
    return additional;
}

int HillCipher::gcd(int a, int b, int &x, int &y)
{
    if (a == 0) {
        x = 0; y = 1;
        return b;
    }
    int x1, y1;
    int d = gcd (b%a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

QGenericMatrix<3, 3, float> HillCipher::reverseKeyMatrix(QGenericMatrix<3, 3, float> &matrix)
{
    QGenericMatrix<3,3,float> reverse;
    QMatrix a00(matrix(1,1), matrix(1,2), matrix(2,1), matrix(2,2), 0, 0);
    reverse(0,0) = a00.determinant() * std::pow(-1, 2);

    QMatrix a01(matrix(1,0), matrix(2,0), matrix(1,2), matrix(2,2), 0, 0);
    reverse(0,1) = a01.determinant() * std::pow(-1, 3);

    QMatrix a02(matrix(1,0), matrix(2,0), matrix(1,1), matrix(2,1), 0, 0);
    reverse(0,2) = a02.determinant() * std::pow(-1, 4);

    QMatrix a10(matrix(0,1), matrix(0,2), matrix(2,1), matrix(2,2), 0, 0);
    reverse(1,0) = a10.determinant() * std::pow(-1, 3);

    QMatrix a11(matrix(0,0), matrix(0,2), matrix(2,0), matrix(2,2), 0, 0);
    reverse(1,1) = a11.determinant() * std::pow(-1, 4);

    QMatrix a12(matrix(0,0), matrix(0,1), matrix(2,0), matrix(2,1), 0, 0);
    reverse(1,2) = a12.determinant() * std::pow(-1, 5);

    QMatrix a20(matrix(0,1), matrix(0,2), matrix(1,1), matrix(1,2), 0, 0);
    reverse(2,0) = a20.determinant() * std::pow(-1, 4);

    QMatrix a21(matrix(0,0), matrix(0,2), matrix(1,0), matrix(1,2), 0, 0);
    reverse(2,1) = a21.determinant() * std::pow(-1, 5);

    QMatrix a22(matrix(0,0), matrix(0,1), matrix(1,0), matrix(1,1), 0, 0);
    reverse(2,2) = a22.determinant() * std::pow(-1, 6);

    return reverse;
}
