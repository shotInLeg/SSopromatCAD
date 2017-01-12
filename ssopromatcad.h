#ifndef SSOPROMATCAD_H
#define SSOPROMATCAD_H

#include <QMainWindow>
#include <QPainter>

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
        if( this->scale == 10 )
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

        this->setMinimumWidth( width+20 );
    }

private:
    SSKPreProccessor * preproc;
    double scale;
};

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

    void on_bUpScale_clicked();

    void on_bDownScale_clicked();

private:
    void updateNodeKernel();
    void updateSupportKernel();
    void updateFKernel();
    void updateQKernel();

private:
    Ui::SSopromatCAD *ui;
    SQRenderWidget * render;
    SSKPreProccessor * preproc;
};

#endif // SSOPROMATCAD_H
