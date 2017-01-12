#include "ssopromatcad.h"
#include "ui_ssopromatcad.h"

SSopromatCAD::SSopromatCAD(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SSopromatCAD)
{
    ui->setupUi(this);
    preproc = new SSKPreProccessor;
    render = new SQRenderWidget( preproc );

    ui->scrollAreaWidgetContents->layout()->addWidget( render );
}

SSopromatCAD::~SSopromatCAD()
{
    delete ui;
}

void SSopromatCAD::on_sbCountNode_valueChanged(int arg1)
{
    ui->twNode->setRowCount( arg1 );

    if( arg1 >= 1 )
        ui->twSupport->setRowCount( arg1 - 1 );
}

void SSopromatCAD::on_sbCountF_valueChanged(int arg1)
{
    ui->twF->setRowCount( arg1 );
}

void SSopromatCAD::on_sbCountQ_valueChanged(int arg1)
{
    ui->twQ->setRowCount( arg1 );
}

void SSopromatCAD::on_bSave_clicked()
{
    updateNodeKernel();
    updateSupportKernel();
    updateFKernel();
    updateQKernel();

    preproc->debugView();

    render->repaint();
}

void SSopromatCAD::updateSupportKernel()
{
    int countRows = ui->twSupport->rowCount();

    for( int i = 0; i < countRows; i++ )
    {
        QTableWidgetItem * curItem = ui->twSupport->item( i, 0 );

        int sn = i;
        int en = i + 1;
        double A = ui->twSupport->item( i, 0 )->text().toDouble();
        double E = ui->twSupport->item( i, 1 )->text().toDouble();
        double G = ui->twSupport->item( i, 2 )->text().toDouble();

        preproc->addSupport( sn, en, A, E, G );
    }
}

void SSopromatCAD::updateFKernel()
{
    int countRows = ui->twF->rowCount();

    for( int i = 0; i < countRows; i++ )
    {
        int n = ui->twF->item( i, 0 )->text().toInt();;
        int xF = ui->twF->item( i, 1 )->text().toInt();
        int yF = ui->twF->item( i, 2 )->text().toInt();


        preproc->addF( n, xF, yF );
    }
}

void SSopromatCAD::updateQKernel()
{
    int countRows = ui->twQ->rowCount();

    for( int i = 0; i < countRows; i++ )
    {
        int n = ui->twQ->item( i, 0 )->text().toInt();;
        int xQ = ui->twQ->item( i, 1 )->text().toInt();
        int yQ = ui->twQ->item( i, 2 )->text().toInt();

        preproc->addQ( n, xQ, yQ );
    }
}

void SSopromatCAD::updateNodeKernel()
{
    int countRows = ui->twNode->rowCount();

    for( int i = 0; i < countRows; i++ )
    {
        QTableWidgetItem * curItem = ui->twNode->item( i, 0 );
        preproc->addNode( curItem->text().toDouble() );
    }
}

void SSopromatCAD::on_bUpScale_clicked()
{
    render->upScale();
    render->repaint();
}

void SSopromatCAD::on_bDownScale_clicked()
{
    render->downScale();
    render->repaint();
}
