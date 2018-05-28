#include "frequencywordcounter.h"

#include <QDebug>

void FrequencyWordCounter::count(const QString &data, const QStringList &alphabet)
{
    setDataForCount(data, alphabet);

    if (isCounted)
        return;

    isCounted = true;

    for (auto iter = m_data.begin(); iter != m_data.end(); ++iter)
    {
        if (*iter == "\n" /*|| (!m_alphabet.contains(*iter))*/)
            continue;
        m_charactersCount.insert(*iter, m_charactersCount.value(*iter, 0)+1);

        QString ram = *iter;
        if (iter+1 != m_data.end())
            ram.append(*(iter+1));
        else
            break;

        m_bigramCount.insert(ram, m_bigramCount.value(ram, 0)+1);
        ram.append(*(iter+2));
        m_threegramCount.insert(ram, m_threegramCount.value(ram, 0)+1);
    }
}

void FrequencyWordCounter::setDataForCount(const QString &data, const QStringList &alphabet)
{
    if ((m_data == data) && (alphabet == m_alphabet))
        return;

    m_charactersCount.clear();
    m_threegramCount.clear();
    m_bigramCount.clear();

    isCounted = false;
    m_data = data;
    m_alphabet = alphabet;
}

QMap<QChar, std::size_t>& FrequencyWordCounter::charactersCount()
{
    return m_charactersCount;
}

QMap<QString, std::size_t>& FrequencyWordCounter::bigramCount()
{
    return m_bigramCount;
}

QMap<QString, std::size_t>& FrequencyWordCounter::threegramCount()
{
    return m_threegramCount;
}
