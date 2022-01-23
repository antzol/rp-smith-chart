#ifndef METRICNOTION_H
#define METRICNOTION_H

#include <QCoreApplication>
#include <QString>
#include <QStringList>

class MetricNotion
{
    Q_DECLARE_TR_FUNCTIONS(MetricNotion)
public:
    explicit MetricNotion(double value = 0);

    void update(double value);

    double value() const;
    double convertedValue() const;
    int powerOf10() const;
    QString prefix() const;

    friend bool operator==(const MetricNotion& lhs, const MetricNotion& rhs);
    friend bool operator!=(const MetricNotion& lhs, const MetricNotion& rhs);

    static QString getIncPrefix(int i);
    static QString getDecPrefix(int i);

//    static const QStringList incPrefixes;
//    static const QStringList decPrefixes;

//    static const QList<const char*> incPrefixes;
//    static const QList<const char*> decPrefixes;


    static const QList<const char*> incPrefixes;
    static const QList<const char*> decPrefixes;


private:
    double m_convertedValue;
    int m_powerOf10{0};
    QString m_prefix;

};

#endif // METRICNOTION_H
