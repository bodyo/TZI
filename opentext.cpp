#include "opentext.h"

#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QChartView>
#include <QMapIterator>
#include <QTableView>
#include <QHeaderView>
#include <QChar>
#include <QGroupBox>
#include <QLineEdit>

#include <QDebug>

#include <algorithm>

#include "alphabetmodel.h"

using namespace QtCharts;

OpenText::OpenText(const QStringList &alphabet, QWidget *parent)
    : QWidget(parent),
      m_textFieldLayout(new QVBoxLayout()),
      m_buttonsLayout(new QVBoxLayout()),
      m_alphabetLayout(new QVBoxLayout()),
      m_tableView(new QTableView()),
      m_openText(new QTextEdit(this)),
      m_bigram(new QPushButton("Bigram", this)),
      m_threegram(new QPushButton("Threegram", this)),
      m_frequencyByAlphabet(new QPushButton("Frequency of repetitions by alphabet", this)),
      m_frequencyByAscending(new QPushButton("Frequency of repetitions by ascending", this)),
      m_searchField(new QLineEdit(this)),
      m_search(new QPushButton("Search", this))

{
    setupUi(alphabet);
}

void OpenText::setupUi(const QStringList &alphabet)
{
    auto layout = new QHBoxLayout(this);

    m_openText->setUndoRedoEnabled(true);

    m_tableView->setModel(new AlphabetModel(alphabet, m_tableView));

    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    m_searchField->setPlaceholderText("Enter something for search");

    auto searchLayout = new QVBoxLayout();
    searchLayout->addWidget(m_searchField);
    searchLayout->addWidget(m_search);

    m_buttonsLayout->addLayout(searchLayout);
    m_buttonsLayout->addWidget(m_bigram);
    m_buttonsLayout->addWidget(m_threegram);
    m_buttonsLayout->addWidget(m_frequencyByAlphabet);
    m_buttonsLayout->addWidget(m_frequencyByAscending);

    addWidgetToAlphabetLayout(m_tableView);

    layout->addLayout(m_textFieldLayout, 4);
    layout->addLayout(m_alphabetLayout, 1);
    layout->addLayout(m_buttonsLayout, 0);

    connect(m_bigram, &QPushButton::clicked, this, &OpenText::showBigram);
    connect(m_threegram, &QPushButton::clicked, this, &OpenText::showThreegram);
    connect(m_frequencyByAlphabet, &QPushButton::clicked, this, &OpenText::showSortedByAlphabet);
    connect(m_frequencyByAscending, &QPushButton::clicked, this, &OpenText::showSortedByAscending);
    connect(m_search, &QPushButton::clicked, this, &OpenText::search);
}

void OpenText::addWidgetToAlphabetLayout(QWidget *widget)
{
    m_alphabetLayout->addWidget(widget);
}

void OpenText::showXRam(QBarSeries *series, QStringList &categories)
{
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Data");
    chart->setAnimationOptions(QChart::SeriesAnimations);
//    chart->setTheme(QChart::ChartThemeDark);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->resize({1800, 600});
    chartView->setAttribute(Qt::WA_DeleteOnClose);
    chartView->show();

    connect(chartView, &QObject::destroyed, [chart, series, axis](){
        delete chart;
        delete series;
        delete axis;
    });
}

void OpenText::searchAndHighlight(QTextDocument *document)
{
    if (m_searchField->text().isEmpty())
        return;

    bool found = false;

    QTextCursor highlightCursor(document);
    QTextCharFormat restore;
    restore.clearBackground();
    highlightCursor.mergeCharFormat(restore);
    QTextCursor cursor(document);

    cursor.beginEditBlock();

    QTextCharFormat plainFormat(highlightCursor.charFormat());
    QTextCharFormat colorFormat = plainFormat;
    colorFormat.setForeground(Qt::red);

    while (!highlightCursor.isNull() && !highlightCursor.atEnd())
    {
        highlightCursor = document->find(m_searchField->text().toUpper(), highlightCursor);

        if (!highlightCursor.isNull())
        {
            found = true;
            highlightCursor.mergeCharFormat(colorFormat);
        }
    }

    cursor.endEditBlock();
}

