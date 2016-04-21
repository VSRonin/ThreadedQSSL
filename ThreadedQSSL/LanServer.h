#ifndef LANSERVER_H
#define LANSERVER_H
#include <QTcpServer>
#include <QHash>
#include <QSslKey>
#include <QSslCertificate>
class LanServerThread;
class QSslError;
namespace QBbgLib { 
    class QBbgRequestGroup; 
    class QBbgManager;
}
class LanServer : public QTcpServer{
	Q_OBJECT
private:
	int m_portToListen;
    QHash<qintptr, LanServerThread*> m_clients;
    QSslKey ssl_key;
    QSslCertificate ssl_cert;
protected:
    virtual void incomingConnection(qintptr socketDescriptor) override;
public:
	LanServer(QObject* parent=nullptr);
    virtual ~LanServer();
    int portToListen() const;
    void setPortToListen(int val);
signals:
    void stateChanged(qintptr soketDescriptor, QAbstractSocket::SocketState sockState);
    void error(qintptr soketDescriptor, QAbstractSocket::SocketError socketError);
    void sslErrors(qintptr soketDescriptor, const QList<QSslError>& errors);
    void cantBindPort();
public slots:
    void StartListening();
private slots:
    void EraseThread(qintptr a);
};
#endif
