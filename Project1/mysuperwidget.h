#ifndef MYSUPERWIDGET_H
#define MYSUPERWIDGET_H

#include <QWidget>

class MySuperWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MySuperWidget(QWidget *parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:

public slots:
    void paintEvent(QPaintEvent *event) override;
    void myUpdate();

private:
    QTimer* updateTimer;
};

#endif // MYSUPERWIDGET_H
