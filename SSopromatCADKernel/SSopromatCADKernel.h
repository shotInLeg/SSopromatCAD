#ifndef SSOPROMATCADKERNEL_H
#define SSOPROMATCADKERNEL_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QPair>
#include <QDebug>

struct Node
{
    int x;
    int xF;
    int yF;

    Node()
    {
        this->x = 0;
        this->xF = 0;
        this->yF = 0;
    }

    Node( int x )
    {
        this->x = x;
        this->xF = 0;
        this->yF = 0;
    }
};

struct Support
{
    int startX;
    double L;
    double A;
    double E;
    double G;
    int xQ;
    int yQ;

    Support()
    {
        this->startX = 0;
        this->L = 0;
        this->A = 0;
        this->E = 0;
        this->G = 0;
        this->xQ = 0;
        this->yQ = 0;
    }

    Support( int startX, double L, double A, double E, double G )
    {
        this->startX = startX;
        this->L = L;
        this->A = A;
        this->E = E;
        this->G = G;
        this->xQ = 0;
        this->yQ = 0;
    }


};

class SSKPreProccessor
{
public:
    SSKPreProccessor()
    {
        this->_leftSeal = true;
        this->_rightSeal = true;
    }

    void addNode( int x )
    {
        _nodes.push_back( Node( x ) );
    }

    void addSupport( int sn, int en, double A, double E, double G )
    {
        int L = _nodes.at(en).x - _nodes.at(sn).x;

        _supports.push_back( Support( _nodes.at(sn).x, L, A, E, G ) );
    }

    void addF( int numNode, int xF, int yF )
    {
        if( numNode >= _nodes.size() )
            return;

        _nodes[numNode].xF = xF;
        _nodes[numNode].yF = yF;
    }

    void addQ( int numSup, int xQ, int yQ )
    {
        if( numSup >= _supports.size() )
            return;

        _supports[numSup].xQ = xQ;
        _supports[numSup].yQ = yQ;
    }

    QVector<Support> supports() const
    {
        return this->_supports;
    }

    QVector<Node> nodes() const
    {
        return this->_nodes;
    }

    bool leftSeal() const
    {
        return this->_leftSeal;
    }

    bool rightSeal() const
    {
        return this->_rightSeal;
    }

    void debugView() const
    {
        qDebug() << "-----DEBUG NODES-------";
        for( int i = 0; i < _nodes.size(); i++ )
        {
            qDebug() << "    " << i << ": " << _nodes.at(i).x  << " F: " << _nodes.at(i).xF << " " << _nodes.at(i).yF;;
        }

        qDebug() << "-----DEBUG SUPPORTS-------";
        for( int i = 0; i < _supports.size(); i++ )
        {
            qDebug() << "    " << i << ": " << _supports.at(i).startX << " " << _supports.at(i).L << " " << _supports.at(i).A << " "
                     << _supports.at(i).E << " " << _supports.at(i).G << " Q: " << _supports.at(i).xQ << " " << _supports.at(i).yQ;
        }
    }

private:
    QVector<Node> _nodes;
    QVector<Support> _supports;
    bool _leftSeal;
    bool _rightSeal;
};

class SSKProccessor
{
public:
    SSKProccessor( SSKPreProccessor * preproc )
    {
        this->_preproc = preproc;

        QVector< Support > supports = this->_preproc->supports();
        QVector< Node > nodes = this->_preproc->nodes();

        this->_matrixA.resize( supports.size()+1 );
        for( int i = 0; i < supports.size()+1; i++ )
        {
            this->_matrixA[i].resize( supports.size()+1 );
        }


        for( int i = 0; i < this->_matrixA.size(); i++ )
        {
            for( int j = 0; j < this->_matrixA.at(i).size(); j++ )
            {
                this->_matrixA[i][j] = 0;
            }
        }


        int x = 0; int y = 0;
        for( int i = 0; i < supports.size(); i++ )
        {
            double q = -((supports.at(i).xQ * supports.at(i).L) / 2);
            this->_vectorQ.push_back( q );

            double pK = (supports.at(i).E * supports.at(i).A) / supports.at(i).L;

            this->_matrixA[x][y] += pK;
            this->_matrixA[x+1][y] += -pK;
            this->_matrixA[x][y+1] += -pK;
            this->_matrixA[x+1][y+1] += pK;

            x++;
            y++;
        }

        for( int i = 0; i < supports.size()+1; i++ )
        {
            double sQ = 0;
            double kQ = 0;

            if( i < supports.size() )
            {
                double Q = supports.at(i).xQ;
                double L = supports.at(i).L;
                sQ = -(Q * L / 2);
                int a = 0;
            }

            if( i > 0 )
            {
                double Q = supports.at(i-1).xQ;
                double L = supports.at(i-1).L;
                kQ = -(Q * L / 2);
                int a = 0;
            }

            double B = (-sQ) + (-kQ) + nodes.at(i).xF;
            this->_vectorB.push_back( B );
        }
        //Δ1= 0; Δ2= 1; Δ3= 1.5; Δ4= 1; Δ5= 0;

        qDebug() << "Matrix A:";
        for( int i = 0; i < this->_matrixA.size(); i++ )
        {
            QString text = "";
            for( int j = 0; j < this->_matrixA.at(i).size(); j++ )
            {
                text += QString::number( this->_matrixA[i][j] ) + "  ";
            }
            qDebug() << text;
            text = "";
        }

        qDebug() << "Vector B:";
        QString text = "";
        for( int i = 0; i < this->_vectorB.size(); i++ )
        {
            text += QString::number( this->_vectorB[i] ) + " ";
        }
        qDebug() << text;
    }

