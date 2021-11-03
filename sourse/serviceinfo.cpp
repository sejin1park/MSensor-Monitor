
#include "header/serviceinfo.h"

/*********************************************************************
 * @fn     ServiceInfo
 *
 * @brief  setting services.
 *
 * @param  service - Information you want to know
 *
 * @return   none
 */
ServiceInfo::ServiceInfo(QLowEnergyService *service):
    m_service(service)
{
    m_service->setParent(this);
}
/*********************************************************************
 * @fn     service
 *
 * @brief  return Currently set service
 *
 * @param  none
 *
 * @return   Currently set service
 */
QLowEnergyService *ServiceInfo::service() const
{
    return m_service;
}
/*********************************************************************
 * @fn     getName
 *
 * @brief  get service name
 *
 * @param  none
 *
 * @return   Currently service name
 */
QString ServiceInfo::getName() const
{
    if (!m_service)
        return QString();

    return m_service->serviceName();
}
/*********************************************************************
 * @fn     getType
 *
 * @brief  get service Type
 *
 * @param  none
 *
 * @return   Currently service type
 */
QString ServiceInfo::getType() const
{
    if (!m_service)
        return QString();

    QString result;
    if (m_service->type() & QLowEnergyService::PrimaryService)
        result += QStringLiteral("primary");
    else
        result += QStringLiteral("secondary");

    if (m_service->type() & QLowEnergyService::IncludedService)
        result += QStringLiteral(" included");

    result.prepend('<').append('>');

    return result;
}
/*********************************************************************
 * @fn     getUuid
 *
 * @brief  get service uudi
 *
 * @param  none
 *
 * @return   Currently service uuid
 */
QString ServiceInfo::getUuid() const
{
    if (!m_service)
        return QString();

    const QBluetoothUuid uuid = m_service->serviceUuid();
    bool success = false;
    quint16 result16 = uuid.toUInt16(&success);
    if (success)
        return QStringLiteral("0x") + QString::number(result16, 16);

    quint32 result32 = uuid.toUInt32(&success);
    if (success)
        return QStringLiteral("0x") + QString::number(result32, 16);

    return uuid.toString().remove(QLatin1Char('{')).remove(QLatin1Char('}'));
}
