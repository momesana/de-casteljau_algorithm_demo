#include "animation.h"

#include <QDebug>

Animation::Animation(QObject* parent)
    : QVariantAnimation(parent)
{
}

void Animation::updateCurrentValue(const QVariant &value)
{
    Q_UNUSED(value);
}
