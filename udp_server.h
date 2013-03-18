#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <QThread>
#include <QUdpSocket>
#include <QMultiMap>

class udpServer : public QThread
{
    Q_OBJECT

    QVector<QUdpSocket*> socketArray;
    QVector<QHostAddress*> clientArray;
    QHash<QString, QByteArray> clientDataHash;
    QMap<int, qint64> packetCount;
    qint16 basePort;
    qint32 countSock;
    qint32 errorCount;
public:
    explicit udpServer(int baseport = 32000, int count_of_sock = 1 ,QObject *parent = 0);
    void run();
    
signals:
     void recvData(const QString &clientStr, const QByteArray &jpegBuffer);
public slots:
    void readPendingData();
    void exit() {
        qDebug() << "udp server exit...";
    }
    
};

#endif // UDP_SERVER_H
