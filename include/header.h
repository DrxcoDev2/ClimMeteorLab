#ifndef HEADER_H
#define HEADER_H

QWidget* crearHeader(const QString &titulo, QWidget *parent = nullptr) {
    QWidget *header = new QWidget(parent);
    header->setFixedHeight(25); 
    header->setStyleSheet("background-color: #000080;");
    header->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *layout = new QHBoxLayout(header);
    layout->setContentsMargins(5, 0, 5, 0);
    layout->setSpacing(5);

    QLabel *titleLabel = new QLabel(titulo);
    titleLabel->setStyleSheet("color: white; font-weight: bold;");
    layout->addWidget(titleLabel);
    layout->addStretch();

    QPushButton *btnMin = new QPushButton("-");
    btnMin->setFixedSize(20, 20);
    btnMin->setStyleSheet(
        "QPushButton { background-color: #c0c0c0; border: 2px outset #ffffff; }"
        "QPushButton:pressed { border: 2px inset #ffffff; background-color: #a0a0a0; }"
    );
    layout->addWidget(btnMin);

    QPushButton *btnClose = new QPushButton("X");
    btnClose->setFixedSize(20, 20);
    btnClose->setStyleSheet(
        "QPushButton { background-color: #c0c0c0; border: 2px outset #ffffff; }"
        "QPushButton:pressed { border: 2px inset #ffffff; background-color: #a0a0a0; }"
    );
    layout->addWidget(btnClose);

    QObject::connect(btnClose, &QPushButton::clicked, [parent]() { parent->close(); });
    QObject::connect(btnMin, &QPushButton::clicked, [parent]() { parent->showMinimized(); });

    return header;
}

#endif // HEADER_H