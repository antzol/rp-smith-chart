#include "metricnotion.h"

#include <math.h>

//const QStringList MetricNotion::incPrefixes = { QT_TR_NOOP("k"), QT_TR_NOOP("M"), QT_TR_NOOP("G"),
//                                                QT_TR_NOOP("T"), QT_TR_NOOP("P"), QT_TR_NOOP("E"),
//                                                QT_TR_NOOP("Z"), QT_TR_NOOP("Y")};
//const QStringList MetricNotion::decPrefixes = { QT_TR_NOOP("m"), QT_TR_NOOP("u"), QT_TR_NOOP("n"),
//                                                QT_TR_NOOP("p"), QT_TR_NOOP("f"), QT_TR_NOOP("a"),
//                                                QT_TR_NOOP("z"), QT_TR_NOOP("a")};




const QList<const char*> MetricNotion::incPrefixes = { QT_TR_NOOP("k"), QT_TR_NOOP("M"), QT_TR_NOOP("G"),
                                                       QT_TR_NOOP("T"), QT_TR_NOOP("P"), QT_TR_NOOP("E"),
                                                       QT_TR_NOOP("Z"), QT_TR_NOOP("Y")};
const QList<const char*> MetricNotion::decPrefixes = { QT_TR_NOOP("m"), QT_TR_NOOP("u"), QT_TR_NOOP("n"),
                                                       QT_TR_NOOP("p"), QT_TR_NOOP("f"), QT_TR_NOOP("a"),
                                                       QT_TR_NOOP("z"), QT_TR_NOOP("a")};

//---------------------------------------------------------------------------------------
MetricNotion::MetricNotion(double value)
{
    update(value);
}

//---------------------------------------------------------------------------------------
void MetricNotion::update(double value)
{
    int period = (int)floor(log10(abs(value)) / 3);
    double scaled = value * pow(1000, -period);

    int sign = (period > 0) ? 1 : ((period < 0) ? -1 : 0);

    if (scaled == 0 || period == 0 || abs(period) > incPrefixes.size())
    {
        m_convertedValue = value;
    }
    else
    {
        switch (sign)
        {
        case +1:
            m_prefix = tr(incPrefixes[period - 1]);
            break;
        case -1:
            m_prefix = tr(decPrefixes[-period - 1]);
            break;
        }

        m_convertedValue = scaled;
        m_powerOf10 = period * 3;
    }
}

//---------------------------------------------------------------------------------------
double MetricNotion::value() const
{
    return m_convertedValue * pow(10, m_powerOf10);
}

//---------------------------------------------------------------------------------------
double MetricNotion::convertedValue() const
{
    return m_convertedValue;
}

//---------------------------------------------------------------------------------------
QString MetricNotion::prefix() const
{
    return m_prefix;
}

//---------------------------------------------------------------------------------------
int MetricNotion::powerOf10() const
{
    return m_powerOf10;
}

//---------------------------------------------------------------------------------------
bool operator==(const MetricNotion& lhs, const MetricNotion& rhs)
{
    return lhs.m_convertedValue == rhs.m_convertedValue
            && lhs.m_prefix == rhs.m_prefix;
}

//---------------------------------------------------------------------------------------
bool operator!=(const MetricNotion& lhs, const MetricNotion& rhs)
{
    return !(operator==(lhs, rhs));
}

//---------------------------------------------------------------------------------------
QString MetricNotion::getIncPrefix(int i)
{
    if (i < 0 || i >= incPrefixes.size())
        return QString();

    return tr(incPrefixes.at(i));
}

//---------------------------------------------------------------------------------------
QString MetricNotion::getDecPrefix(int i)
{
    if (i < 0 || i >= decPrefixes.size())
        return QString();

    return tr(decPrefixes.at(i));
}

//---------------------------------------------------------------------------------------
