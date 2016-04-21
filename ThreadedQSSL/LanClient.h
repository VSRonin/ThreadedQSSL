#ifndef LANCLIENT_H
#define LANCLIENT_H
#include <QObject>
#include <QAbstractSocket>
#include <QSslError>
#include "Globals.h"
class QSslSocket;
namespace QBbgLib { class QBbgRequestGroup; }
class LanClient : public QObject{
	Q_OBJECT
    Q_PROPERTY(QString host READ host WRITE setHost)
    Q_PROPERTY(int port READ listenPort WRITE setListenPort)
private:
	QString m_host;
	int m_listenPort;
	QSslSocket* tcpSocket;
    MtgGlobals::BlockSizeType nextBlockSize;
public:
	LanClient(QObject* parent=nullptr);
    virtual ~LanClient();
	Q_INVOKABLE QString socketErrorString() const;
    int listenPort() const;
    void setListenPort(int val);
    const QString& host() const;
    void setHost(const QString& a);
signals:
    void stateChanged(QAbstractSocket::SocketState sockState);
    void encrypted();
	void error(QAbstractSocket::SocketError socketError);
    void sslError(const QList<QSslError>& errors);
	void connected();
	void disconnected();
private slots:
	void incomingTransmission();
public slots:
	void connectToHost();
	void disconnectFromHost();
};
#endif
