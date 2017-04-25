#ifndef SSOPROMATCAD_H
#define SSOPROMATCAD_H

#include <QMainWindow>
#include <QPainter>
#include <QTableWidget>

#include "SSopromatCADKernel/SSopromatCADKernel.h"

namespace Ui {
class SSopromatCAD;
}

class SQRenderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SQRenderWidget(QWidget *parent = 0){ this->scale = 1; }
    explicit SQRenderWidget(SSKPreProccessor * preproc, QWidget *parent = 0)
    {
        this->preproc = preproc;
        this->scale = 1;
    }
    ~SQRenderWidget(){}

    void upScale()
    {
        if( this->scale == 100 )
            return;

        this->scale+=0.25;
    }

    void downScale()
    {
        if( this->scale == 0.25 )
            return;

        this->scale-=0.25;
    }


    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);

        QVector<Support> support = this->preproc->supports();
        int h2 = this->height() / 2;


        int width = 0;
        //Отрисовка стержней
        for( int i = 0; i < support.size(); i++ )
        {
            //Стержень
            painter.setPen(QPen(Qt::green, 1, Qt::SolidLine, Qt::RoundCap));


            width += support.at(i).L*scale;
            int topY = h2 - ( ( support.at(i).A * scale ) / 2);

            painter.drawRect( ( support.at(i).startX * scale )+50, topY, support.at(i).L*scale, support.at(i).A*scale );

            //Распределенная нагрузка
            painter.setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap));

            if( support.at(i).xQ != 0 )
            {
                int x = ( support.at(i).startX * scale )+50;
                while( x+10 <= ( support.at(i).startX * scale )+50 + support.at(i).L*scale )
                {
                    painter.drawLine( x, h2, x+10, h2 );

                    if( support.at(i).xQ > 0 )
                    {
                        painter.drawLine( x+5, h2-5, x+10, h2 );
                        painter.drawLine( x+5, h2+5, x+10, h2 );
                    }
                    else
                    {
                        painter.drawLine( x+5, h2-5, x, h2 );
                        painter.drawLine( x+5, h2+5, x, h2 );
                    }

                    x+=10;
                }
            }

            if( support.at(i).yQ != 0 )
            {
                int sx = ( support.at(i).startX * scale )+50;
                int nx = ( support.at(i).startX * scale )+50 + support.at(i).L*scale;

                painter.drawLine( sx, topY-10, nx, topY-10 );

                int x = sx;
                while( x+15 <= nx )
                {
                    painter.drawLine( x, topY-10, x, topY );
                    painter.drawLine( x-5, topY-5, x, topY );
                    painter.drawLine( x+5, topY-5, x, topY );

                    x+=15;
                }
            }

            //Заделки
            painter.setPen(QPen(Qt::green, 1, Qt::SolidLine, Qt::RoundCap));

            //Левая заделка
            if( i == 0 )
            {
                int y = topY;
                while( y < topY + support.at(i).A*scale )
                {
                    int xFrom = (support.at(i).startX*scale)+50 - 10; //Левее на 10px левой грани
                    int xTo = (support.at(i).startX*scale)+50; //Ровно левая грань
                    painter.drawLine( xFrom, y + 5, xTo, y );
                    y+=5;
                }
            }

            //Правая заделка
            if( i == support.size()-1 )
            {
                int y = topY;
                while( y < topY + support.at(i).A*scale )
                {

                    int xFrom = (support.at(i).startX*scale)+50 + support.at(i).L*scale + 10; //Правее на 10px правой грани
                    int xTo = (support.at(i).startX*scale)+50 + support.at(i).L*scale; //Ровно правая грань
                    painter.drawLine( xFrom, y + 5, xTo, y );
                    y+=5;
                }
            }
        }

        QVector<Node> nodes = this->preproc->nodes();

        //Сосредоточенная нагрузка
        painter.setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap));

        for( int i = 0; i < nodes.size(); i++ )
        {


            int topY = h2 - ( ( support.at(int(i/2)).A * scale ) / 2);
            if( nodes.at(i).xF != 0 )
            {

                int x = ( nodes.at(i).x * scale )+50;
                if( nodes.at(i).xF > 0 )
                {
                    painter.drawLine( x-20, h2, x, h2 );
                    painter.drawLine( x-13, h2-5, x, h2 );
                    painter.drawLine( x-13, h2+5, x, h2 );
                }
                else
                {
                    painter.drawLine( x+20, h2, x, h2 );
                    painter.drawLine( x+7, h2-5, x, h2 );
                    painter.drawLine( x+7, h2+5, x, h2 );
                }

                x+=10;
            }

            if( nodes.at(i).yF != 0 )
            {
                int sx = ( nodes.at(i).x * scale )+50;

                painter.drawLine( sx, topY-20, sx, topY );
                painter.drawLine( sx-5, topY-5, sx, topY );
                painter.drawLine( sx+5, topY-5, sx, topY );
            }
        }

        this->setMinimumWidth( width+50 );
    }

