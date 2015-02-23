#pragma once

#include <QObject>

namespace terminus
{

class Train;
class QMLWagon;

class QMLTrain : public QObject
{    
    Q_OBJECT
    //Q_PROPERTY(Train* train READ train WRITE setTrain NOTIFY trainChanged)
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)

public:
    QMLTrain(Train *train = nullptr);
    ~QMLTrain();

    Train *train() const;
    void setTrain(Train *train);
    Q_INVOKABLE QMLWagon *wagonAt(unsigned int index) const;
    Q_INVOKABLE unsigned int size() const;

signals:
    void trainChanged();
    void sizeChanged();

private:
    Train *m_train;
};

}