    QVector< double > gauss()
    {
        QVector< Support > supports = this->_preproc->supports();
        QVector< QVector<double> > A = this->_matrixA;
        QVector< double > B = this->_vectorB;
        int n = supports.size() + 1;

        qDebug() << A.size() << " " << A.at(0).size();

        if( this->_preproc->leftSeal() )
        {
            for( int i = 1; i < A.at(0).size(); i++ )
            {
                A[0][i] = 0;
            }

            B[0] = 0;
        }

        if( this->_preproc->rightSeal() )
        {
            for( int i = 0; i < A.at( A.size()-1 ).size()-1; i++ )
            {
                A[ A.size()-1 ][i] = 0;
            }

            B[ B.size()-1 ] = 0;
        }



        qDebug() << B;

        for( int k = 0; k < n; k++ )
        {
            if( A[k][k] == 0.0 )
            {
                for( int i = k+1; i < n; i++ )
                {
                    if ( A[i][k] != 0.0 )
                    {
                        for (int j = 0; j < n; ++j)
                        {
                            double temp = A[i][j];
                            A[i][j] = A[k][j];
                            A[k][j] = temp;
                        }

                        double temp = B[i];
                        B[i] = B[k];
                        B[k] = temp;

                        break;
                    }
                }
            }

            double div = A[k][k];

            for( int j = 0; j < n; j++ )
                A[k][j] /= div;

            B[k] /= div;

            for( int i = k + 1; i < n; i++ )
            {
                double multi = A[i][k];

                for( int j = 0; j < n; j++ )
                    A[i][j] -= multi * A[k][j];

                B[i] -= multi * B[k];
            }
        }

        for( int k = n-1; k > 0; k-- )
        {
            for (int i = k-1; i + 1 > 0; i-- )
            {
                double multi = A[i][k];

                for (int j = 0; j < n; ++j)
                    A[i][j] -= multi * A[k][j];

                B[i] -= multi * B[k];
            }
        }

        qDebug() << B;

        return B;
    }


private:
    SSKPreProccessor * _preproc;
    QVector< QVector<double> > _matrixA;
    QVector< double > _vectorQ;
    QVector< double > _vectorB;


};

//num1 di +
class SSKPostProccessor
{
public:
    SSKPostProccessor( SSKPreProccessor * pre, QVector< double > delta )
    {
        this->_preproc = pre;
        this->_delta = delta;
    }

    void proccess()
    {
        QVector<Support> supports = this->_preproc->supports();

        QVector<double> num;
        QVector< QVector< double > > upX;


        for( int i = 0; i < supports.size(); i++ )
        {
            num.push_back( supports.at(i).L / 50 );
        }

        double num1 = 0;
        double sonum = 0;
        for(int i = 0; i < num.size(); i++)
        {
            num1 = 0;
            sonum = num.at(i);

            QVector<double> upX1;
            while( num1 < supports.at(i).L )
            {
                upX1.push_back(this->_delta[i] + (num1/supports.at(i).L) * (this->_delta[i+1] - this->_delta[i]) + (supports.at(i).xQ * supports.at(i).L *supports.at(i).L * num1) / (2*supports.at(i).E * supports.at(i).A * supports.at(i).L) * (1 - (num1/supports.at(i).L)) );
                num1 += sonum;
            }
            this->_upx.push_back(upX1);

            num1 = 0;
            QVector<double> npX1;
            while(num1 < supports.at(i).L)
            {
                npX1.push_back(supports.at(i).E * supports.at(i).A / supports.at(i).L * (this->_delta[i + 1] - this->_delta[i]) + supports.at(i).xQ * supports.at(i).L / 2 * (1 - 2* (num1/supports.at(i).L)) );
                num1 += sonum;
            }
            this->_npx.push_back(npX1);
        }

        QVector<double> pxX;
        for(int i = 0; i < this->_npx.size(); i++)
        {
            for(int j = 0; j < this->_npx.at(i).size(); j++)
            {
                pxX.push_back(this->_npx.at(i).at(i)/ supports.at(i).A);
            }
            this->_ppx.push_back(pxX);
            pxX.clear();
        }

        //qDebug() << "NPX: " << this->_npx;
        //qDebug() << "UPX: " << this->_upx;
        //qDebug() << "PPX: " << this->_ppx;
    }

    QVector< QVector< double > > NPX() const
    {
        return this->_npx;
    }

    QVector< QVector< double > > UPX() const
    {
        return this->_upx;
    }

    QVector< QVector< double > > PPX() const
    {
        return this->_ppx;
    }

private:
    SSKPreProccessor * _preproc;
    QVector<double> _delta;

    QVector< QVector< double > > _upx;
    QVector< QVector<double> > _npx;
    QVector< QVector<double> > _ppx;
};

#endif // SSOPROMATCADKERNEL_H