void OpenText::showBigram()
{
    auto &bigramMap = m_wordsCounter.bigramCount();

    auto tableData = static_cast<AlphabetModel *>(m_tableView->model())->data();
    m_wordsCounter.count(m_openText->toPlainText(), tableData);

    QBarSet *set = new QBarSet("Data");
    if (bigramMap.isEmpty())
        return;

    QVector<QPair<QString, std::size_t>> sortedBigram;
    QMapIterator<QString, size_t> iter(bigramMap);

    while(iter.hasNext())
    {
        iter.next();
        sortedBigram.append(qMakePair(iter.key(), iter.value()));
    }

    std::sort(sortedBigram.begin(), sortedBigram.end(),
              [](const QPair<QString, std::size_t> &first, const QPair<QString, std::size_t> &second){
        return first.second > second.second;
    });

    QStringList categories;
    for (int i = 0; i < 15 && i <= sortedBigram.size()-1; i++)
    {
        set->append(sortedBigram.at(i).second);
        categories.append(QString("\"%1\"").arg(sortedBigram.at(i).first));
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    showXRam(series, categories);
}

void OpenText::showThreegram()
{
    auto &bigramMap = m_wordsCounter.threegramCount();

    auto tableData = static_cast<AlphabetModel *>(m_tableView->model())->data();
    m_wordsCounter.count(m_openText->toPlainText(), tableData);

    QBarSet *set = new QBarSet("Data");
    if (bigramMap.isEmpty())
        return;

    QVector<QPair<QString, std::size_t>> sortedBigram;
    QMapIterator<QString, size_t> iter(bigramMap);

    while(iter.hasNext())
    {
        iter.next();
        sortedBigram.append(qMakePair(iter.key(), iter.value()));
    }

    std::sort(sortedBigram.begin(), sortedBigram.end(),
              [](const QPair<QString, std::size_t> &first, const QPair<QString, std::size_t> &second){
        return first.second > second.second;
    });

    QStringList categories;
    for (int i = 0; i < 15 && i <= sortedBigram.size()-1; i++)
    {
        set->append(sortedBigram.at(i).second);
        categories.append(QString("\"%1\"").arg(sortedBigram.at(i).first));
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    showXRam(series, categories);
}

void OpenText::showSortedByAlphabet()
{
    auto &charactersMap = m_wordsCounter.charactersCount();

    auto tableData = static_cast<AlphabetModel *>(m_tableView->model())->data();
    m_wordsCounter.count(m_openText->toPlainText(), tableData);

    QBarSet *set = new QBarSet("Data");
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

    QBarSeries *series = new QBarSeries();
    series->append(set);

    showXRam(series, categories);
}

void OpenText::showSortedByAscending()
{
    auto &charactersMap = m_wordsCounter.charactersCount();

    auto tableData = static_cast<AlphabetModel *>(m_tableView->model())->data();

    m_wordsCounter.count(m_openText->toPlainText(), tableData);

    QBarSet *set = new QBarSet("Data");
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

    QBarSeries *series = new QBarSeries();
    series->append(set);

    showXRam(series, categories);
}

void OpenText::search()
{
    QTextDocument *document = m_openText->document();

    if (!isFirstTime)
        document->undo();

    searchAndHighlight(document);
    isFirstTime = false;
}

FrequencyWordCounter &OpenText::wordsCounter()
{
    return m_wordsCounter;
}

QTextEdit *OpenText::openText() const
{
    return m_openText;
}

QTableView *OpenText::tableView() const
{
    return m_tableView;
}

QVBoxLayout *OpenText::buttonsLayout() const
{
    return m_buttonsLayout;
}

void OpenText::addTextLieldToLayout()
{
    m_textFieldLayout->addWidget(m_openText);
}

QVBoxLayout *OpenText::textFieldLayout() const
{
    return m_textFieldLayout;
}

