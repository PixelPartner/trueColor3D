#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QtGui>
#include <QCheckBox>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QLabel;
class QMimeData;
class QPushButton;
class QTableWidget;
QT_END_NAMESPACE
class DropArea;

//! [MainWindow header]
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void updateMoviesTable(const QMimeData *mimeData);
    void change( int c, int r);
    void clear();
    void create();
    void about();

signals:
    void changed();

private:
    void createKmq( QTextStream *s, int index, bool turned );
    void createKMQiPad( QTextStream *s, int index, bool turned );
    void createKMQ1080p( QTextStream *s, int index);
    void createKMQ1280( QTextStream *s, int index);
    void create3df33d( QTextStream *s, int index);
    void create3dtv( QTextStream *s, int index);
    void createMy3D960avs( QTextStream *s, int index);
    void createMy3D960( QTextStream *s, int index);
    void createMy3D480p( QTextStream *s, int index);
    void createMy3D720p( QTextStream *s, int index);
    void createMy3D1280( QTextStream *s, int index);
    void createXPol( QTextStream *s, int index, int w, int h);
    void createGRilli( QTextStream *s, int index, int w, int h );

    DropArea *dropArea;
    QLabel *abstractLabel;
    QTableWidget *moviesTable;

    QStringList *types;
    QStringList *aspects;
    QStringList *rates;

    QCheckBox *checkKmqP;
    QCheckBox *checkKmqL;
    QCheckBox *checkKmq1080p;
    QCheckBox *checkKmq1280;
    QCheckBox *checkMy3D960avs;
    QCheckBox *checkMy3D960;
    QCheckBox *checkMy3D480p;
    QCheckBox *checkMy3D720p;
    QCheckBox *checkMy3D1280;
    QCheckBox *checkGRilliPad;
    QCheckBox *checkGRilliTouch;
    QCheckBox *checkFeed;
    QCheckBox *checkXPol1366;
    QCheckBox *check3DTV;

    QPushButton *clearButton;
    QPushButton *createButton;
    QPushButton *aboutButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
};
//! [MainWindow header]

#endif // MAINWINDOW_H
