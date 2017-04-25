#include "ssopromatcad.h"
#include "ui_ssopromatcad.h"
#include <QDebug>

SSopromatCAD::SSopromatCAD(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SSopromatCAD)
{
    ui->setupUi(this);
    preproc = new SSKPreProccessor;
    render = new SQRenderWidget( preproc );

    ui->wPicture->layout()->addWidget( render );
}

SSopromatCAD::~SSopromatCAD()
{
    delete ui;
}

void SSopromatCAD::on_aStart_triggered()
{
    for(int i = 0; i < nodes.size(); i++)
    {
        preproc->addNode( nodes.at(i).x );
    }

    for(int i = 0; i < supports.size(); i++)
    {
        double A = supports.at(i).A;
        double E = supports.at(i).E;
        double G = supports.at(i).G;

        preproc->addSupport( A, E, G );
    }

    for(int i = 0; i < fpowers.size(); i++)
    {
        int numNode = fpowers.at(i).num_node;
        int value = fpowers.at(i).value;

        preproc->addF( numNode, value, 0 );
    }

    for(int i = 0; i < qpowers.size(); i++)
    {
        int numSupport = qpowers.at(i).num_support;
        int value = qpowers.at(i).value;

        preproc->addQ( numSupport, value, 0 );
    }

    render->repaint();

    proc = new SSKProccessor( preproc );

    QVector< double > d = this->proc->gauss();

    QString text = "";
    for( int i = 0; i < d.size(); i++ )
    {
        text += "d" + QString::number(i+1) + " = " + QString::number( d.at(i) ) + "; ";
    }

   ui->lDelta->setText( text );

   postproc = new SSKPostProccessor( preproc, d );
   postproc->proccess();

   /*
   QVector< QVector<double> > npx = postproc->NPX();
   QVector< QVector<double> > upx = postproc->UPX();
   QVector< QVector<double> > ppx = postproc->PPX();

   printTable( ui->twNpx, npx);
   printTable( ui->twUpx, upx);
   printTable( ui->twPpx, ppx);
   */

   curve = new SQCurveWidget( preproc, postproc );
   ui->wGraphic->layout()->addWidget( curve );
}

/*
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
*/

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

/*
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
*/


// Manupulate Methods
void SSopromatCAD::addNode()
{
    if(nodes.size() <= supports.size() && nodes.size() != 0)
        throw std::invalid_argument("Count of node less or equal count of supports");

    nodes.push_back(node());

    if( supports.size() < nodes.size()-1 )
        supports.push_back(support());

    nodes = sort(nodes);
    supports = sort(supports);
}

void SSopromatCAD::addSupport()
{
    if(nodes.size() <= supports.size() && nodes.size() != 0)
        throw std::invalid_argument("Count of node less or equal then count of supports");

    supports.push_back(support());

    if(nodes.size() == supports.size())
        nodes.push_back(node());

    nodes = sort(nodes);
    supports = sort(supports);
}

void SSopromatCAD::addFPower()
{
    if(nodes.size() == 0)
    {
        //Message
        return;
    }

    fpowers.push_back(fpower());

    fpowers = sort(fpowers);
}

void SSopromatCAD::addQPower()
{
    if(supports.size() == 0)
    {
        //Message
        return;
    }

    qpowers.push_back(qpower());

    qpowers = sort(qpowers);
}

void SSopromatCAD::updateNode(int row)
{
    if(row == -1 || row >= nodes.size())
        return;

    nodes[row].x = ui->sbNodeX->value();

    nodes = sort(nodes);
}

void SSopromatCAD::updateSupport(int row)
{
    if(row == -1 || row >= supports.size())
        return;

    supports[row].A = ui->sbSupportA->value();
    supports[row].E = ui->sbSupportE->value();
    supports[row].G = ui->sbSupportG->value();

    supports = sort(supports);
}

void SSopromatCAD::updateFPower(int row)
{
    if(row == -1 || row >= fpowers.size())
        return;

    fpowers[row].num_node = ui->sbFNodeNumber->value();
    fpowers[row].value = ui->sbFValue->value();

    fpowers = sort(fpowers);
}

