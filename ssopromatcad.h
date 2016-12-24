#ifndef SSOPROMATCAD_H
#define SSOPROMATCAD_H

#include <QMainWindow>
#include "SSopromatCADKernel/SSopromatCADKernel.h"

namespace Ui {
class SSopromatCAD;
}

class SSopromatCAD : public QMainWindow
{
    Q_OBJECT

public:
    explicit SSopromatCAD(QWidget *parent = 0);
    ~SSopromatCAD();

private slots:
    void on_sbCountNode_valueChanged(int arg1);

    void on_sbCountF_valueChanged(int arg1);

    void on_sbCountQ_valueChanged(int arg1);

    void on_bSave_clicked();

private:
    void updateNodeKernel();
    void updateSupportKernel();
    void updateFKernel();
    void updateQKernel();

private:
    Ui::SSopromatCAD *ui;
    SSKPreProccessor * preproc;
};

#endif // SSOPROMATCAD_H
