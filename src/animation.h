#pragma once

#include <QVariantAnimation>

class Animation : public QVariantAnimation
{
    Q_OBJECT
public:
    Animation(QObject *parent = Q_NULLPTR);

protected:
    void updateCurrentValue(const QVariant &value);
};
