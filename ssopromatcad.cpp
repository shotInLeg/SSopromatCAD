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
        int n = ui->twF->item( i, 0 )->text().toInt()-1;
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
        int n = ui->twQ->item( i, 0 )->text().toInt()-1;
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

void SSopromatCAD::on_twMainWindow_currentChanged(int index)
{
    if( index == 1 )
    {
        proc = new SSKProccessor( preproc );

        QVector< double > d = this->proc->gauss();

        QString text = "";
        for( int i = 0; i < d.size(); i++ )
        {
            text += "d" + QString::number(i+1) + " = " + QString::number( d.at(i) ) + "; ";
        }

        ui->lDelta->setText( text );

        postproc = new SSKPostProccessor( preproc, d );
    }

    if( index == 2 )
    {
        postproc->proccess();

        QVector< QVector<double> > npx = postproc->NPX();
        QVector< QVector<double> > upx = postproc->UPX();
        QVector< QVector<double> > ppx = postproc->PPX();

        printTable( ui->twNpx, npx);
        printTable( ui->twUpx, upx);
        printTable( ui->twPpx, ppx);

        curve = new SQCurveWidget( preproc, postproc );
        ui->wCurve->layout()->addWidget( curve );
    }
}

void SSopromatCAD::printTable(QTableWidget *table, QVector<QVector<double> > mtx)
{
    table->setRowCount( mtx.at(0).size() );
    table->setColumnCount( mtx.size() );

    for( int i = 0; i < mtx.size(); i++ )
    {
        for( int j = 0; j < mtx.at(i).size(); j++ )
        {
            table->setItem( j, i, new QTableWidgetItem( QString::number( mtx.at(i).at(j) ) ) );
            //table->item(j, i)->setText( QString::number( mtx.at(i).at(j) ) );
        }
    }
}

void SSopromatCAD::on_bScaleUpCurve_clicked()
{
   curve->upScale();
   curve->repaint();
}

void SSopromatCAD::on_bScaleDownCurve_clicked()
{
    curve->downScale();
    curve->repaint();
}
