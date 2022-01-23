#include "config.h"

#include <QSettings>

#include <QApplication>
#include <QDir>
#include <QMap>
#include <QStandardPaths>

Config* Config::m_instance = nullptr;

const QMap<int, QPair<QString, QString>> Config::m_stringValues
{
    {Config::ColorResistanceCircle,   {"Colors", "ResistanceCircle"}},
    {Config::ColorReactanceArc,       {"Colors", "ReactanceArc"}},
    {Config::ColorConductanceCircle,  {"Colors", "ConductanceCircle"}},
    {Config::ColorSusceptanceArc,     {"Colors", "SusceptanceArc"}},
    {Config::ColorSeriesCapacitor,    {"Colors", "SeriesCapacitor"}},
    {Config::ColorSeriesInductance,   {"Colors", "SeriesInductance"}},
    {Config::ColorParallelCapacitor,  {"Colors", "ParallelCapacitor"}},
    {Config::ColorParallelInductance, {"Colors", "ParallelInductance"}},

    {Config::ColorSourceImpedancePoint, {"Colors", "SourceImpedancePoint"}},
    {Config::ColorSourceImpedanceConjugatePoint, {"Colors", "SourceImpedanceConjugatePoint"}},
    {Config::ColorLoadImpedancePoint,   {"Colors", "LoadImpedancePoint"}},

    {Config::Language,                 {"Preferences", "Language"}}

};

const QMap<int, QPair<QString, QString>> Config::m_intValues
{
    {Config::WidthChartLine,          {"ChartSizes", "WidthChartLine"}},
    {Config::WidthComponentLine,      {"ChartSizes", "WidthComponentLine"}},
    {Config::ImpedancePointSize,      {"ChartSizes", "ImpedancePointSize"}},

    {Config::ImpedanceZ0,             {"Impedances", "Z0"}}
};

const QMap<int, QPair<QString, QString>> Config::m_doubleValues
{
    {Config::ImpedanceReSource,    {"Impedances", "ReSource"}},
    {Config::ImpedanceImSource,    {"Impedances", "ImSource"}},
    {Config::ImpedanceReLoad,      {"Impedances", "ReLoad"}},
    {Config::ImpedanceImLoad,      {"Impedances", "ImLoad"}},

    {Config::FrequencyCenter,      {"Frequencies", "Center"}},
    {Config::FrequencySpan,        {"Frequencies", "Span"}},
    {Config::FrequencyStep,        {"Frequencies", "Step"}},

    {Config::ComponentDefaultCapacitor,   {"Components", "DefaultCapacitor"}},
    {Config::ComponentDefaultInductance,  {"Components", "DefaultInductance"}}
};


const QMap<int, QPair<QString, QString>> Config::m_doubleArrayValues
{
    {Config::ChartCirclesValues,      {"ChartValues", "CircleValues"}},
    {Config::ChartArcsValues,         {"ChartValues", "ArcsValues"}}
};

//---------------------------------------------------------------------------------------
Config::Config()
{
    QString configFile = QApplication::applicationDirPath() + "/" +"config.ini";
    m_setting = new QSettings(configFile, QSettings::IniFormat);
}

//---------------------------------------------------------------------------------------
Config* Config::instance()
{
    if(!m_instance)
        m_instance = new Config();

    return m_instance;
}

//---------------------------------------------------------------------------------------
QColor Config::getColor(int key)
{
    return QColor(getStringValue(key));
}

//---------------------------------------------------------------------------------------
void Config::setColor(int key, const QColor &color)
{
    setStringValue(key, color.name(QColor::HexRgb));
}

//---------------------------------------------------------------------------------------
QString Config::getStringValue(int key)
{
    QString val;

    if (!m_stringValues.contains(key))
        return val;

    m_setting->beginGroup(m_stringValues[key].first);
    val = m_setting->value(m_stringValues[key].second).toString();
    m_setting->endGroup();

    return val;
}

//---------------------------------------------------------------------------------------
void Config::setStringValue(int key, const QString &val)
{
    if (!m_stringValues.contains(key))
        return;

    m_setting->beginGroup(m_stringValues[key].first);
    m_setting->setValue(m_stringValues[key].second, val);
    m_setting->endGroup();
}

