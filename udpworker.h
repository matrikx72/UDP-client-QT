#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>
#include <QHostAddress>

#define BIND_PORT 12345

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket();
    void SendDatagram(QByteArray data);

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket* receiverSocket;
    QUdpSocket* senderSocket;

signals:

    void sig_sendTimeToGUI(QDateTime data);


    void sig_sendMessageToGUI(QString message, QHostAddress senderAddress, int size);
};

#endif
