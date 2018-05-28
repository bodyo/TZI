#include "opentextv2.h"

#include <QTextEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QTableView>

#include <QDebug>

#include "alphabetmodel.h"

using namespace QtCharts;

OpenAndCodedText::OpenAndCodedText(const QStringList &alphabet, QWidget *parent)
    : OpenText(alphabet, parent),
      m_codedText(new QTextEdit(this)),
      m_codedGroupBox(new QGroupBox("Coded text", this)),
      m_openTextGroupBox(new QGroupBox("Open text", this)),
      m_linearChart(new QPushButton("Line Sorted By Ascending", this))
{
    auto codedLayout = new QVBoxLayout();
    codedLayout->addWidget(m_codedText);
    m_codedGroupBox->setLayout(codedLayout);

    auto openTextLayout = new QVBoxLayout();
    openTextLayout->addWidget(openText());
    m_openTextGroupBox->setLayout(openTextLayout);

    textFieldLayout()->addWidget(m_codedGroupBox);
    textFieldLayout()->addWidget(m_openTextGroupBox);
    connect(m_linearChart, &QPushButton::clicked, this, &OpenAndCodedText::showLineSortedByAscending);
    buttonsLayout()->addWidget(m_linearChart);
}

void OpenAndCodedText::showBigram()
{
    auto &bigramMap1 = wordsCounter().bigramCount();
    auto &bigramMap2 = m_wordsCounter1.bigramCount();

    auto tableData = static_cast<AlphabetModel *>(tableView()->model())->data();
    wordsCounter().count(openText()->toPlainText(), tableData);
    m_wordsCounter1.count(m_codedText->toPlainText(), tableData);

    QBarSet *set = new QBarSet("Open Text");
    if (bigramMap1.isEmpty())
        return;

    QVector<QPair<QString, std::size_t>> sortedBigram1;
    QMapIterator<QString, size_t> iter1(bigramMap1);

    while(iter1.hasNext())
    {
        iter1.next();
        sortedBigram1.append(qMakePair(iter1.key(), iter1.value()));
    }

    std::sort(sortedBigram1.begin(), sortedBigram1.end(),
              [](const QPair<QString, std::size_t> &first, const QPair<QString, std::size_t> &second){
        return first.second > second.second;
    });

    QStringList categories1;
    for (int i = 0; i < 15 && i <= sortedBigram1.size()-1; i++)
    {
        set->append(sortedBigram1.at(i).second);
        categories1.append(QString("\"%1\"").arg(sortedBigram1.at(i).first));
    }

    QBarSet *set1 = new QBarSet("Coded Text");
    if (bigramMap1.isEmpty())
        return;

    QVector<QPair<QString, std::size_t>> sortedBigram2;
    QMapIterator<QString, size_t> iter(bigramMap2);

    while(iter.hasNext())
    {
        iter.next();
        sortedBigram2.append(qMakePair(iter.key(), iter.value()));
    }

    std::sort(sortedBigram2.begin(), sortedBigram2.end(),
              [](const QPair<QString, std::size_t> &first, const QPair<QString, std::size_t> &second){
        return first.second > second.second;
    });

    QStringList categories;
    for (int i = 0; i < 15 && i <= sortedBigram2.size()-1; i++)
    {
        set1->append(sortedBigram2.at(i).second);
        categories.append(QString("\"%1\"").arg(sortedBigram2.at(i).first));
    }

    QStringList finalCategories;
    for (int i = 0; i < categories.size() && i < categories1.size(); ++i)
        finalCategories.append(QString("%1/%2").arg(categories1.at(i)).arg(categories.at(i)));

    QBarSeries *series = new QBarSeries();
    series->append(set);
    series->append(set1);

    showXRam(series, finalCategories);
}

