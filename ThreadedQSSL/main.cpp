
#include <QCoreApplication>
#include <LanClient.h>
#include <LanServer.h>
#include <QDebug>
#include <QSslError>
#include <QAbstractSocket>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    LanServer serverSide;
    LanClient clientSide;

    QObject::connect(&serverSide, &LanServer::cantBindPort, []() {
        qDebug() << "Server Port not Binded"; 
    });
    QObject::connect(&clientSide, &LanClient::connected, []() {
        qDebug() << "Client Connected"; 
    });
    QObject::connect(&clientSide, &LanClient::encrypted, [&clientSide]() {
        qDebug() << "Client Encrypted";
    });
    QObject::connect(&clientSide, &LanClient::error, [](QAbstractSocket::SocketError err) {
        qDebug() << "Client SocketError " << err; 
    });
    QObject::connect(&serverSide, &LanServer::error, [](qintptr, QAbstractSocket::SocketError err) {
        qDebug() << "Server SocketError " << err;
    });
    QObject::connect(&serverSide, &LanServer::sslErrors, [](qintptr, const QList<QSslError>& err) {
        qDebug() << "Server SSL Error";
        for (const QSslError& singeErr : err) {
            qDebug() << singeErr.errorString();
        }
    });
    QObject::connect(&clientSide, &LanClient::sslError, [](const QList<QSslError>& err) {
        qDebug() << "Client SSL Error"; 
        for (const QSslError& singeErr : err){
            qDebug() << singeErr.errorString();
        }
    });
    QObject::connect(&clientSide, &LanClient::stateChanged, [](QAbstractSocket::SocketState sockState) {
        qDebug() << "Client State Changed " << sockState;
    });
    QObject::connect(&serverSide, &LanServer::stateChanged, [](qintptr, QAbstractSocket::SocketState sockState) {
        qDebug() << "Server State Changed " << sockState;
    });


    serverSide.StartListening();
    clientSide.connectToHost();
    
    return a.exec();
}
