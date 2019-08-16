#include <QDebug>
#include "Device.h"
#include "Control.h"

Device::Device(QObject *parent)
    : QObject(parent),
      m_ch1Coupling(new EnumControl<Coupling>("ch1Coupling", this)),
      m_ch2Coupling(new EnumControl<Coupling>("ch2Coupling", this)),
      m_startEdge(new EnumControl<Edge>("startEdge", this)),
      m_stopEdge(new EnumControl<Edge>("stopEdge", this)),
      m_countEdge(new EnumControl<Edge>("countEdge", this)),
      m_clock(new EnumControl<Clock>("clock", this)),
      m_mode(new EnumControl<Mode>("mode", this)),
      m_run(new EnumControl<Run>("run", this))
{
}

void Device::call1()
{
    qDebug() << "qwq";
}

QList<Control *> Device::constrols() const
{
    return findChildren<Control *>();
}

QJsonObject Device::toJsonObject() const
{
    QJsonObject object;
    foreach (Control *control, findChildren<Control *>())
        object.insert(control->objectName(), control->toJsonValue());

    return object;
}

void Device::setJsonObject(const QJsonObject &object)
{
    foreach (Control *control, findChildren<Control *>())
        control->setJsonValue(object.value(control->objectName()));
}

