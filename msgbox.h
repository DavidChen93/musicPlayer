#ifndef MSGBOX_H
#define MSGBOX_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QString>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QPoint>
#include <QBitmap>


class MsgBox : public QDialog
{
    Q_OBJECT

public:
    MsgBox(QWidget *parent);
    ~MsgBox();

    bool mouse_press;
    QString ok_text;
    QString cancel_text;
    QPoint move_point;

    QPushButton *close_button;
    QLabel *title_label;
    QLabel *msg_label;
    QLabel *ask_label;
    QCheckBox *check_box;
    QPushButton *cancel_button;
    QPushButton *ok_button;

    void translateLanguage();
    void setInfo(QString, QString, QPixmap, bool, bool);

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);

signals:
    void msgChecked(bool, bool);
    void okMessageHidden(bool);

private slots:
    void okOperate();
    void cancelOperate();
};

#endif // MSGBOX_H
