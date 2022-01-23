#ifndef CONFIG_H
#define CONFIG_H

#include <complex>
#include <map>

#include <QColor>
#include <QPair>
#include <QStringView>

class QString;
class QSettings;

class Config
{
public:
    enum
    {
        ColorResistanceCircle,
        ColorReactanceArc,
        ColorConductanceCircle,
        ColorSusceptanceArc,

        ColorSeriesCapacitor,
        ColorSeriesInductance,
        ColorParallelCapacitor,
        ColorParallelInductance,

        ColorSourceImpedancePoint,
        ColorSourceImpedanceConjugatePoint,
        ColorLoadImpedancePoint,

        WidthChartLine,
        WidthComponentLine,
        ImpedancePointSize,

        ChartCirclesValues,
        ChartArcsValues,

        ImpedanceReSource,
        ImpedanceImSource,
        ImpedanceReLoad,
        ImpedanceImLoad,
        ImpedanceZ0,

        FrequencyCenter,
        FrequencySpan,
        FrequencyStep,

        ComponentDefaultCapacitor,
        ComponentDefaultInductance,

        Language
    };

    static Config* instance();

    QColor getColor(int key);
    void setColor(int key, const QColor &color);

    QString getStringValue(int key);
    void setStringValue(int key, const QString &val);

    int getIntValue(int key);
    void setIntValue(int key, int val);

    double getDoubleValue(int key);
    void setDoubleValue(int key, double val);

    QVector<double> getDoubleVector(int key);
    void setDoubleVector(int key, const QVector<double> vect);

    QString getRecentSavePath();
    void setRecentSavePath(const QString& fileName);

    double getComponentDefaultValue(const QString& componentSymbol);

    std::complex<double> getSourceImpedanceDefaultValue();
    std::complex<double> getLoadImpedanceDefaultValue();



private:
    Config();

    static Config* m_instance;

    QSettings *m_setting;

    // Pair is <group, key> in the ini-file
    static const QMap<int, QPair<QString, QString>> m_stringValues;
    static const QMap<int, QPair<QString, QString>> m_intValues;
    static const QMap<int, QPair<QString, QString>> m_doubleValues;
    static const QMap<int, QPair<QString, QString>> m_doubleArrayValues;

};

#endif // CONFIG_H
