#ifndef COLORCOUNTDIALOG_H
#define COLORCOUNTDIALOG_H

#include <QWidget>
#include <QScrollArea>
#include <QDialog>
#include <QVector>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <iostream>

class ColorCountDialog : public QDialog
{
    Q_OBJECT

public:
    ColorCountDialog(QVector<QRgb> *colors, QVector<int> *counts, QString title="Colors Counted");
};

#endif
