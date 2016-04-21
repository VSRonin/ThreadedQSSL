#include "LanServerThread.h"
#include "LanServerSocket.h"
LanServerThread::LanServerThread(qintptr SockDesc, const QSslKey& key, const QSslCertificate& cert, QObject* parent)
	:QThread(parent)
	,m_socketID(SockDesc)
{
    m_tcpSocket = new LanServerSocket(this);

    connect(m_tcpSocket, static_cast<void (LanServerSocket::*)(const QList<QSslError>&)>(&LanServerSocket::sslErrors), this, &LanServerThread::sendSslErrors);
    connect(m_tcpSocket, static_cast<void (LanServerSocket::*)(QAbstractSocket::SocketError)>(&LanServerSocket::error), this, &LanServerThread::sendError);
    connect(m_tcpSocket, &LanServerSocket::stateChanged, this, &LanServerThread::sendStateChanged);


    m_tcpSocket->setUpSocket(SockDesc, key, cert);
}

LanServerThread::~LanServerThread() = default;


LanServerSocket* LanServerThread::tcpSocket() const
{
    return m_tcpSocket;
}

void LanServerThread::run()
{
    exec();
}

void LanServerThread::stop()
{
    exit(0);
}
void LanServerThread::sendSslErrors(const QList<QSslError>& errors)
{
    emit sslErrors(m_socketID, errors);
}
void LanServerThread::sendError(QAbstractSocket::SocketError errors)
{
    emit error(m_socketID, errors);
}
void LanServerThread::sendStateChanged(QAbstractSocket::SocketState sockState)
{
    emit stateChanged(m_socketID, sockState);
}
