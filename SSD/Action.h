#ifndef ACTION_H
#define ACTION_H

#include <QObject>

class Action : public QObject
{
    Q_OBJECT
public:
    explicit Action(QObject *parent = 0);
    ~Action();

    virtual void execute() = 0;
protected:
    QString name;
signals:

public slots:
};

#endif // ACTION_H