void OpenAndCodedText::showThreegram()
{
    auto &bigramMap1 = wordsCounter().threegramCount();
    auto &bigramMap2 = m_wordsCounter1.threegramCount();

    auto tableData = static_cast<AlphabetModel *>(tableView()->model())->data();
    wordsCounter().count(openText()->toPlainText(), tableData);
    m_wordsCounter1.count(m_codedText->toPlainText(), tableData);

    QBarSet *set = new QBarSet("Open Text");
    if (bigramMap1.isEmpty())
        return;

    QVector<QPair<QString, std::size_t>> sortedBigram1;
    QMapIterator<QString, size_t> iter1(bigramMap1);

    while(iter1.hasNext())
    {
        iter1.next();
        sortedBigram1.append(qMakePair(iter1.key(), iter1.value()));
    }

    std::sort(sortedBigram1.begin(), sortedBigram1.end(),
              [](const QPair<QString, std::size_t> &first, const QPair<QString, std::size_t> &second){
        return first.second > second.second;
    });

    QStringList categories1;
    for (int i = 0; i < 15 && i <= sortedBigram1.size()-1; i++)
    {
        set->append(sortedBigram1.at(i).second);
        categories1.append(QString("\"%1\"").arg(sortedBigram1.at(i).first));
    }

    QBarSet *set1 = new QBarSet("Coded Text");
    if (bigramMap1.isEmpty())
        return;

    QVector<QPair<QString, std::size_t>> sortedBigram2;
    QMapIterator<QString, size_t> iter(bigramMap2);

    while(iter.hasNext())
    {
        iter.next();
        sortedBigram2.append(qMakePair(iter.key(), iter.value()));
    }

    std::sort(sortedBigram2.begin(), sortedBigram2.end(),
              [](const QPair<QString, std::size_t> &first, const QPair<QString, std::size_t> &second){
        return first.second > second.second;
    });

    QStringList categories;
    for (int i = 0; i < 15 && i <= sortedBigram2.size()-1; i++)
    {
        set1->append(sortedBigram2.at(i).second);
        categories.append(QString("\"%1\"").arg(sortedBigram2.at(i).first));
    }

    QStringList finalCategories;
    for (int i = 0; i < categories.size() && i < categories1.size(); ++i)
        finalCategories.append(QString("%1/%2").arg(categories1.at(i)).arg(categories.at(i)));

    QBarSeries *series = new QBarSeries();
    series->append(set);
    series->append(set1);

    showXRam(series, finalCategories);
}

void OpenAndCodedText::showSortedByAlphabet()
{
    auto &charactersMap = wordsCounter().charactersCount();
    auto &charactersMap1 = m_wordsCounter1.charactersCount();

    auto tableData = static_cast<AlphabetModel *>(m_tableView->model())->data();
    m_wordsCounter1.count(m_codedText->toPlainText(), tableData);
    wordsCounter().count(openText()->toPlainText(), tableData);

    QBarSet *set = new QBarSet("Open text");
    if (charactersMap.isEmpty())
        return;

    auto charatersInText = charactersMap.keys();

    std::sort(charatersInText.begin(), charatersInText.end(), [&tableData](const QChar &ch1, const QChar &ch2){
        return tableData.indexOf(ch1) < tableData.indexOf(ch2);
    });

    QStringList categories;
    for (auto character : charatersInText)
    {
        set->append(charactersMap.value(character));
        categories.append(QString("\"%1\"").arg(character));
    }

    QBarSet *set1 = new QBarSet("Coded text");
    if (charactersMap.isEmpty())
        return;

    auto charatersInText1 = charactersMap1.keys();

    std::sort(charatersInText1.begin(), charatersInText1.end(), [&tableData](const QChar &ch1, const QChar &ch2){
        return tableData.indexOf(ch1) < tableData.indexOf(ch2);
    });

    QStringList categories1;
    for (auto character : charatersInText1)
    {
        set1->append(charactersMap1.value(character));
        categories1.append(QString("\"%1\"").arg(character));
    }

    QStringList finalCategories;
    for (int i = 0; i < categories.size() && i < categories1.size(); ++i)
        finalCategories.append(QString("%1/%2").arg(categories.at(i)).arg(categories1.at(i)));

    QBarSeries *series = new QBarSeries();
    series->append(set);
    series->append(set1);

    showXRam(series, finalCategories);
}

void OpenAndCodedText::showSortedByAscending()
{
    auto &charactersMap = wordsCounter().charactersCount();
    auto &charactersMap1 = m_wordsCounter1.charactersCount();

    auto tableData = static_cast<AlphabetModel *>(m_tableView->model())->data();
    m_wordsCounter1.count(m_codedText->toPlainText(), tableData);
    wordsCounter().count(openText()->toPlainText(), tableData);

    QBarSet *set = new QBarSet("Open text");
    if (charactersMap.isEmpty())
        return;

    auto charatersInText = charactersMap.keys();

    std::sort(charatersInText.begin(), charatersInText.end(), [&charactersMap](const QChar &ch1, const QChar &ch2){
            return charactersMap.value(ch1) > charactersMap.value(ch2);
        });

    QStringList categories;
    for (auto character : charatersInText)
    {
        set->append(charactersMap.value(character));
        categories.append(QString("\"%1\"").arg(character));
    }

    QBarSet *set1 = new QBarSet("Coded text");
    if (charactersMap1.isEmpty())
        return;

    auto charatersInText1 = charactersMap1.keys();

    std::sort(charatersInText1.begin(), charatersInText1.end(), [&charactersMap1](const QChar &ch1, const QChar &ch2){
            return charactersMap1.value(ch1) > charactersMap1.value(ch2);
        });

    QStringList categories1;
    for (auto character : charatersInText1)
    {
        set1->append(charactersMap1.value(character));
        categories1.append(QString("\"%1\"").arg(character));
    }

    QStringList finalCategories;
    for (int i = 0; i < categories.size() && i < categories1.size(); ++i)
        finalCategories.append(QString("%1/%2").arg(categories.at(i)).arg(categories1.at(i)));

    QBarSeries *series = new QBarSeries();
    series->append(set);
    series->append(set1);

    showXRam(series, finalCategories);
}

