#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();

    // Подключаем старый сигнал (время)
    connect(udpWorker, &UDPworker::sig_sendTimeToGUI, this, &MainWindow::DisplayTime);

    // Подключаем новый сигнал (текст)
    connect(udpWorker, &UDPworker::sig_sendMessageToGUI, this, &MainWindow::DisplayMessage);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{
        QDateTime dateTime = QDateTime::currentDateTime();
        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);
        outStr << dateTime;
        udpWorker->SendDatagram(dataToSend);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
}

void MainWindow::on_pb_stop_clicked()
{
    timer->stop();
}

// --- НОВЫЙ СЛОТ ДЛЯ ОТПРАВКИ ---
void MainWindow::on_pb_send_clicked()
{
    QString text = ui->le_message->text(); // Имя виджета для ввода
    if (text.isEmpty()) return;

    QByteArray dataToSend = text.toUtf8();
    udpWorker->SendDatagram(dataToSend);
}

// --- СТАРЫЙ СЛОТ ДЛЯ ВРЕМЕНИ (оставляем как было) ---
void MainWindow::DisplayTime(QDateTime data)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }
    ui->te_result->append("Текущее время: " + data.toString() + ". "
                          "Принято пакетов " + QString::number(counterPck));
}

// --- НОВЫЙ СЛОТ ДЛЯ СООБЩЕНИЙ ---
void MainWindow::DisplayMessage(QString message, QHostAddress sender, int size)
{
    // Формат по заданию: "Принято сообщение от адрес отправителя, размер сообщения(байт) Х"
    QString displayStr = QString("Принято сообщение от %1, размер сообщения(байт) %2")
                            .arg(sender.toString())
                            .arg(size);

    ui->te_result->append(displayStr);
}
