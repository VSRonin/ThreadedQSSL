#include "LanClient.h"
#include <QSslSocket>
#include <QDataStream>
typedef std::underlying_type<MtgGlobals::BbComCodes>::type ComIDType;
LanClient::LanClient(QObject* parent)
	:QObject(parent)
	,m_host("localhost")
	,m_listenPort(MtgGlobals::BbComPort)
	,nextBlockSize(0)
{
	tcpSocket=new QSslSocket(this);
    tcpSocket->setProtocol(QSsl::SslV3);
    connect(tcpSocket, &QSslSocket::connected, this, &LanClient::connected);
    connect(tcpSocket, &QSslSocket::encrypted, this, &LanClient::encrypted);
    connect(tcpSocket, &QSslSocket::disconnected, this, &LanClient::disconnected);
    connect(tcpSocket, &QSslSocket::readyRead, this, &LanClient::incomingTransmission);
    connect(tcpSocket, &QSslSocket::stateChanged, this, &LanClient::stateChanged);
    connect(tcpSocket, static_cast<void (QSslSocket::*)(QAbstractSocket::SocketError)>(&QSslSocket::error), this, &LanClient::error);
    connect(tcpSocket, static_cast<void (QSslSocket::*)(const QList<QSslError>&)>(&QSslSocket::sslErrors), this, &LanClient::sslError);
}
void LanClient::setHost(const QString& a){
		m_host=a;
}
QString LanClient::socketErrorString() const { 
    return tcpSocket->errorString(); 
}

int LanClient::listenPort() const
{
    return m_listenPort;
}

const QString& LanClient::host() const
{
    return m_host;
}

void LanClient::setListenPort(int val)
{
    m_listenPort = val;
}

void LanClient::disconnectFromHost()
{
    tcpSocket->disconnectFromHost();
}


void LanClient::connectToHost()
{
    tcpSocket->connectToHostEncrypted(host(), listenPort());
}

void LanClient::incomingTransmission()
{
	QDataStream incom(tcpSocket);
    incom.setVersion(STREAM_VERSION);
	for(;;) {
		if (nextBlockSize == 0) {
            if (tcpSocket->bytesAvailable() < sizeof(MtgGlobals::BlockSizeType))
				break;
			incom >> nextBlockSize;
		}
		if (tcpSocket->bytesAvailable() < nextBlockSize)
			break;
        // DO Stuff
		nextBlockSize = 0;
	}
}

LanClient::~LanClient() = default;
