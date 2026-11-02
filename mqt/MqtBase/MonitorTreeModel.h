#ifndef MONITORTREEMODEL_H
#define MONITORTREEMODEL_H

#include <QSharedPointer>
#include <QAbstractItemModel>

class MonitoredLog;


//=============================================================================================
enum MonitorNodeType{entRoot, entRouter, entLog};
class MonitorTreeNode: public QObject
{
    MonitorNodeType ntype;
protected:
    QString NodeName;
    QString hintString;
    int childNo;
    MonitorTreeNode *parentItem;
    QSharedPointer< MonitoredLog> mlog;

public:
    MonitorTreeNode(MonitorNodeType sn, MonitorTreeNode *parent, QString name);
    MonitorTreeNode(MonitorNodeType sn, MonitorTreeNode *parent, QSharedPointer< MonitoredLog> log);
    virtual ~MonitorTreeNode();

    virtual MonitorNodeType GetNodeType() const
    {
        return ntype;
    }
    QVector<MonitorTreeNode *> nodes;
    int childNumber() const;

    int find( const MonitorTreeNode *t ) const;

    void clear();

    virtual QString Name (  ) const
    {
        return NodeName;
    }
    virtual QString hint() const
    {
        return hintString;
    }
    virtual QString data( int column ) = 0;

    MonitorTreeNode *parent();
    MonitorTreeNode *child( int number );
    int childCount() const;
    QSharedPointer< MonitoredLog> getLog()
    {
        return mlog;
    }
//    void setLog(QSharedPointer< MonitoredLog>l)
//    {
//        mlog = l;
//    }
};
class RootTreeNode:public MonitorTreeNode
{
public:
    RootTreeNode():MonitorTreeNode(entRoot, nullptr, "Root")
    {

    }
    virtual QString data( int column );
};
class RouterTreeNode:public MonitorTreeNode
{
public:
    RouterTreeNode(MonitorTreeNode *parent, QString name):MonitorTreeNode(entRouter, parent, name)
    {
    }
    virtual QString data( int column );
};
class LogTreeNode:public MonitorTreeNode
{
    Q_OBJECT
public:
    LogTreeNode(MonitorTreeNode *parent,QSharedPointer< MonitoredLog> log):MonitorTreeNode(entLog, parent, log)
    {
    }
    virtual QString data( int column );
};
class MonitorTreeModel: public QAbstractItemModel
{
    Q_OBJECT

    MonitorTreeNode *rootData;
public:
    MonitorTreeModel();
    ~MonitorTreeModel() override;
    void setRoot(  MonitorTreeNode *root );
    void clear();

    QVariant data( const QModelIndex &index, int role ) const Q_DECL_OVERRIDE;
    QVariant headerData( int section, Qt::Orientation orientation,
                         int role ) const Q_DECL_OVERRIDE;
    QModelIndex index( int row, int column,
                       const QModelIndex &parent = QModelIndex() ) const Q_DECL_OVERRIDE;
    QModelIndex parent( const QModelIndex &index ) const Q_DECL_OVERRIDE;

    int rowCount( const QModelIndex &parent = QModelIndex() ) const Q_DECL_OVERRIDE;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const Q_DECL_OVERRIDE;
    MonitorTreeNode *getItem( const QModelIndex &index ) const;
};

#endif // MONITORTREEMODEL_H
