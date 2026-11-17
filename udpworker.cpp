#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{
    receiverSocket = new QUdpSocket(this);
    senderSocket = new QUdpSocket(this);
}

void UDPworker::InitSocket()
{
    receiverSocket->bind(QHostAddress::LocalHost, BIND_PORT);
    connect(receiverSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
}

void UDPworker::SendDatagram(QByteArray data)
{
    senderSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);
}

void UDPworker::readPendingDatagrams()
{
    while(receiverSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = receiverSocket->receiveDatagram();
        QByteArray data = datagram.data();
        QHostAddress sender = datagram.senderAddress();


        QDataStream inStr(&data, QIODevice::ReadOnly);
        QDateTime dateTime;
        inStr >> dateTime;

        if (dateTime.isValid() && inStr.status() == QDataStream::Ok) {
            emit sig_sendTimeToGUI(dateTime);
        }
        else {
            QString message = QString::fromUtf8(data);
            emit sig_sendMessageToGUI(message, sender, data.size());
        }
    }
}