//---------------------------------------------------------------------------------------
int Config::getIntValue(int key)
{
    int val = 0;

    if (!m_intValues.contains(key))
        return val;

    m_setting->beginGroup(m_intValues[key].first);
    val = m_setting->value(m_intValues[key].second).toInt();
    m_setting->endGroup();

    return val;
}

//---------------------------------------------------------------------------------------
void Config::setIntValue(int key, int val)
{
    if (!m_intValues.contains(key))
        return;

    m_setting->beginGroup(m_intValues[key].first);
    m_setting->setValue(m_intValues[key].second, val);
    m_setting->endGroup();
}

//---------------------------------------------------------------------------------------
double Config::getDoubleValue(int key)
{
    double val = 0;

    if (!m_doubleValues.contains(key))
        return val;

    m_setting->beginGroup(m_doubleValues[key].first);
    val = m_setting->value(m_doubleValues[key].second).toDouble();
    m_setting->endGroup();

    return val;
}

//---------------------------------------------------------------------------------------
void Config::setDoubleValue(int key, double val)
{
    if (!m_doubleValues.contains(key))
        return;

    m_setting->beginGroup(m_doubleValues[key].first);
    m_setting->setValue(m_doubleValues[key].second, val);
    m_setting->endGroup();
}

//---------------------------------------------------------------------------------------
QVector<double> Config::getDoubleVector(int key)
{
    QVector<double> vect;

    if (!m_doubleArrayValues.contains(key))
        return vect;

    m_setting->beginGroup(m_doubleArrayValues[key].first);
    int size = m_setting->beginReadArray(m_doubleArrayValues[key].second);
    vect.reserve(size);
    for (int i = 0; i < size; ++i)
    {
        m_setting->setArrayIndex(i);
        double val = m_setting->value("value").toDouble();
        vect.append(val);
    }
    m_setting->endArray();
    m_setting->endGroup();

    return vect;
}

//---------------------------------------------------------------------------------------
void Config::setDoubleVector(int key, const QVector<double> vect)
{
    if (!m_doubleArrayValues.contains(key))
        return;

    m_setting->beginGroup(m_doubleArrayValues[key].first);
    m_setting->remove(m_doubleArrayValues[key].second);
    m_setting->beginWriteArray(m_doubleArrayValues[key].second);
    for (int i = 0; i < vect.size(); ++i)
    {
        m_setting->setArrayIndex(i);
        m_setting->setValue("value", vect[i]);
    }
    m_setting->endArray();
    m_setting->endGroup();

    return;
}

//---------------------------------------------------------------------------------------
QString Config::getRecentSavePath()
{
    m_setting->beginGroup("RecentData");
    QString path = m_setting->value("RecentSavePath").toString();
    m_setting->endGroup();

    if (path.isEmpty())
        path = QDir::homePath();

    return path;
}

//---------------------------------------------------------------------------------------
void Config::setRecentSavePath(const QString& fileName)
{
    QDir d = QFileInfo(fileName).absoluteDir();
    QString path = d.absolutePath();

    m_setting->beginGroup("RecentData");
    m_setting->setValue("RecentSavePath", path);
    m_setting->endGroup();
}

//---------------------------------------------------------------------------------------
double Config::getComponentDefaultValue(const QString& componentSymbol)
{
    const QMap<QString, int> keys{{"L", ComponentDefaultInductance},
                                  {"C", ComponentDefaultCapacitor}};
    double mostDefaultValue = 1e-12;

    if (!keys.contains(componentSymbol))
        return mostDefaultValue;

    double val = getDoubleValue(keys[componentSymbol]);

    return val ? val : mostDefaultValue;
}

//---------------------------------------------------------------------------------------
std::complex<double> Config::getSourceImpedanceDefaultValue()
{
    double re = getDoubleValue(ImpedanceReSource);
    double im = getDoubleValue(ImpedanceImSource);

    return std::complex<double>(re, im);
}

//---------------------------------------------------------------------------------------
std::complex<double> Config::getLoadImpedanceDefaultValue()
{
    double re = getDoubleValue(ImpedanceReLoad);
    double im = getDoubleValue(ImpedanceImLoad);

    return std::complex<double>(re, im);
}

//---------------------------------------------------------------------------------------
