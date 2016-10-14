#include "settings.h"

#include <QVariant>
#include <QDebug>

Settings::Settings(QObject* parent)
    : QSettings(parent)
    , m_notificationsEnabled(true)
{
}


Settings* Settings::m_instance = nullptr;

Settings* Settings::instance()
{
    if (m_instance == nullptr)
        m_instance = new Settings();

    return m_instance;
}


void Settings::setValue(const QString &key, const QVariant &value)
{
    QSettings::setValue(key, value);
    if (m_notificationsEnabled) {
        emit updated(key, value);
    }

}

void Settings::enableNotifications(bool on)
{
    m_notificationsEnabled = on;
}
