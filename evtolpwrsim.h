#ifndef EVTOLPWRSIM_H
#define EVTOLPWRSIM_H

#include <QMainWindow>
#include <QStandardItem>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui { class eVtolPwrSim; }
QT_END_NAMESPACE

class eVtolPwrSim : public QMainWindow
{
    Q_OBJECT

public:
    eVtolPwrSim(QWidget *parent = nullptr);
    ~eVtolPwrSim();

private slots:
    void on_runPwrSimBtn_clicked();

private:
    void extracted(QStringList &rowData, QList<QStandardItem *> &items);
    Ui::eVtolPwrSim *ui;
};
#endif // EVTOLPWRSIM_H
