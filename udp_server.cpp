#include "udp_server.h"


void udpServer::run()
{
    exec();
}

void udpServer::readPendingData()
{
    QVector<QUdpSocket*>::iterator iter;
    for(iter=socketArray.begin(); iter!=socketArray.end(); iter++)
    {
        if((*iter)->hasPendingDatagrams())
        {
            QByteArray datagram;
            QHostAddress sender;
            quint16 senderPort;

            datagram.resize((*iter)->pendingDatagramSize());
            (*iter)->readDatagram(datagram.data(), datagram.size(),
                                    &sender, &senderPort);

            QString clientStr = sender.toString() + QString("-") + QString::number(senderPort);

            const int MAXUDPSIZE = 512;
            //TODO: handle pic data
            {
                if (clientDataHash.contains(clientStr)){
                       clientDataHash[clientStr].append(datagram);
                } else {
                    clientDataHash.insert(clientStr, datagram);
                }
            }
            if(datagram.size() != MAXUDPSIZE) {
                datagram = clientDataHash.value(clientStr);
                clientDataHash[clientStr].clear();

                qDebug() << clientStr << "recv " << datagram.size()/1024 << "kb";
                emit recvData(clientStr, datagram);
            }

       }
    }
}

udpServer::udpServer(int baseport, int count_of_sock, QObject *parent):
    QThread(parent),
    clientArray(),
    clientDataHash(),
    basePort(baseport),
    countSock(count_of_sock),
    errorCount(0)
{
    for(int i=0; i<count_of_sock; i++)
    {
        QUdpSocket *sock=new QUdpSocket();
        sock->bind(basePort+i, QUdpSocket::ReuseAddressHint);
        connect(sock, SIGNAL(readyRead()), this, SLOT(readPendingData()));
        socketArray.push_back(sock);
    }

    qDebug()<<"Server started on "<<countSock<<" ports";
}
