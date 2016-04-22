#include "LanServerSocket.h"
#include <QDataStream>
#include <QFile>
LanServerSocket::LanServerSocket(QObject *parent)
    :QSslSocket(parent)
    , m_socketID(0)
    , m_nextBlockSize(0)
{
    connect(this, &LanServerSocket::readyRead, this, &LanServerSocket::readClient);
}
void LanServerSocket::setUpSocket(qintptr soketDescriptor, const QSslKey& key, const QSslCertificate& cert)
{
    if (!setSocketDescriptor(soketDescriptor)) {
        // #TODO
    }
    setProtocol(QSsl::SslV3);
    setPrivateKey(key);
    setLocalCertificate(cert);
    startServerEncryption();
}
void LanServerSocket::readClient()
{
    QDataStream incom(this);
    incom.setVersion(STREAM_VERSION);
    for (;;) {
        if (m_nextBlockSize == 0) {
            if (bytesAvailable() < sizeof(MtgGlobals::BlockSizeType))
                break;
            incom >> m_nextBlockSize;
        }
        if (bytesAvailable() < m_nextBlockSize)
            break;
        // Do Stuff
        m_nextBlockSize = 0;
    }
}







