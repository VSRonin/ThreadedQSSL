#include "LanServer.h"
#include "LanServerThread.h"
#include "LanServerSocket.h"
#include "Globals.h"
#include <QFile>
LanServer::LanServer(QObject* parent)
	:QTcpServer(parent)
    , m_portToListen(MtgGlobals::BbComPort)
{
    QByteArray temp;
    QFile file_key("server.key");
    if (file_key.open(QIODevice::ReadOnly)) {
        temp = file_key.readAll();
        file_key.close();
    }
    ssl_key = QSslKey(temp, QSsl::Rsa);
    temp.clear();
    QFile file_cert("server.crt");
    if (file_cert.open(QIODevice::ReadOnly)) {
        temp = file_cert.readAll();
        file_cert.close();
    }
    ssl_cert = QSslCertificate(temp);
}

LanServer::~LanServer() {
    for(auto index= m_clients.begin();index!= m_clients.end();++index){
        index.value()->deleteLater();
    }
}

int LanServer::portToListen() const
{
    return m_portToListen;
}

void LanServer::setPortToListen(int val)
{
    m_portToListen = val;
}

void LanServer::StartListening()
{
	if (isListening()) return;
	if (!listen(QHostAddress::Any, portToListen()))
		emit cantBindPort();
}


void LanServer::incomingConnection(qintptr socketDescriptor)
{
    auto clientIter = m_clients.find(socketDescriptor);
    if (clientIter != m_clients.end()) {
        clientIter.value()->deleteLater();
        clientIter.value() = new LanServerThread(socketDescriptor, ssl_key, ssl_cert, this);
    }
    else {
        clientIter = m_clients.insert(socketDescriptor, new LanServerThread(socketDescriptor, ssl_key, ssl_cert, this));
    }
    connect(clientIter.value(), &LanServerThread::sslErrors, this, &LanServer::sslErrors);
    connect(clientIter.value(), &LanServerThread::error, this, &LanServer::error);
    connect(clientIter.value(), &LanServerThread::stateChanged, this, &LanServer::stateChanged);

    addPendingConnection(clientIter.value()->tcpSocket());

    clientIter.value()->start();
}
void LanServer::EraseThread(qintptr a)
{
    auto index = m_clients.find(a);
    if (index != m_clients.end()) {
        index.value()->deleteLater();
        m_clients.erase(index);
    }
}
