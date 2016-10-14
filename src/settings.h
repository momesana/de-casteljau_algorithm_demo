#pragma once

#include <QSettings>

#include <functional>

class Settings : public QSettings
{
    Q_OBJECT
public:
    static Settings* instance();
    void setValue(const QString &key, const QVariant &value);

//    void addListener(const QString& string, std::function<void(const QString& key, const QVariant& value)> f);
    void enableNotifications(bool on);
protected:
    explicit Settings(QObject *parent = Q_NULLPTR);
    Settings (const Settings&);

signals:
    void updated(const QString& key, const QVariant& variant);

private:
//    QMap<const QString, std::function<void(const QString&, const QVariant&)>> m_listeners;
    static Settings *m_instance;
    bool m_notificationsEnabled;
};

