#include "frequency2textcounter.h"

#include <QDebug>

void Frequency2TextCounter::count(const QString &data, const QString &text2, const QStringList &alphabet)
{
    setDataForCount(data, alphabet);

    if (isCounted)
        return;

    isCounted = true;

    for (auto iter = m_text1.begin(); iter != m_text1.end(); ++iter)
    {
        if (*iter == "\n" || (!m_alphabet.contains(*iter)))
            continue;
        m_charactersCount.insert(*iter, m_charactersCount.value(*iter, 0)+1);

        QString ram = *iter;
        if (iter+1 != m_text1.end())
            ram.append(*(iter+1));
        else
            break;

        m_bigramCount.insert(ram, m_bigramCount.value(ram, 0)+1);
        ram.append(*(iter+2));
        m_threegramCount.insert(ram, m_threegramCount.value(ram, 0)+1);
    }
}

void Frequency2TextCounter::setDataForCount(const QString &text1, const QString &text2, const QStringList &alphabet)
{
    if (m_text1 == text1 && m_text2 == text2 && alphabet == m_alphabet)
        return;
    isCounted = false;
    m_text1 = text1;
    m_alphabet = alphabet;
}

QMap<QChar, std::size_t>& Frequency2TextCounter::charactersCount()
{
    return m_charactersCount;
}

QMap<QString, std::size_t>& Frequency2TextCounter::bigramCount()
{
    return m_bigramCount;
}

QMap<QString, std::size_t>& Frequency2TextCounter::threegramCount()
{
    return m_threegramCount;
}
