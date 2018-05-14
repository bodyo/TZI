#ifndef OPENTEXT_H
#define OPENTEXT_H

#include <QWidget>

#include "frequencywordcounter.h"

class QTextEdit;
class QPushButton;
class QTableView;
class QVBoxLayout;
class QGroupBox;
class QLineEdit;
class QTextDocument;

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
    void searchAndHighlight(QTextDocument *document);

private slots:
    virtual void showBigram();
    virtual void showThreegram();
    virtual void showSortedByAlphabet();
    virtual void showSortedByAscending();

    virtual void search();

protected:
    FrequencyWordCounter m_wordsCounter;
    bool isFirstTime{true};

    QVBoxLayout *m_textFieldLayout;
    QVBoxLayout *m_buttonsLayout;
    QVBoxLayout *m_alphabetLayout;
    QTableView *m_tableView;
    QTextEdit *m_openText;
    QPushButton *m_bigram;
    QPushButton *m_threegram;
    QPushButton *m_frequencyByAlphabet;
    QPushButton *m_frequencyByAscending;
    QLineEdit *m_searchField;
    QPushButton *m_search;
};

#endif // OPENTEXT_H