private:
    SSKPreProccessor * preproc;
    double scale;
};

class SQCurveWidget : public QWidget
{
public:
    explicit SQCurveWidget(QWidget *parent = 0){ this->scale = 1; }
    explicit SQCurveWidget(SSKPreProccessor * preproc, SSKPostProccessor * postproc, QWidget *parent = 0)
    {
        this->scale = 1;
        this->preproc = preproc;
        this->postproc = postproc;

        this->viewNpx = false;
        this->viewUpx = true;
        this->viewPpx = false;
    }
    ~SQCurveWidget(){}

    void upScale()
    {
        if( this->scale == 100 )
            return;

        this->scale+=0.25;
    }

    void downScale()
    {
        if( this->scale == 0.25 )
            return;

        this->scale-=0.25;
    }

    void paintEvent(QPaintEvent *)
    {
        QVector< QVector<double> > npx = postproc->NPX();
        QVector< QVector<double> > upx = postproc->UPX();
        QVector< QVector<double> > ppx = postproc->PPX();
        //double scale = frameSize().height() / 4;

        for( int i = 0; i < npx.size(); i++ )
        {
            for(int j = 0; j < npx.at(i).size(); j++)
            {
                npx[i][j] = npx[i][j]  * frameSize().height() / 4;
                upx[i][j] = upx[i][j] * frameSize().height() / 4;
                ppx[i][j] = ppx[i][j] * frameSize().height() / 4;
            }
        }


        QVector<Support> supports = this->preproc->supports();

        int countSupports = this->preproc->supports().size();
        int totalL = 0;
        int countSection = 50;
        for( int i = 0; i < this->preproc->supports().size(); i++ )
        {
            totalL += this->preproc->supports().at(i).L;
        }


        QPainter painter(this);
        double fu = 0;
        int x = 0;
        int y = 0;
        y =(frameSize().height() )/4;
        x = 0;
        painter.translate(0, 0);
        painter.drawText(10, frameSize().height()/4, "+");
        painter.drawText(10, frameSize().height()/4 + frameSize().height()/4+ frameSize().height()/4, "-");

        painter.translate(x, y);

        double num = 0;
        int size = 0;
        int check = 0;
        painter.translate(x,y);
        painter.drawLine(0, 0, frameSize().width(), 0);
        painter.drawLine(0, 0, -frameSize().width(), 0);

        if(viewUpx == true)
        {
            for(int i = 0; i < upx.size(); i++)
            {

                size =  upx.at(i).size() -1;
                fu = supports.at(i).L / countSection * frameSize().height() /4;

                int j =0;
                while(j < size)
                {
                    painter.drawLine(num, upx.at(i).at(j), num+fu, upx.at(i).at(j+1));
                    j++;
                    painter.drawLine(num, upx.at(i).at(j),num, 0 );
                    num += fu;

                }
                painter.drawLine(num,frameSize().width(), num, -frameSize().width() );
                painter.drawText(num, 0, QString::number( supports.at(i).L) );
                fu = 0;
            }
        }

        num = 0;
        size =0;
        fu = 0;

        if(viewNpx == true)
        {
            qDebug() << "NPX: " << npx;
            for(int i = 0; i < npx.size(); i++)
            {
                fu = supports.at(i).L / countSection * frameSize().width() /6;
                size =  npx.at(i).size() - 1;


                int j =0;
                while(j < size)
                {
                    painter.drawLine(num, npx.at(i).at(j), num+fu, npx.at(i).at(j+1));
                    j++;
                    painter.drawLine(num, npx.at(i).at(j),num, 0 );
                    num += fu;

                }
                painter.drawLine(num,frameSize().width(), num, -frameSize().width() );
                painter.drawText(num, 0, QString::number(supports.at(i).L));
                fu = 0;
            }
        }

        if(viewPpx == true)
        {
            for(int i = 0; i < ppx.size(); i++)
            {

                fu = supports.at(i).L / countSection * frameSize().width() /6;
                size =  ppx.at(i).size() - 1;


                int j =0;
                while(j < size)
                {
                    painter.drawLine(num, ppx.at(i).at(j), num+fu, ppx.at(i).at(j+1));
                    j++;
                    painter.drawLine(num, ppx.at(i).at(j),num, 0 );
                    num += fu;

                }
                painter.drawLine(num,frameSize().width(), num, -frameSize().width() );
                painter.drawText(num, 0, QString::number( supports.at(i).L ));
                fu = 0;
            }
        }
    }

private:
    SSKPreProccessor * preproc;
    SSKPostProccessor * postproc;