void OpenAndCodedText::showLineSortedByAscending()
{
    auto &charactersMap = wordsCounter().charactersCount();
    auto &charactersMap1 = m_wordsCounter1.charactersCount();

    auto tableData = static_cast<AlphabetModel *>(m_tableView->model())->data();
    m_wordsCounter1.count(m_codedText->toPlainText(), tableData);
    wordsCounter().count(openText()->toPlainText(), tableData);

    QVector<int> interval;
    {
        int interv = 0;
        for (int i = 0; i < tableData.size(); ++i)
        {
            interval.append(interv);
            interv+=2;
        }
    }
    QLineSeries *set = new QLineSeries();
    set->setName("Open text");
    if (charactersMap.isEmpty())
        return;

    auto charatersInText = charactersMap.keys();

    std::sort(charatersInText.begin(), charatersInText.end(), [&charactersMap](const QChar &ch1, const QChar &ch2){
            return charactersMap.value(ch1) > charactersMap.value(ch2);
        });

    QStringList categories;
    auto intervIterator = interval.cbegin();
    for (auto character : charatersInText)
    {
        set->append(*intervIterator, charactersMap.value(character));
        categories.append(QString("\"%1\"").arg(character));
        if (intervIterator != interval.end())
            ++intervIterator;
    }

    QLineSeries *set1 = new QLineSeries();
    set1->setName("Coded text");
    if (charactersMap1.isEmpty())
        return;

    auto charatersInText1 = charactersMap1.keys();

    std::sort(charatersInText1.begin(), charatersInText1.end(), [&charactersMap1](const QChar &ch1, const QChar &ch2){
            return charactersMap1.value(ch1) > charactersMap1.value(ch2);
        });

    QStringList categories1;
    intervIterator = interval.cbegin();
    for (auto character : charatersInText1)
    {
        set1->append(*intervIterator, charactersMap1.value(character));
        categories1.append(QString("\"%1\"").arg(character));
        if (intervIterator != interval.end())
            ++intervIterator;
    }

    QCategoryAxis *axisX = new QCategoryAxis();
    intervIterator = interval.cbegin();
    for (int i = 0; i < categories.size() && i < categories1.size(); ++i)
    {
        axisX->append(QString("%1/%2").arg(categories.at(i)).arg(categories1.at(i)), *intervIterator);
        if (intervIterator != interval.end())
            ++intervIterator;
    }

    QChart *chart = new QChart();
    chart->addSeries(set);
    chart->addSeries(set1);
    chart->setTitle("Data");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
//    axisY->setTitleText("");
    chart->setAxisY(axisY, set);
    chart->setAxisY(axisY, set1);
    chart->setAxisX(axisX, set);
    chart->setAxisX(axisX, set1);
//    chart->setAxisX(axisX, set1);
//    chart->setTheme(QChart::ChartThemeDark);

//    QBarCategoryAxis *axis = new QBarCategoryAxis();
//    axis->append(categories);
//    chart->createDefaultAxes();
//    chart->setAxisX(axis, series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->resize({1800, 600});
    chartView->setAttribute(Qt::WA_DeleteOnClose);
    chartView->show();

    connect(chartView, &QObject::destroyed, [chart](){
        delete chart;
    });
}

void OpenAndCodedText::search()
{
    QTextDocument *document = m_codedText->document();

    if (!isFirstTime)
        document->undo();
    searchAndHighlight(document);

    QTextDocument *document1 = m_openText->document();

    if (!isFirstTime)
        document1->undo();
    searchAndHighlight(document1);
    isFirstTime = false;
}

QTextEdit *OpenAndCodedText::codedText() const
{
    return m_codedText;
}
