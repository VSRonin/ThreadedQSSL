#ifndef LANSERVERTHREAD_H
#define LANSERVERTHREAD_H
#include <QThread>
#include <QAbstractSocket>
class QSslError;
class LanServerSocket;
class QSslKey;
class QSslCertificate;
namespace QBbgLib { class QBbgRequestGroup; }
class LanServerThread : public QThread
{
    Q_OBJECT
private:
    qintptr m_socketID;
    LanServerSocket* m_tcpSocket;
public:
    LanServerThread(qintptr SockDesc, const QSslKey& key, const QSslCertificate& cert, QObject* parent = nullptr);
    virtual ~LanServerThread();
    LanServerSocket* tcpSocket() const;
public slots:
    void run();
    void stop();
signals:
    void sslErrors(qintptr soketDescriptor, const QList<QSslError>& errors);
    void error(qintptr soketDescriptor, QAbstractSocket::SocketError socketError);
    void stateChanged(qintptr soketDescriptor, QAbstractSocket::SocketState sockState);
private slots:
    void sendSslErrors(const QList<QSslError>& errors);
    void sendError(QAbstractSocket::SocketError socketError);
    void sendStateChanged(QAbstractSocket::SocketState sockState);
};
#endif
