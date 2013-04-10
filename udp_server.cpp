#include "udp_server.h"
 #include<QFile>

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
            const int UDPDATA_MAX_SIZE = 1024;

            QByteArray datagram(UDPDATA_MAX_SIZE+6, '0');
            QHostAddress sender;
            quint16 senderPort;
//            struct frame_info {
//                int8_t 	mask[2];	// 'FM' const
//                int32_t	len;	//
//                int8_t	data[UDPDATA_MAX_SIZE];
//            };
            (*iter)->readDatagram(datagram.data(), datagram.size(),
                                    &sender, &senderPort);

            QString clientStr = sender.toString() + QString("-") + QString::number(senderPort);

            if(datagram[0] != 'F' || datagram[1] != 'M'){
                qDebug() << "mask not match";
                (*iter)->writeDatagram("ERROR", 6, sender, senderPort);
                continue;
            }

            (*iter)->writeDatagram("OK", 3, sender, senderPort);

            const int *dataPointer = (const int *)(datagram.constData() + 2);
            const int datalen = *dataPointer;

            clientDataHash[clientStr].append(datagram.constData() + 6, datalen);
            if(datalen != UDPDATA_MAX_SIZE) {
                emit recvData(clientStr, clientDataHash.value(clientStr));
                clientDataHash[clientStr].clear();
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
        //sock->bind(QHostAddress::Any, basePort+i);
        connect(sock, SIGNAL(readyRead()), this, SLOT(readPendingData()));
        socketArray.push_back(sock);
    }

    qDebug()<<"Server started on "<<countSock<<" ports";
}
