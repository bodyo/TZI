#ifndef OPENTEXT_H
#define OPENTEXT_H

#include <QWidget>

#include "frequencywordcounter.h"

class QTextEdit;
class QPushButton;
class QTableView;
class QVBoxLayout;
class QGroupBox;

namespace QtCharts {
    class QBarSeries;
}

class OpenText : public QWidget
{
    Q_OBJECT
public:
    explicit OpenText(const QStringList &alphabet, QWidget *parent = nullptr);

    QVBoxLayout *textFieldLayout() const;
    QVBoxLayout *buttonsLayout() const;

    void addTextLieldToLayout();

    QTableView *tableView() const;
    QTextEdit *openText() const;

    FrequencyWordCounter& wordsCounter();
    void addWidgetToAlphabetLayout(QWidget *widget);

private:
    void setupUi(const QStringList &alphabet);

protected:
    void showXRam(QtCharts::QBarSeries *series, QStringList &categories);

private slots:
    virtual void showBigram();
    virtual void showThreegram();
    virtual void showSortedByAlphabet();
    virtual void showSortedByAscending();

protected:
    FrequencyWordCounter m_wordsCounter;

    QVBoxLayout *m_textFieldLayout;
    QVBoxLayout *m_buttonsLayout;
    QVBoxLayout *m_alphabetLayout;
    QTableView *m_tableView;
    QTextEdit *m_openText;
    QPushButton *m_bigram;
    QPushButton *m_threegram;
    QPushButton *m_frequencyByAlphabet;
    QPushButton *m_frequencyByAscending;
};

#endif // OPENTEXT_H
