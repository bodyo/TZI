#ifndef OPENTEXTV2_H
#define OPENTEXTV2_H

#include "opentext.h"

class OpenAndCodedText : public OpenText
{
public:
    explicit OpenAndCodedText(const QStringList &alphabet, QWidget *parent = nullptr);

    QTextEdit *codedText() const;

private slots:
    void showBigram() override;
    void showThreegram() override;
    void showSortedByAlphabet() override;
    void showSortedByAscending() override;
    void showLineSortedByAscending();

    void search() override;

private:
    FrequencyWordCounter m_wordsCounter1;

    QTextEdit *m_codedText;
    QGroupBox *m_codedGroupBox;
    QGroupBox *m_openTextGroupBox;
    QPushButton *m_linearChart;
};

#endif // OPENTEXTV2_H
