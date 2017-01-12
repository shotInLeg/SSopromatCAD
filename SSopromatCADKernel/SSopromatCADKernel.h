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
    SSKPreProccessor(){}

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
};

#endif // SSOPROMATCADKERNEL_H
