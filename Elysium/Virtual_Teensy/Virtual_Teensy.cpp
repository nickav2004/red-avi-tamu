#include "Virtual_Teensy.h"

#include <QDebug>
#include <iostream>
using std::cout;
using std::endl;

Virtual_Teensy::Virtual_Teensy() {
    this->ser = new QSerialPort();
    this->ser->setPortName(PORT);
    this->ser->setBaudRate(BAUD);
    this->ser->open(QSerialPort::ReadWrite);

    if (!this->ser->isOpen()) {
        qDebug() << this->ser->error();
    }

    // Reads are detected automatically
    QObject::connect(this->ser, SIGNAL(readyRead()), this, SLOT(read_data()));

    // Writes are done every INTERVAL (set in header).
    this->timer = new QTimer();
    QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(write_data()));
    this->timer->start(INTERVAL);
}

Virtual_Teensy::~Virtual_Teensy() {
    this->ser->close();
    delete this->ser;
    delete this->timer;
}

void Virtual_Teensy::read_data() {
    cout << "Read: ";
    this->last_read = ser->readAll();
    for (int i = 0; i < this->last_read.size(); ++i) {
        if ('\n' == this->last_read[i]) {
            cout << "\\n" << ", ";
        } else {
            cout << this->last_read[i] << ", ";
        }
    }
    cout << endl;
}

void Virtual_Teensy::write_data() {
    cout << "Write: 1.01,2.40005,12345\\r\\n" << endl;
    this->ser->write("1.01,2.40005,12345\r\n"); 
}