    bool viewNpx;
    bool viewUpx;
    bool viewPpx;

    double scale;
};



class node
{
public:
    int x;

    node():x(0){}

    bool operator<(const node& b)
    {
        return this->x < b.x;
    }
};

class support
{
public:
    int A;
    int E;
    int G;

    support():A(0),E(0),G(0){}

    bool operator<(const support& b)
    {
        return this->A < b.A;
    }
};

class fpower
{
public:
    int num_node;
    int value;

    fpower():num_node(0),value(0){}

    bool operator<(const fpower& b)
    {
        return this->num_node < b.num_node;
    }
};

class qpower
{
public:
    int num_support;
    int value;

    qpower():num_support(0),value(0){}

    bool operator<(const qpower& b)
    {
        return this->num_support < b.num_support;
    }
};


class SSopromatCAD : public QMainWindow
{
    Q_OBJECT

public:
    explicit SSopromatCAD(QWidget *parent = 0);
    ~SSopromatCAD();

private slots:
    /*
    void on_sbCountNode_valueChanged(int arg1);

    void on_sbCountF_valueChanged(int arg1);

    void on_sbCountQ_valueChanged(int arg1);

    void on_bSave_clicked();

    void on_bUpScale_clicked();

    void on_bDownScale_clicked();

    void on_twMainWindow_currentChanged(int index);
    */

    void printTable( QTableWidget * table, QVector< QVector< double > > mtx );

    /*
    void on_bScaleUpCurve_clicked();

    void on_bScaleDownCurve_clicked();
    */

    void on_twListsObj_currentChanged(int index);

    void on_bAdd_clicked();

    void on_lwSupports_currentRowChanged(int currentRow);

    void on_lwNodes_currentRowChanged(int currentRow);

    void on_lwFPowers_currentRowChanged(int currentRow);

    void on_lwQPowers_currentRowChanged(int currentRow);

    void on_sbQSupportNumber_editingFinished();

    void on_sbQValue_editingFinished();

    void on_sbNodeX_editingFinished();

    void on_sbSupportA_editingFinished();

    void on_sbSupportE_editingFinished();

    void on_sbSupportG_editingFinished();

    void on_sbFNodeNumber_editingFinished();

    void on_sbFValue_editingFinished();

    void on_aStart_triggered();

private:
    void addNode();
    void addSupport();
    void addFPower();
    void addQPower();

    void updateNode(int row);
    void updateSupport(int row);
    void updateFPower(int row);
    void updateQPower(int row);

    template<class T>
    QVector<T> sort(const QVector<T>& list)
    {
        if(list.size() == 0)
            return list;

        QVector<T> sorted = list;

        for(int i = 0; i < sorted.size()-1; i++)
        {
            for(auto j = i+1; j < sorted.size(); j++)
            {
                if( sorted[j] < sorted[i] )
                {
                    T temp = sorted[j];
                    sorted[j] = sorted[i];
                    sorted[i] = temp;
                }
            }
        }

        return sorted;
    }

    void prepareAndView();
    void viewStats(int row);

    void updateNodeKernel();
    void updateSupportKernel();
    void updateFKernel();
    void updateQKernel();

private:
    Ui::SSopromatCAD *ui;
    SQRenderWidget * render;
    SQCurveWidget * curve;
    SSKPreProccessor * preproc;
    SSKProccessor * proc;
    SSKPostProccessor * postproc;

    int currentObjects;

    QVector<node> nodes;
    QVector<support> supports;
    QVector<fpower> fpowers;
    QVector<qpower> qpowers;
};

#endif // SSOPROMATCAD_H
