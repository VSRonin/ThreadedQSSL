#ifndef LANSERVERSOCKET_H
#define LANSERVERSOCKET_H
#include <QSslSocket>
#include "Globals.h"
namespace QBbgLib { class QBbgRequestGroup; }
class LanServerSocket : public QSslSocket
{
    Q_OBJECT
public:
    LanServerSocket(QObject *parent = nullptr);
    virtual ~LanServerSocket() = default;
    void setUpSocket(qintptr soketDescriptor, const QSslKey& key, const QSslCertificate& cert);
private:
    qintptr m_socketID;
    MtgGlobals::BlockSizeType m_nextBlockSize;
private slots:
    void readClient();
};
#endif
