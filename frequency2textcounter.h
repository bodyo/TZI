#ifndef FREQUENCY2TEXTCOUNTER_H
#define FREQUENCY2TEXTCOUNTER_H

#include <QString>
#include <QMap>

class Frequency2TextCounter
{
public:
    void count(const QString &text1, const QString &text2, const QStringList &alphabet);

    QMap<QChar, std::size_t>& charactersCount();
    QMap<QString, std::size_t>& bigramCount();
    QMap<QString, std::size_t>& threegramCount();

private:
    void setDataForCount(const QString &data, const QStringList& alphabet);

private:
    bool isCounted{false};
    QStringList m_alphabet;
    QString m_text1;
    QString m_text2;

    QMap<QChar, std::size_t> m_charactersCount;
    QMap<QString, std::size_t> m_bigramCount;
    QMap<QString, std::size_t> m_threegramCount;
};

#endif // FREQUENCY2TEXTCOUNTER_H
