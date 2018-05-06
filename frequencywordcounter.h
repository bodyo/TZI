#ifndef FREQUENCYWORDCOUNTER_H
#define FREQUENCYWORDCOUNTER_H

#include <QString>
#include <QMap>

class FrequencyWordCounter
{
public:
    void count(const QString &data, const QStringList &alphabet);

    QMap<QChar, std::size_t>& charactersCount();
    QMap<QString, std::size_t>& bigramCount();
    QMap<QString, std::size_t>& threegramCount();

private:
    void setDataForCount(const QString &data, const QStringList& alphabet);

private:
    bool isCounted{false};
    QStringList m_alphabet;
    QString m_data;

    QMap<QChar, std::size_t> m_charactersCount;
    QMap<QString, std::size_t> m_bigramCount;
    QMap<QString, std::size_t> m_threegramCount;
};

#endif // FREQUENCYWORDCOUNTER_H
