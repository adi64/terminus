#include "ipprovider.h"

#include <QNetworkInterface>

namespace terminus
{

IPProvider::IPProvider(QObject *parent) : QObject(parent)
{

}

IPProvider::~IPProvider()
{

}

QString IPProvider::localIP() const
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for(int nIter=0; nIter<list.count(); nIter++)
    {
        auto ipStr = list[nIter].toString();
        if(ipStr.contains('.') && ipStr != "127.0.0.1")
        {
            return ipStr;
        }
    }
    return QString("no IP");
}

} // namespace terminus