void SSopromatCAD::updateQPower(int row)
{
    if(row == -1 || row >= qpowers.size())
        return;

    qpowers[row].num_support = ui->sbQSupportNumber->value();
    qpowers[row].value = ui->sbQValue->value();

    qpowers = sort(qpowers);
}

void SSopromatCAD::prepareAndView()
{
    ui->lwNodes->clear();
    for(int i = 0; i < nodes.size(); i++)
    {
        ui->lwNodes->addItem("Node #" + QString::number(i));
    }

    ui->lwSupports->clear();
    for(int i = 0; i < supports.size(); i++)
    {
        ui->lwSupports->addItem("Support #" + QString::number(i));
    }

    ui->lwFPowers->clear();
    for(int i = 0; i < fpowers.size(); i++)
    {
        ui->lwFPowers->addItem("F Power #" + QString::number(i));
    }

    ui->lwQPowers->clear();
    for(int i = 0; i < qpowers.size(); i++)
    {
        ui->lwQPowers->addItem("Q Power #" + QString::number(i));
    }
}

void SSopromatCAD::viewStats(int row)
{
    if(row == -1)
        return;

    if(currentObjects == 0)
    {
        ui->sbNodeX->setValue( nodes.at(row).x );
    }
    else if(currentObjects == 1)
    {
        ui->sbSupportA->setValue( supports.at(row).A );
        ui->sbSupportE->setValue( supports.at(row).E );
        ui->sbSupportG->setValue( supports.at(row).G );
    }
    else if(currentObjects == 2)
    {
        ui->sbFNodeNumber->setValue(fpowers.at(row).num_node);
        ui->sbFValue->setValue(fpowers.at(row).value);
    }
    else if(currentObjects == 3)
    {
        ui->sbQSupportNumber->setValue(qpowers.at(row).num_support);
        ui->sbQValue->setValue(qpowers.at(row).value);
    }
}


// NEW UI

void SSopromatCAD::on_twListsObj_currentChanged(int index)
{
    currentObjects = index;
    ui->swListPropObj->setCurrentIndex(index);
}

void SSopromatCAD::on_bAdd_clicked()
{
    if(currentObjects == 0)
        addNode();
    else if(currentObjects == 1)
        addSupport();
    else if(currentObjects == 2)
        addFPower();
    else if(currentObjects == 3)
        addQPower();

    prepareAndView();
    ui->swListPropObj->setCurrentIndex(currentObjects);
}


void SSopromatCAD::on_lwNodes_currentRowChanged(int currentRow)
{
    viewStats(currentRow);
}

void SSopromatCAD::on_lwSupports_currentRowChanged(int currentRow)
{
    viewStats(currentRow);
}

void SSopromatCAD::on_lwFPowers_currentRowChanged(int currentRow)
{
    viewStats(currentRow);
}

void SSopromatCAD::on_lwQPowers_currentRowChanged(int currentRow)
{
    viewStats(currentRow);
}

void SSopromatCAD::on_sbQSupportNumber_editingFinished()
{
    updateQPower(ui->lwQPowers->currentRow());
}

void SSopromatCAD::on_sbQValue_editingFinished()
{
    updateQPower(ui->lwQPowers->currentRow());
}

void SSopromatCAD::on_sbNodeX_editingFinished()
{
    updateNode(ui->lwNodes->currentRow());
}

void SSopromatCAD::on_sbSupportA_editingFinished()
{
    updateSupport(ui->lwSupports->currentRow());
}

void SSopromatCAD::on_sbSupportE_editingFinished()
{
    updateSupport(ui->lwSupports->currentRow());
}

void SSopromatCAD::on_sbSupportG_editingFinished()
{
    updateSupport(ui->lwSupports->currentRow());
}

void SSopromatCAD::on_sbFNodeNumber_editingFinished()
{
    updateFPower(ui->lwFPowers->currentRow());
}

void SSopromatCAD::on_sbFValue_editingFinished()
{
    updateFPower(ui->lwFPowers->currentRow());
}


