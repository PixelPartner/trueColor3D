#include <QtGui>
#include "droparea.h"
#include "mainwindow.h"

#ifdef Q_WS_WIN
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

MainWindow::MainWindow()
{
    types = new QStringList();
    *types << tr("SBS LR") << tr("SBS RL") << tr("OU RL") << tr("OU LR");

    aspects = new QStringList();
    *aspects << tr("16:9") << tr("4:3") << tr("custom");

    rates = new QStringList();
    *rates << tr("keep") << tr("23.98") << tr("24") << tr("25") << tr("29.97") << tr("30");

    abstractLabel = new QLabel(tr("Drag in your downloaded or edited 3-D movie(s)."));
    abstractLabel->setWordWrap(true);
    abstractLabel->adjustSize();

    dropArea = new DropArea;
    connect(dropArea, SIGNAL(changed(const QMimeData*)),
            this, SLOT(updateMoviesTable(const QMimeData*)));

    QStringList labels;
    labels << tr("Type") << tr("Aspect") << tr("frame rate") << tr("File Name");

    moviesTable = new QTableWidget;
    moviesTable->setColumnCount(labels.count());
    moviesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    moviesTable->setHorizontalHeaderLabels(labels);
    moviesTable->horizontalHeader()->setStretchLastSection(true);

    checkKmq1080p = new QCheckBox(tr("iPad3D for Youtube3D (RonL)"));
    checkKmq1280 = new QCheckBox(tr("iPad3D for vimeo3D (rotated)"));
    checkMy3D480p = new QCheckBox(tr("TOYin3D and Haspro My3D (480p)"));
    checkMy3D720p = new QCheckBox(tr("TOYin3D and Haspro My3D (720p)"));
    checkMy3D960 = new QCheckBox(tr("TOYin3D and Haspro My3D (960px)"));

    checkKmqP = new QCheckBox(tr("&openKMQ/iPad3D native res. (768x1024)"));
    checkKmqL = new QCheckBox(tr("iPad3D &landscape (for AirVideo)"));
    checkGRilliPad = new QCheckBox(tr("&GRilli3D lenticular film for iPad"));
    checkGRilliTouch = new QCheckBox(tr("GRilli3D lenticular film for iPhone/iPodTouch Gen&3"));
    checkFeed = new QCheckBox(tr("&Video for 3Df33d.tv"));
    checkMy3D960avs = new QCheckBox(tr("TOYin3D and Haspro My3D (960px)"));
    checkMy3D1280 = new QCheckBox(tr("TOYin3D and Haspro My3D (1280px)"));
    check3DTV = new QCheckBox(tr("&s-b-s frame compatible for 3DTV"));
    /// \todo add over/under TV
    checkXPol1366 = new QCheckBox(tr("&XPol 1366x786 for ACER w/ passive glasses"));

    QGroupBox *vimeoBox = new QGroupBox(tr("Output options for vimeo"));
    QVBoxLayout *vbox1 = new QVBoxLayout;
    vbox1->addWidget(checkKmq1280);
    vbox1->addWidget(checkMy3D480p); // war checkMy3D1280
    vbox1->addWidget(checkGRilliPad);
    vbox1->addWidget(checkGRilliTouch);
    vbox1->addWidget(checkXPol1366);
    vbox1->addStretch(1);
    vimeoBox->setLayout(vbox1);

    QGroupBox *youtubeBox = new QGroupBox(tr("Output options for Youtube"));
    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->addWidget(checkKmq1080p);
    vbox2->addWidget(checkMy3D720p);
    vbox2->addStretch(1);
    youtubeBox->setLayout(vbox2);

    QGroupBox *nativeBox = new QGroupBox(tr("Output options for others"));
    QVBoxLayout *vbox3 = new QVBoxLayout;
    vbox3->addWidget(checkKmqP);
    vbox3->addWidget(checkKmqL);
    vbox3->addWidget(check3DTV);
    vbox3->addWidget(checkFeed);
    vbox3->addWidget(checkMy3D960);
    vbox3->addStretch(1);
    nativeBox->setLayout(vbox3);

    // prechecked
    checkKmq1080p->setChecked(true);
    checkKmq1280->setChecked(true);
    checkMy3D720p->setChecked(true);

#ifndef Q_WS_WIN
    // disable some options, only available with AviSynth
    checkFeed->setEnabled(false);
    checkGRilliTouch->setEnabled(false);
    check3DTV->setEnabled(false);
    //checkMy3D960avs->setEnabled(false);
    checkMy3D1280->setEnabled(false);
    checkGRilliPad->setEnabled(false);
    checkGRilliTouch->setEnabled(false);
    checkXPol1366->setEnabled(false);
#endif

    clearButton  = new QPushButton(tr("Clear movie list"));
    createButton = new QPushButton(tr("Create scripts"));
    aboutButton  = new QPushButton(tr("About"));
    quitButton   = new QPushButton(tr("Quit"));

    QVBoxLayout *buttonBox = new QVBoxLayout;
    buttonBox->addWidget(clearButton,1);
    buttonBox->addWidget(createButton,5);
    buttonBox->addWidget(aboutButton,1);
    buttonBox->addWidget(quitButton,1);

    QHBoxLayout *creationBox = new QHBoxLayout;
    creationBox->addWidget(nativeBox);
    creationBox->addWidget(youtubeBox);
    creationBox->addWidget(vimeoBox);
    creationBox->addLayout(buttonBox,2);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(abstractLabel);
    mainLayout->addWidget(dropArea);
    mainLayout->addWidget(moviesTable);
    mainLayout->addLayout(creationBox);

    setLayout(mainLayout);

    setWindowTitle(tr("trueColor3D, the 3-D video converter for openKMQ, openMy3D and openGRilli3D"));
    setMinimumSize(600, 300);

    // und nur die Verkettung von Buttons/Actions mit Methoden
    connect(quitButton, SIGNAL(pressed()), this, SLOT(close()));
    connect(clearButton, SIGNAL(pressed()), this, SLOT(clear()));
    connect(createButton, SIGNAL(pressed()), this, SLOT(create()));
    connect(aboutButton, SIGNAL(pressed()), this, SLOT(about()));

    connect(moviesTable, SIGNAL(cellClicked(int,int)), this, SLOT(change(int,int)));
}

void MainWindow::about()
{
    QMessageBox::about( this, tr("About this program"),
                        "trueColor3D, version 0.99c\n"
                        "(C) 2010, 2011 by Thomas Kumlehn,\n"
                        "released under GPL 3.0\n"
                        "Find updates and news under\n"
                        "http://openKMQ.blogspot.com\n\n"
                        "This software was made cross platform using Qt, gcc, "
#ifdef Q_WS_WIN
                        "MinGW"
#else
                        "QtCreator"
#endif
                        "\n"
                        "and needs also other tools installed to run:\n"
                        "qt-faststart, ffmpeg "
#ifdef Q_WS_WIN
                        "with --enabled-avisynth and "
#else
                        "with --enable-avfilter and "
#endif
                        "included to PATH\n"
#if 0 //defined( Q_WS_LINUX ) || defined(Q_WS_MAC)
                        "avs2yuv\nwine\n"
#endif
#ifdef Q_WS_WIN
                        "AviSynth and it's plugin DLL MaskTools.dll\n"
#endif
                        "\nPlease donate to these projects and mine!"
                       );
}

void MainWindow::change( int row, int col)
{
    int nextIdx;
    QString value = moviesTable->item(row,col)->text();
    switch(col) {
    case 0 : // 3D layout
        nextIdx = types->indexOf(value)+1;
        if(nextIdx >= types->count())
            nextIdx = 0;
        moviesTable->setItem(row,col, new QTableWidgetItem(types->at(nextIdx)));
        break;
    case 1 : // aspect ratio
        nextIdx = aspects->indexOf(value)+1;
        if(nextIdx >= aspects->count())
            nextIdx = 0;
        moviesTable->setItem(row,col, new QTableWidgetItem(aspects->at(nextIdx)));
        break;
    case 2 : // frame rate
        nextIdx = rates->indexOf(value)+1;
        if(nextIdx >= rates->count())
            nextIdx = 0;
        moviesTable->setItem(row,col, new QTableWidgetItem(rates->at(nextIdx)));
        break;
    }
}

void MainWindow::clear()
{
    moviesTable->setRowCount(0);
//    emit changed();
}

void MainWindow::updateMoviesTable(const QMimeData *mimeData)
{

    if (!mimeData)
        return;

    foreach (QString format, mimeData->formats()) {
        if (format == "text/uri-list") {
            QList<QUrl> urlList = mimeData->urls();
            for (int i = 0; i < urlList.size(); ++i) {
                QTableWidgetItem *lineItem = new QTableWidgetItem();
                lineItem->setFlags(Qt::ItemIsEnabled);
                lineItem->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);
                int row = moviesTable->rowCount();
                moviesTable->insertRow(row);
                moviesTable->setItem(row, 0, new QTableWidgetItem(types->at(0)));
                moviesTable->setItem(row, 1, new QTableWidgetItem(aspects->at(0)));
                moviesTable->setItem(row, 2, new QTableWidgetItem(rates->at(0)));
                QString fileName = urlList[i].toString().replace("file://","");
#ifdef Q_WS_WIN
                if(fileName.at(2) == QChar(':')) {
                    fileName = fileName.right(fileName.size()-1);
                }
                fileName.replace("/","\\");
#endif
                moviesTable->setItem(row, 3, new QTableWidgetItem(fileName));
            }
        }

    }
    
    moviesTable->resizeColumnToContents(0);
    moviesTable->resizeColumnToContents(1);
    moviesTable->resizeColumnToContents(2);
}

void MainWindow::create3dtv( QTextStream *s, int index )
{
    QString fps = moviesTable->item(index,2)->text();
    QString oldName = moviesTable->item(index,3)->text().replace(QString("\\"),QString("/"));
    QFileInfo movie(oldName);
    QString newName = movie.absolutePath() + QString("/") + movie.baseName() + "_sbs";

    QString script(8000);
    script = "# This is processed on the fly by the DirectShow pluging AviSynth\r\n";
    script += "in = DirectShowSource(\"" + oldName +"\"";
    if (!fps.isEmpty() && (fps != tr("keep"))) script += ", fps="+fps;
    script += ")\r\n";
    if( moviesTable->item(index,1)->text()==tr("costum") ) {
        script += "# Please manually change 1920(width), 540(height)\r\n";
        script += "a = BilinearResize(in, 1920, 540, 0, 0, in.width/2, in.height).AddBorders(0, 0, 0, 0, color_black )\r\n";
        script += "b = BilinearResize(in, 1920, 540, in.width/2, 0, in.width/2, in.height).AddBorders(0, 0, 0, 0, color_black )\r\n";
     } else if( moviesTable->item(index,1)->text()==tr("16:9") ) {
        if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
         || (moviesTable->item(index,0)->text()==tr("SBS LR")) ){
            script += "a = BilinearResize(in, 1920, 540, 0, 0, in.width/2, in.height)\r\n";
            script += "b = BilinearResize(in, 1920, 540, in.width/2, 0, in.width/2, in.height)\r\n";
        } else {
            script += "a = BilinearResize(in, 1920, 540, 0, 0, in.width, in.height/2)\r\n";
            script += "b = BilinearResize(in, 1920, 540, 0, in.height/2, in.width, in.height/2)\r\n";
        }
    } else { // 4:3
        if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
         || (moviesTable->item(index,0)->text()==tr("SBS LR")) ){
            script += "a = BilinearResize(in, 1440, 540, 0, 0, in.width/2, in.height).AddBorders(240, 0, 240, 0, color_black )\r\n";
            script += "b = BilinearResize(in, 1440, 540, in.width/2, 0, in.width/2, in.height).AddBorders(240, 0, 240, 0, color_black )\r\n";
        } else {
            script += "a = BilinearResize(in, 1440, 540, 0, 0, in.width, in.height/2).AddBorders(240, 0, 240, 0, color_black )\r\n";
            script += "b = BilinearResize(in, 1440, 540, 0, in.height/2, in.width, in.height/2).AddBorders(240, 0, 240, 0, color_black )\r\n";
        }
    }
    if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
     || (moviesTable->item(index,0)->text()==tr("OU RL")) ){
        script += "StackVertical(a, b)\r\n";
    } else {
        script += "StackVertical(b,a)\r\n";
    }

    // output avs file
    QString avsName = newName + ".avs";
    QFile avsFile(avsName);
    avsFile.open(QIODevice::WriteOnly);
    QTextStream avsStream(&avsFile);
    avsStream << script;
    avsFile.close();

    // output conversion batch file
    QString mp4 = newName + ".mp4";
#ifdef Q_WS_WIN
    QString batName = newName + ".bat";
    QFile batFile(batName);
    batFile.open(QIODevice::WriteOnly);
    QTextStream batStream(&batFile);
    QString line;
    line = "ffmpeg -i \""+avsName+"\" -b:v 8500K -y";
    if(fps != tr("keep"))
        line += " -r " + fps;
    line += " \""+mp4+"\"\r\n";
    batStream << line;
    *s << line;
    batFile.close();
#endif
#ifdef Q_WS_LINUX
#endif

}

void MainWindow::create3df33d( QTextStream *s, int index )
{
    QString fps = moviesTable->item(index,2)->text();
    QString oldName = moviesTable->item(index,3)->text().replace(QString("\\"),QString("/"));
    QFileInfo movie(oldName);
    QString newName = movie.absolutePath() + QString("/") + movie.baseName() + "_3df33d";

    QString script(8000);
    script = "# This is processed on the fly by the DirectShow pluging AviSynth\r\n";
    script += "in = DirectShowSource(\"" + oldName +"\"";
    if (!fps.isEmpty() && (fps != tr("keep"))) script += ", fps="+fps;
    script += ")\r\n";
    if( moviesTable->item(index,1)->text()==tr("costum") ) {
        script += "# Please manually change 1280(width), 360(height)\r\n";
        script += "a = BilinearResize(in, 1280, 360, 0, 0, in.width/2, in.height).AddBorders(0, 0, 0, 0, color_black )\r\n";
        script += "b = BilinearResize(in, 1280, 360, in.width/2, 0, in.width/2, in.height).AddBorders(0, 0, 0, 0, color_black )\r\n";
     } else if( moviesTable->item(index,1)->text()==tr("16:9") ) {
        if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
         || (moviesTable->item(index,0)->text()==tr("SBS LR")) ){
            script += "a = BilinearResize(in, 1280, 360, 0, 0, in.width/2, in.height)\r\n";
            script += "b = BilinearResize(in, 1280, 360, in.width/2, 0, in.width/2, in.height)\r\n";
        } else {
            script += "a = BilinearResize(in, 1280, 360, 0, 0, in.width, in.height/2)\r\n";
            script += "b = BilinearResize(in, 1280, 360, 0, in.height/2, in.width, in.height/2)\r\n";
        }
    } else { // 4:3
        if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
         || (moviesTable->item(index,0)->text()==tr("SBS LR")) ){
            script += "a = BilinearResize(in, 640, 240, 0, 0, in.width/2, in.height)\r\n";
            script += "b = BilinearResize(in, 640, 240, in.width/2, 0, in.width/2, in.height)\r\n";
        } else {
            script += "a = BilinearResize(in, 640, 240, 0, 0, in.width, in.height/2)\r\n";
            script += "b = BilinearResize(in, 640, 240, 0, in.height/2, in.width, in.height/2)\r\n";
        }
    }
    if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
     || (moviesTable->item(index,0)->text()==tr("OU RL")) ){
        script += "StackVertical(a, b)\r\n";
    } else {
        script += "StackVertical(b,a)\r\n";
    }

    // output avs file
    QString avsName = newName + ".avs";
    QFile avsFile(avsName);
    avsFile.open(QIODevice::WriteOnly);
    QTextStream avsStream(&avsFile);
    avsStream << script;
    avsFile.close();

    // output conversion batch file
    QString mp4 = newName + ".mp4";
#ifdef Q_WS_WIN
    QString batName = newName + ".bat";
    QFile batFile(batName);
    batFile.open(QIODevice::WriteOnly);
    QTextStream batStream(&batFile);
    QString line;
    line = "ffmpeg -i \""+avsName+"\" -b:v 8500K -y";
    if(fps != tr("keep"))
        line += " -r " + fps;
    line += " \""+mp4+"\"\r\n";
    batStream << line;
    *s << line;
    batFile.close();
#endif
#ifdef Q_WS_LINUX
#endif

}

void MainWindow::createKmq( QTextStream *s, int index, bool turned )
{
    QString fps = moviesTable->item(index,2)->text();
    QString oldName = moviesTable->item(index,3)->text().replace(QString("\\"),QString("/"));
    QFileInfo movie(oldName);
    QString newName = movie.absolutePath() + QString("/") + movie.baseName() + ((turned) ? "_Air3D" : "_iPad3D");

    QString script(8000);
    script = "# This is processed on the fly by the DirectShow pluging AviSynth\r\n";
    script += "in = DirectShowSource(\"" + oldName +"\"";
    if (!fps.isEmpty() && (fps != tr("keep"))) script += ", fps="+fps;
    script += ")\r\n";
    if( moviesTable->item(index,1)->text()==tr("costum") ) {
        script += "# Please manually change 768(width), 432(height) and 0/40(borders) so that a full 768x1024 image results\r\n";
        script += "left = BilinearResize(in, 768, 432, 0, 0, in.width/2, in.height).AddBorders(0, 40, 0, 40, color_black )\r\n";
        script += "right = BilinearResize(in, 768, 432, in.width/2, 0, in.width/2, in.height).AddBorders(0, 40, 0, 40, color_black )\r\n";
     } else if( moviesTable->item(index,1)->text()==tr("16:9") ) {
        if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
         || (moviesTable->item(index,0)->text()==tr("SBS LR")) ){
            script += "left = BilinearResize(in, 768, 432, 0, 0, in.width/2, in.height).AddBorders(0, 40, 0, 40, color_black )\r\n";
            script += "right = BilinearResize(in, 768, 432, in.width/2, 0, in.width/2, in.height).AddBorders(0, 40, 0, 40, color_black )\r\n";
        } else {
            script += "left = BilinearResize(in, 768, 432, 0, 0, in.width, in.height/2).AddBorders(0, 40, 0, 40, color_black )\r\n";
            script += "right = BilinearResize(in, 768, 432, 0, in.height/2, in.width, in.height/2).AddBorders(0, 40, 0, 40, color_black )\r\n";
        }
    } else { // 4:3
        if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
         || (moviesTable->item(index,0)->text()==tr("SBS LR")) ){
            script += "left = BilinearResize(in, 666, 500, 0, 0, in.width/2, in.height).AddBorders(50, 6, 50, 6, color_black )\r\n";
            script += "right = BilinearResize(in, 666, 500, in.width/2, 0, in.width/2, in.height).AddBorders(50, 6, 50, 6, color_black )\r\n";
        } else {
            script += "left = BilinearResize(in, 666, 500, 0, 0, in.width, in.height/2).AddBorders(50, 6, 50, 6, color_black )\r\n";
            script += "right = BilinearResize(in, 666, 500, 0, in.height/2, in.width, in.height/2).AddBorders(50, 6, 50, 6, color_black )\r\n";
        }
    }
    if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
     || (moviesTable->item(index,0)->text()==tr("OU RL")) ){
        script += "StackVertical(left, right)\r\n";
    } else {
        script += "StackVertical(right,left)\r\n";
    }

    // Add title on first frame
    script += "Subtitle(\"3-D\", 384, 190, 0, 0, \"Arial\", 180, $FFFFFF, 0, 5, 0)\r\n";

    // try to split file name into 1 or 3 lines
    QString title = movie.baseName();
    QStringList parts = title.split(" ",QString::SkipEmptyParts);
    switch(parts.count()) {
    case 2:
        script += "Subtitle(\""+parts.at(0)+"\", 384, 380, 0, 0, \"Arial\", 60, $FFFF00, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(1)+"\", 384, 520, 0, 0, \"Arial\", 50, $FFFFFF, 0, 5, 0)\r\n";
        break;
    case 3 :
        script += "Subtitle(\""+parts.at(0)+"\", 384, 320, 0, 0, \"Arial\", 60, $FFFF00, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(1)+"\", 384, 445, 0, 0, \"Arial\", 50, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(2)+"\", 384, 510, 0, 0, \"Arial\", 50, $FFFFFF, 0, 5, 0)\r\n";
        break;
    case 4 :
        script += "Subtitle(\""+parts.at(0)+"\", 384, 320, 0, 0, \"Arial\", 60, $FFFF00, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(1)+"\", 384, 445, 0, 0, \"Arial\", 50, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(2)+"\", 384, 510, 0, 0, \"Arial\", 50, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(3)+"\", 384, 575, 0, 0, \"Arial\", 50, $FFFFFF, 0, 5, 0)\r\n";
        break;
    case 1:
        if (title.length()<20) {
            script += "Subtitle(\""+title.replace("_"," ")+"\", 384, 445, 0, 0, \"Arial\", 60, $FFFF00, 0, 5, 0)\r\n";
            break;
        } // else fall thru to default
    default:
        script += "# The suggested title is : " + title + "\r\n";
        script += "Subtitle(\"Please manually\", 384, 320, 0, 0, \"Arial\", 60, $FFFF00, 0, 5, 0)\r\n";
        script += "Subtitle(\"edit these lines\", 384, 445, 0, 0, \"Arial\", 50, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\"in the avs file!\", 384, 510, 0, 0, \"Arial\", 50, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\"\", 384, 510, 0, 0, \"Arial\", 50, $FFFFFF, 0, 5, 0)\r\n";
        break;
    }

/*
Subtitle("3-D", 384, 190, 0, 0, "Arial", 180, $FFFFFF, 0, 5, 0)
Subtitle("'A Railway Bridge Moves'", 384, 320, 0, 0, "Arial", 60, $FFFF00, 0, 5, 0)
Subtitle("A docu slide show", 384, 445, 0, 0, "Arial", 50, $FFFFFF, 0, 5, 0)
Subtitle("made with", 384, 510, 0, 0, "Arial", 50, $FFFFFF, 0, 5, 0)
Subtitle("StereoBookMaker", 384, 575, 0, 0, "Arial", 50, $FFFFFF, 0, 5, 0)
Subtitle("More about this 3-D method on", 384, 700, 0, 0, "Arial", 50, $FFFFFF, 0, 5, 0)
Subtitle("http://openKMQ.blogspot.com", 384, 760, 0, 0, "Arial", 50, $FFFFFF, 0, 5, 0)
 */

    script += "Subtitle(\"More about this 3-D method on\", 384, 700, 0, 0, \"Arial\", 50, $FFFFFF, 0, 5, 0)\r\n";
    script += "Subtitle(\"http://openKMQ.blogspot.com\", 384, 760, 0, 0, \"Arial\", 50, $FFFFFF, 0, 5, 0)\r\n";
    if(turned) script += "TurnRight\r\n";
    //script += "#ImageWriter(\"C:\\\", 100, 100, \"PNG\")\r\n";

    // output avs file
    QString avsName = newName + ".avs";
    QFile avsFile(avsName);
    avsFile.open(QIODevice::WriteOnly);
    QTextStream avsStream(&avsFile);
    avsStream << script;
    avsFile.close();

    // output conversion batch file
    QString mp4 = newName + ".mp4";
#ifdef Q_WS_WIN
    QString batName = newName + ".bat";
    QFile batFile(batName);
    batFile.open(QIODevice::WriteOnly);
    QTextStream batStream(&batFile);
    QString line;
    line = "ffmpeg -i \""+avsName+"\" -b:v 8500K -y";
    if(fps != tr("keep"))
        line += " -r " + fps;
    line += " \""+mp4+"\"\r\n";
    batStream << line;
    *s << line;
    batStream << "ECHO If you want to transfer the movie to an iPad\r\n";
    batStream << "ECHO using the Camera Connection Kit, please manually\r\n";
    batStream << "ECHO rename the movie file to an XXXXXXXX.MP4 name (exact 8.3 capital chars!)\r\n";
    batStream << "PAUSE Press any Key\r\n";
    batFile.close();
#endif
#ifdef Q_WS_LINUX
#endif

}

void MainWindow::createKMQiPad( QTextStream *s, int index, bool turned )
{
    QString fps = moviesTable->item(index,2)->text();
    QString oldName = moviesTable->item(index,3)->text().replace(QString("\\"),QString("/"));
    QFileInfo movie(oldName);
    QString newName = movie.absolutePath() + QString("/") + movie.baseName() + "_KMQiPad" + ((turned) ? "L" : "P");

    // output conversion batch file
    QString mp4 = newName + ".mp4";
    QString temp = newName + ".tmp";
#ifdef Q_WS_WIN
    QString batName = newName + ".bat";
#else
    QString batName = newName + ".sh";
#endif
    QFile batFile(batName);
    batFile.open(QIODevice::WriteOnly);
    QString line;
    QTextStream script(&batFile);

    line = "ffmpeg -y -i \"" + oldName + "\"";
    if(fps != tr("keep"))
        line += " -r " + fps.toAscii();
    line += QString(" -b:v 8500K -aspect %1 -vf \"[in]").arg((turned) ? "4:3" : "3:4");
    line += "select=1,";    //line += "select='gte(t," + str(startSec) + ")*lte(t," + str(endSec) + ")',";
    line += "crop='";
    if(moviesTable->item(index,0)->text()==tr("SBS RL"))
        line += "iw/2:ih:iw/2:0";
    else if (moviesTable->item(index,0)->text()==tr("SBS LR"))
        line += "iw/2:ih:0:0";
    else if (moviesTable->item(index,0)->text()==tr("OU LR"))
        line += "iw:ih/2:0:0";
    else if (moviesTable->item(index,0)->text()==tr("OU RL"))
        line += "iw:ih/2:0:ih/2";

    int h, w;
    if( moviesTable->item(index,1)->text()==tr("16:9") ) {
        w = 768;
        h = w*9/16;
    } else {
        w = 666;
        h = w*3/4;
    }
    int vgap = (512-h)/2;
    line += QString("',scale=%1:%2[A];").arg(w).arg(h);
#ifdef Q_WS_WIN
    line += "movie=" + oldName.right(oldName.length()-2); // chop off disk name C: or so until
#else
    line += "movie=" + oldName;
#endif
    line += ",select=1";    //line += ",select='gte(t," + str(startSec) + ")*lte(t," + str(endSec) + ")'";
    line += ",crop='";
    if(moviesTable->item(index,0)->text()==tr("SBS RL"))
        line += "iw/2:ih:0:0";
    else if (moviesTable->item(index,0)->text()==tr("SBS LR"))
        line += "iw/2:ih:iw/2:0";
    else if (moviesTable->item(index,0)->text()==tr("OU LR"))
        line += "iw:ih/2:0:ih/2";
    else if (moviesTable->item(index,0)->text()==tr("OU RL"))
        line += "iw:ih/2:0:0";
    line += QString("',scale=%1:%2,").arg(w).arg(h);
    line += QString("pad=%1:1024:0:%2:black[B];").arg(w).arg(vgap);
    line += QString("[B][A]overlay=0:%1").arg(vgap+512);
    if (turned)
        line += ",transpose=1";
    line += "\""; // end of vf

    line += " -f mp4 \""+temp+"\"\nqt-faststart \""+temp+"\" \""+mp4+"\"\n";
#ifdef Q_WS_WIN
    line += "del \""+temp+"\"\n";
#else
    line += "rm \""+temp+"\"\n";
#endif
#ifndef Q_WS_WIN
    script << "#!/usr/bin/sh\r\n";
#endif
    script << line;
    *s << line;
    batFile.close();

    QByteArray ba = batName.toLocal8Bit();
    const char *c_batName = ba.data();
#ifdef Q_WS_WIN
    _chmod(c_batName, _S_IREAD | _S_IWRITE | _S_IEXEC );
#else
    chmod(c_batName, 0755);
#endif

}

void MainWindow::createKMQ1080p( QTextStream *s, int index)
{
    QString fps = moviesTable->item(index,2)->text();
    QString oldName = moviesTable->item(index,3)->text().replace(QString("\\"),QString("/"));
    QFileInfo movie(oldName);
    QString newName = movie.absolutePath() + QString("/") + movie.baseName() + "_KMQ1080p";

    // output conversion batch file
    QString mp4 = newName + ".mp4";
    QString temp = newName + ".tmp";
#ifdef Q_WS_WIN
    QString batName = newName + ".bat";
#else
    QString batName = newName + ".sh";
#endif
    QFile batFile(batName);
    batFile.open(QIODevice::WriteOnly);
    QString line;
    QTextStream script(&batFile);

    line = "ffmpeg -y -i \"" + oldName + "\"";
    if(fps != tr("keep"))
        line += QString(" -r %1").arg(fps);
    line += " -b:v 8500K -aspect 3:4 -vf \"[in]";
    /**/QString startSec = QString("20");
    /**/QString endSec = QString("30");

    line += "select=1,";    //line += "select='gte(t," + startSec + ")*lte(t," + endSec + ")',";
    line += "crop='";
    if(moviesTable->item(index,0)->text()==tr("SBS RL"))
        line += "iw/2:ih:iw/2:0";
    else if (moviesTable->item(index,0)->text()==tr("SBS LR"))
        line += "iw/2:ih:0:0";
    else if (moviesTable->item(index,0)->text()==tr("OU LR"))
        line += "iw:ih/2:0:0";
    else if (moviesTable->item(index,0)->text()==tr("OU RL"))
        line += "iw:ih/2:0:ih/2";

    int h, w;
    if( moviesTable->item(index,1)->text()==tr("16:9") ) {
        w = 810;
        h = w*9/16;
    } else {
        w = 720;
        h = w*3/4;
    }
    int vgap = (540-h)/2;
    line += QString("',scale=%1:%2[A];").arg(w).arg(h);
#ifdef Q_WS_WIN
    line += "movie=" + oldName.right(oldName.length()-2); // chop off disk name C: or so until
#else
    line += "movie=" + oldName;
#endif
    line += ",select=1";    //line += ",select='gte(t," + startSec + ")*lte(t," + endSec + ")'";
    line += ",crop='";
    if(moviesTable->item(index,0)->text()==tr("SBS RL"))
        line += "iw/2:ih:0:0";
    else if (moviesTable->item(index,0)->text()==tr("SBS LR"))
        line += "iw/2:ih:iw/2:0";
    else if (moviesTable->item(index,0)->text()==tr("OU LR"))
        line += "iw:ih/2:0:ih/2";
    else if (moviesTable->item(index,0)->text()==tr("OU RL"))
        line += "iw:ih/2:0:0";
    line += QString("',scale=%1:%2,").arg(w).arg(h);
    line += QString("pad=%1:%2:%3:%4:black[B];").arg(w).arg(1080).arg(0).arg(vgap);
    line += QString("[B][A]overlay=0:%1").arg(vgap+540);
    line += "\""; // end of vf

    line += " -f mp4 \""+temp+"\"\nqt-faststart \""+temp+"\" \""+mp4+"\"\n";
#ifdef Q_WS_WIN
    line += "del \""+temp+"\"\n";
#else
    line += "rm \""+temp+"\"\n";
#endif
#ifndef Q_WS_WIN
    script << "#!/usr/bin/sh\r\n";
#endif
    script << line;
    *s << line;
    batFile.close();

    QByteArray ba = batName.toLocal8Bit();
    const char *c_batName = ba.data();
#ifdef Q_WS_WIN
    _chmod(c_batName, _S_IREAD | _S_IWRITE | _S_IEXEC );
#else
    chmod(c_batName, 0755);
#endif

}

void MainWindow::createKMQ1280( QTextStream *s, int index )
{
    QString fps = moviesTable->item(index,2)->text();
    QString oldName = moviesTable->item(index,3)->text().replace(QString("\\"),QString("/"));
    QFileInfo movie(oldName);
    QString newName = movie.absolutePath() + QString("/") + movie.baseName() + "_KMQ1280";

    // output conversion batch file
    QString mp4 = newName + ".mp4";
    QString temp = newName + ".tmp";
#ifdef Q_WS_WIN
    QString batName = newName + ".bat";
#else
    QString batName = newName + ".sh";
#endif
    QFile batFile(batName);
    QString line;
    batFile.open(QIODevice::WriteOnly);
    QTextStream script(&batFile);

    int h, ht=1280, w, vgap;
    if( moviesTable->item(index,1)->text()==tr("16:9") ) {
        h = (ht - 104)/2;
        w = 1040;
     } else {
        h = (ht - 40)/2;
        w = h *  4 / 3;
    }
    vgap = (ht-(2*h));

    line = QString("ffmpeg -y -i \"%1\"").arg(oldName);
    if(fps != tr("keep"))
        line += QString(" -r %1").arg(fps);
    line += QString(" -b:v 8500K -aspect %1:%2 -vf \"[in]").arg(ht).arg(w);
    line += "select=1,";    //line += "select='gte(t," + startSec + ")*lte(t," + endSec + ")',";
    line += "crop='";
    if(moviesTable->item(index,0)->text()==tr("SBS LR"))
        line += "iw/2:ih:iw/2:0";
    else if (moviesTable->item(index,0)->text()==tr("SBS RL"))
        line += "iw/2:ih:0:0";
    else if (moviesTable->item(index,0)->text()==tr("OU LR"))
        line += "iw:ih/2:0:0";
    else if (moviesTable->item(index,0)->text()==tr("OU RL"))
        line += "iw:ih/2:0:ih/2";

    line += QString("',scale=%1:%2[A];").arg(w).arg(h);
    line += QString("movie=%1").arg(oldName);
    line += ",select=1";    //line += ",select='gte(t," + startSec + ")*lte(t," + endSec + ")'";
    line += ",crop='";
    if(moviesTable->item(index,0)->text()==tr("SBS LR"))
        line += "iw/2:ih:0:0";
    else if (moviesTable->item(index,0)->text()==tr("SBS RL"))
        line += "iw/2:ih:iw/2:0";
    else if (moviesTable->item(index,0)->text()==tr("OU LR"))
        line += "iw:ih/2:0:ih/2";
    else if (moviesTable->item(index,0)->text()==tr("OU RL"))
        line += "iw:ih/2:0:0";
    line += QString("',scale=%1:%2,").arg(w).arg(h);
    line += QString("pad=%1:%2:0:0:black[B];").arg(w).arg(ht);
    line += QString("[B][A]overlay=0:%1,transpose=1").arg(vgap+h);
    line += "\""; // end of vf

    line += " -f mp4 \""+temp+"\"\nqt-faststart \""+temp+"\" \""+mp4+"\"\n";
#ifdef Q_WS_WIN
    line += "del \""+temp+"\"\n";
#else
    line += "rm \""+temp+"\"\n";
#endif
#ifndef Q_WS_WIN
    script << "#!/usr/bin/sh\r\n";
#endif
    script << line;
    *s << line;
    batFile.close();

    QByteArray ba = batName.toLocal8Bit();
    const char *c_batName = ba.data();
#ifdef Q_WS_WIN
    _chmod(c_batName, _S_IREAD | _S_IWRITE | _S_IEXEC );
#else
    chmod(c_batName, 0755);
#endif

}

void MainWindow::createMy3D960avs( QTextStream *s, int index)
{
    QString fps = moviesTable->item(index,2)->text();
    QString oldName = moviesTable->item(index,3)->text().replace(QString("\\"),QString("/"));
    QFileInfo movie(oldName);
    QString newName = movie.absolutePath() + QString("/") + movie.baseName() + "_my3D960";

    QString script(8000);
    script = "# This is processed on the fly by the DirectShow pluging AviSynth\r\n";
    script += "in = DirectShowSource(\"" + oldName +"\"";
    if (!fps.isEmpty() && (fps != tr("keep"))) script += ", fps="+fps;
    script += ")\r\n";
    if( moviesTable->item(index,1)->text()==tr("costum") ) {
        script += "# Please manually change 436(width), 242(height) and borders so that a full 960x242 image results\r\n";
        script += "left = BilinearResize(in, 436, 242, 0, 0, in.width/2, in.height).AddBorders(8, 0, 24, 0, color_black )\r\n";
        script += "right = BilinearResize(in, 436, 242, in.width/2, 0, in.width/2, in.height).AddBorders(0, 0, 56, 0, color_black )\r\n";
     } else if( moviesTable->item(index,1)->text()==tr("16:9") ) {
        if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
         || (moviesTable->item(index,0)->text()==tr("SBS LR")) ){
            script += "left = BilinearResize(in, 436, 242, 0, 0, in.width/2, in.height).AddBorders(8, 0, 24, 0, color_black )\r\n";
            script += "right = BilinearResize(in, 436, 242, in.width/2, 0, in.width/2, in.height).AddBorders(0, 0, 56, 0, color_black )\r\n";
        } else {
            script += "left = BilinearResize(in, 436, 242, 0, 0, in.width, in.height/2).AddBorders(8, 0, 24, 0, color_black )\r\n";
            script += "right = BilinearResize(in, 436, 242, 0, in.height/2, in.width, in.height/2).AddBorders(0, 0, 56, 0, color_black )\r\n";
        }
    } else { // 4:3
        if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
         || (moviesTable->item(index,0)->text()==tr("SBS LR")) ){
            script += "left = BilinearResize(in, 436, 328, 0, 0, in.width/2, in.height).AddBorders(0, 0, 56, 0, color_black )\r\n";
            script += "right = BilinearResize(in, 436, 328, in.width/2, 0, in.width/2, in.height).AddBorders(8, 0, 24, 0, color_black )\r\n";
        } else {
            script += "left = BilinearResize(in, 436, 328, 0, 0, in.width, in.height/2).AddBorders(0, 0, 56, 0, color_black )\r\n";
            script += "right = BilinearResize(in, 436, 328, 0, in.height/2, in.width, in.height/2).AddBorders(8, 0, 24, 0, color_black )\r\n";
        }
    }
    if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
     || (moviesTable->item(index,0)->text()==tr("OU RL")) ){
        script += "StackHorizontal(right,left)\r\n";
    } else {
        script += "StackHorizontal(left,right)\r\n";
    }

    // Add title on first frame
    script += "Subtitle(\"trueColor\", 170, 50, 0, 0, \"Arial\", 60, $FF3322, 0, 5, 0)\r\n";
    script += "Subtitle(\"3-D\", 170, 180, 0, 0, \"Arial\", 170, $FF3322, 0, 5, 0)\r\n";
    script += "Subtitle(\"use with Retina display\", 170, 220, 0, 0, \"Arial\", 24, $FFFF00, 0, 5, 0)\r\n";

    // try to split file name into 1 or 3 lines
    QString title = movie.baseName();
    QStringList parts = title.split(" ",QString::SkipEmptyParts);
    switch(parts.count()) {
    case 2:
        script += "Subtitle(\""+parts.at(0)+"\", 600, 120, 0, 0, \"Arial\", 70, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(1)+"\", 600, 180, 0, 0, \"Arial\", 70, $FFFFFF, 0, 5, 0)\r\n";
        break;
    case 3 :
        script += "Subtitle(\""+parts.at(0)+"\", 600,  80, 0, 0, \"Arial\", 60, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(1)+"\", 600, 145, 0, 0, \"Arial\", 60, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(2)+"\", 600, 210, 0, 0, \"Arial\", 60, $FFFFFF, 0, 5, 0)\r\n";
        break;
    case 4 :
        script += "Subtitle(\""+parts.at(0)+"\", 600,  70, 0, 0, \"Arial\", 55, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(1)+"\", 600, 120, 0, 0, \"Arial\", 55, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(2)+"\", 600, 170, 0, 0, \"Arial\", 55, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(3)+"\", 600, 220, 0, 0, \"Arial\", 55, $FFFFFF, 0, 5, 0)\r\n";
        break;
    case 1:
        if (title.length()<20) {
            script += "Subtitle(\""+title.replace("_"," ")+"\", 600, 150, 0, 0, \"Arial\", 60, $FFFFFF, 0, 5, 0)\r\n";
            break;
        } // else fall thru to default
    default:
        script += "# The suggested title is : " + title + "\r\n";
        script += "Subtitle(\"Please manually\",  600,  70, 0, 0, \"Arial\", 55, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\"edit these lines\", 600, 120, 0, 0, \"Arial\", 55, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\"in the avs file!\", 600, 170, 0, 0, \"Arial\", 55, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\"\", 600, 220, 0, 0, \"Arial\", 55, $FFFFFF, 0, 5, 0)\r\n";
        break;
    }

    /*
    script += "Subtitle(\"More about this 3-D method on\", 600, 240, 0, 0, \"Arial\", 50, $FFFFFF, 0, 5, 0)\r\n";
    script += "Subtitle(\"http://openMy3D.blogspot.com\",  600, 270, 0, 0, \"Arial\", 50, $FFFFFF, 0, 5, 0)\r\n";
    */
    //script += "#ImageWriter(\"C:\\\", 100, 100, \"PNG\")\r\n";

    // output avs file
    QString avsName = newName + ".avs";
    QFile avsFile(avsName);
    avsFile.open(QIODevice::WriteOnly);
    QTextStream avsStream(&avsFile);
    avsStream << script;
    avsFile.close();

    // output conversion batch file
    QString mp4 = newName + ".mp4";
#ifdef Q_WS_WIN
    QString batName = newName + ".bat";
    QFile batFile(batName);
    batFile.open(QIODevice::WriteOnly);
    QTextStream batStream(&batFile);
    QString line;
    line = "ffmpeg -i \""+avsName+"\" -b:v 4500K -y";
    if(fps != tr("keep"))
        line += " -r " + fps;
    line += " \""+mp4+"\"\r\n";
    batStream << line;
    *s << line;
    batFile.close();
#endif
#ifdef Q_WS_LINUX
#endif

}

void MainWindow::createMy3D480p( QTextStream *s, int index)
{
    QString fps = moviesTable->item(index,2)->text();
    QString oldName = moviesTable->item(index,3)->text().replace(QString("\\"),QString("/"));
    QFileInfo movie(oldName);
    QString newName = movie.absolutePath() + QString("/") + movie.baseName() + "_my3D480p";

    // output conversion batch file
    QString mp4 = newName + ".mp4";
    QString temp = newName + ".tmp";
#ifdef Q_WS_WIN
    QString batName = newName + ".bat";
#else
    QString batName = newName + ".sh";
#endif
    QFile batFile(batName);
    batFile.open(QIODevice::WriteOnly);
    QString line;
    QTextStream script(&batFile);

    line = QString("ffmpeg -y -i \"%1\"").arg(oldName);
    if(fps != tr("keep"))
        line += QString(" -r %1").arg(fps);
    line += " -b:v 4500K -aspect 2.66667 -vf \"[in]";
    line += "select=1,";    //line += "select='gte(t," + startSec + ")*lte(t," + endSec + ")',";
    line += "crop='";
    if(moviesTable->item(index,0)->text()==tr("SBS RL"))
        line += "iw/2:ih:iw/2:0";
    else if (moviesTable->item(index,0)->text()==tr("SBS LR"))
        line += "iw/2:ih:0:0";
    else if (moviesTable->item(index,0)->text()==tr("OU LR"))
        line += "iw:ih/2:0:ih/2";
    else if (moviesTable->item(index,0)->text()==tr("OU RL"))
        line += "iw:ih/2:0:0";

    int h, w1 = 582;
    if( moviesTable->item(index,1)->text()==tr("16:9") )
        h = w1*9/16;
    else
        h = w1*3/4;

    int vgap = (480-h)/2;
    line += QString("',scale=%1:%2[A];").arg(w1).arg(h);
#ifdef Q_WS_WIN
    line += "movie=" + oldName.right(oldName.length()-2); // chop off disk name C: or so until
#else
    line += "movie=" + oldName;
#endif
    line += ",select=1";    //line += ",select='gte(t," + startSec + ")*lte(t," + endSec + ")'";
    line += ",crop='";
    if(moviesTable->item(index,0)->text()==tr("SBS RL"))
        line += "iw/2:ih:0:0";
    else if (moviesTable->item(index,0)->text()==tr("SBS LR"))
        line += "iw/2:ih:iw/2:0";
    else if (moviesTable->item(index,0)->text()==tr("OU LR"))
        line += "iw:ih/2:0:0";
    else if (moviesTable->item(index,0)->text()==tr("OU RL"))
        line += "iw:ih/2:0:ih/2";
    line += QString("',scale=%1:%2,").arg(w1).arg(h);
    line += QString("pad=1280:480:10:%1:black[B];").arg(vgap);
    line += QString("[B][A]overlay=624:%1").arg(vgap);
    line += "\""; // end of vf

    line += " -f mp4 \""+temp+"\"\nqt-faststart \""+temp+"\" \""+mp4+"\"\n";
#ifdef Q_WS_WIN
    line += "del \""+temp+"\"\n";
#else
    line += "rm \""+temp+"\"\n";
#endif
#ifndef Q_WS_WIN
    script << "#!/usr/bin/sh\r\n";
#endif
    script << line;
    *s << line;
    batFile.close();

    QByteArray ba = batName.toLocal8Bit();
    const char *c_batName = ba.data();
#ifdef Q_WS_WIN
    _chmod(c_batName, _S_IREAD | _S_IWRITE | _S_IEXEC );
#else
    chmod(c_batName, 0755);
#endif

}

void MainWindow::createMy3D960( QTextStream *s, int index)
{
    QString fps = moviesTable->item(index,2)->text();
    QString oldName = moviesTable->item(index,3)->text().replace(QString("\\"),QString("/"));
    QFileInfo movie(oldName);
    QString newName = movie.absolutePath() + QString("/") + movie.baseName() + "_my3D960";

    // output conversion batch file
    QString mp4 = newName + ".mp4";
    QString temp = newName + ".tmp";
#ifdef Q_WS_WIN
    QString batName = newName + ".bat";
#else
    QString batName = newName + ".sh";
#endif
    QFile batFile(batName);
    batFile.open(QIODevice::WriteOnly);
    QString line;
    QTextStream script(&batFile);

    int h, w1 = 436;
    if( moviesTable->item(index,1)->text()==tr("16:9") ) {
        h = w1*9/16;
    } else {
        h = w1*3/4;
    }
    h &= 0xFFFFFFFE; // round down

    line = QString("ffmpeg -y -i \"%1\"").arg(oldName);
    if(fps != tr("keep"))
        line += QString(" -r %1").arg(fps);
    line += QString(" -b:v 4500K -aspect 960:%1 -vf \"[in]").arg(h);
    line += "select=1,";    //line += "select='gte(t," + startSec + ")*lte(t," + endSec + ")',";
    line += "crop='";
    if(moviesTable->item(index,0)->text()==tr("SBS LR"))
        line += "iw/2:ih:iw/2:0";
    else if (moviesTable->item(index,0)->text()==tr("SBS RL"))
        line += "iw/2:ih:0:0";
    else if (moviesTable->item(index,0)->text()==tr("OU RL"))
        line += "iw:ih/2:0:0";
    else if (moviesTable->item(index,0)->text()==tr("OU LR"))
        line += "iw:ih/2:0:ih/2";

    line += QString("',scale=%1:%2[A];").arg(w1).arg(h);
#ifdef Q_WS_WIN
    line += "movie=" + oldName.right(oldName.length()-2); // chop off disk name C: or so until
#else
    line += "movie=" + oldName;
#endif
    line += ",select=1";    //line += ",select='gte(t," + startSec + ")*lte(t," + endSec + ")'";
    line += ",crop='";
    if(moviesTable->item(index,0)->text()==tr("SBS LR"))
        line += "iw/2:ih:0:0";
    else if (moviesTable->item(index,0)->text()==tr("SBS RL"))
        line += "iw/2:ih:iw/2:0";
    else if (moviesTable->item(index,0)->text()==tr("OU RL"))
        line += "iw:ih/2:0:ih/2";
    else if (moviesTable->item(index,0)->text()==tr("OU LR"))
        line += "iw:ih/2:0:0";
    line += QString("',scale=%1:%2,").arg(w1).arg(h);
    line += QString("pad=960:%1:8:0:black[B];").arg(h);
    line += QString("[B][A]overlay=468:0");
    line += "\""; // end of vf

    line += " -f mp4 \""+temp+"\"\nqt-faststart \""+temp+"\" \""+mp4+"\"\n";
#ifdef Q_WS_WIN
    line += "del \""+temp+"\"\n";
#else
    line += "rm \""+temp+"\"\n";
#endif
#ifndef Q_WS_WIN
    script << "#!/usr/bin/sh\r\n";
#endif
    script << line;
    *s << line;
    batFile.close();

    QByteArray ba = batName.toLocal8Bit();
    const char *c_batName = ba.data();
#ifdef Q_WS_WIN
    _chmod(c_batName, _S_IREAD | _S_IWRITE | _S_IEXEC );
#else
    chmod(c_batName, 0755);
#endif

}

void MainWindow::createMy3D720p( QTextStream *s, int index)
{
    QString fps = moviesTable->item(index,2)->text();
    QString oldName = moviesTable->item(index,3)->text().replace(QString("\\"),QString("/"));
    QFileInfo movie(oldName);
    QString newName = movie.absolutePath() + QString("/") + movie.baseName() + "_my3D720p";

    // output conversion batch file
    QString mp4 = newName + ".mp4";
    QString temp = newName + ".tmp";
#ifdef Q_WS_WIN
    QString batName = newName + ".bat";
#else
    QString batName = newName + ".sh";
#endif
    QFile batFile(batName);
    batFile.open(QIODevice::WriteOnly);
    QString line;
    QTextStream script(&batFile);

    line = QString("ffmpeg -y -i \"%1\"").arg(oldName);
    if(fps != tr("keep"))
        line += QString(" -r %1").arg(fps);
    line += " -b:v 8500K -aspect 2.66667 -vf \"[in]";
    line += "select=1,";    //line += "select='gte(t," + startSec + ")*lte(t," + endSec + ")',";
    line += "crop='";
    if(moviesTable->item(index,0)->text()==tr("SBS RL"))
        line += "iw/2:ih:iw/2:0";
    else if (moviesTable->item(index,0)->text()==tr("SBS LR"))
        line += "iw/2:ih:0:0";
    else if (moviesTable->item(index,0)->text()==tr("OU RL"))
        line += "iw:ih/2:0:ih/2";
    else if (moviesTable->item(index,0)->text()==tr("OU LR"))
        line += "iw:ih/2:0:0";

    int h, w1 = 874;
    if( moviesTable->item(index,1)->text()==tr("16:9") )
        h = w1*9/16;
    else
        h = w1*3/4;

    int vgap = (720-h)/2;
    line += QString("',scale=%1:%2,").arg(w1).arg(h);
    line += QString("pad=1920:720:10:%1:black[A];").arg(vgap);
#ifdef Q_WS_WIN
    line += "movie=" + oldName.right(oldName.length()-2); // chop off disk name C: or so until
#else
    line += "movie=" + oldName;
#endif
    line += ",select=1";    //line += ",select='gte(t," + startSec + ")*lte(t," + endSec + ")'";
    line += ",crop='";
    if(moviesTable->item(index,0)->text()==tr("SBS RL"))
        line += "iw/2:ih:0:0";
    else if (moviesTable->item(index,0)->text()==tr("SBS LR"))
        line += "iw/2:ih:iw/2:0";
    else if (moviesTable->item(index,0)->text()==tr("OU RL"))
        line += "iw:ih/2:0:0";
    else if (moviesTable->item(index,0)->text()==tr("OU LR"))
        line += "iw:ih/2:0:ih/2";
    line += QString("',scale=%1:%2[B];").arg(w1).arg(h);
    line += QString("[A][B]overlay=936:%1").arg(vgap);
    line += "\""; // end of vf

    line += " -f mp4 \""+temp+"\"\nqt-faststart \""+temp+"\" \""+mp4+"\"\n";
#ifdef Q_WS_WIN
    line += "del \""+temp+"\"\n";
#else
    line += "rm \""+temp+"\"\n";
#endif
#ifndef Q_WS_WIN
    script << "#!/usr/bin/sh\r\n";
#endif
    script << line;
    *s << line;
    batFile.close();

    QByteArray ba = batName.toLocal8Bit();
    const char *c_batName = ba.data();
#ifdef Q_WS_WIN
    _chmod(c_batName, _S_IREAD | _S_IWRITE | _S_IEXEC );
#else
    chmod(c_batName, 0755);
#endif

}

void MainWindow::createMy3D1280( QTextStream *s, int index)
{
    QString fps = moviesTable->item(index,2)->text();
    QString oldName = moviesTable->item(index,3)->text().replace(QString("\\"),QString("/"));
    QFileInfo movie(oldName);
    QString newName = movie.absolutePath() + QString("/") + movie.baseName() + "_my3D1280";

    QString script(8000);
    script = "# This is processed on the fly by the DirectShow pluging AviSynth\r\n";
    script += "in = DirectShowSource(\"" + oldName +"\"";
    if (!fps.isEmpty() && (fps != tr("keep"))) script += ", fps="+fps;
    script += ")\r\n";
    if( moviesTable->item(index,1)->text()==tr("costum") ) {
        script += "# Please manually change 582(width), 324(height) and borders so that a full 1280x480 image results\r\n";
        script += "left = BilinearResize(in, 582, 324, 0, 0, in.width/2, in.height).AddBorders(10, 78, 32, 78, color_black )\r\n";
        script += "right = BilinearResize(in, 582, 324, in.width/2, 0, in.width/2, in.height).AddBorders(0, 78, 74, 78, color_black )\r\n";
     } else if( moviesTable->item(index,1)->text()==tr("16:9") ) {
        if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
         || (moviesTable->item(index,0)->text()==tr("SBS LR")) ){
            script += "left = BilinearResize(in, 582, 324, 0, 0, in.width/2, in.height).AddBorders(10, 78, 32, 78, color_black )\r\n";
            script += "right = BilinearResize(in, 582, 324, in.width/2, 0, in.width/2, in.height).AddBorders(0, 78, 74, 78, color_black )\r\n";
        } else {
            script += "left = BilinearResize(in, 582, 324, 0, 0, in.width, in.height/2).AddBorders(10, 78, 32, 78, color_black )\r\n";
            script += "right = BilinearResize(in, 582, 324, 0, in.height/2, in.width, in.height/2).AddBorders(0, 78, 74, 78, color_black )\r\n";
        }
    } else { // 4:3
        if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
         || (moviesTable->item(index,0)->text()==tr("SBS LR")) ){
            script += "left = BilinearResize(in, 582, 438, 0, 0, in.width/2, in.height).AddBorders(0, 21, 74, 21, color_black )\r\n";
            script += "right = BilinearResize(in, 582, 438, in.width/2, 0, in.width/2, in.height).AddBorders(10, 0, 32, 0, color_black )\r\n";
        } else {
            script += "left = BilinearResize(in, 582, 438, 0, 0, in.width, in.height/2).AddBorders(0, 21, 74, 21, color_black )\r\n";
            script += "right = BilinearResize(in, 582, 438, 0, in.height/2, in.width, in.height/2).AddBorders(10, 21, 32, 21, color_black )\r\n";
        }
    }
    if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
     || (moviesTable->item(index,0)->text()==tr("OU RL")) ){
        script += "StackHorizontal(right,left)\r\n";
    } else {
        script += "StackHorizontal(left,right)\r\n";
    }

    // Add title on first frame
    script += "Subtitle(\"trueColor\", 226, 147, 0, 0, \"Arial\", 80, $FF3322, 0, 5, 0)\r\n";
    script += "Subtitle(\"3-D\", 226, 320, 0, 0, \"Arial\", 227, $FF3322, 0, 5, 0)\r\n";
    script += "Subtitle(\"use with Retina display\", 226, 374, 0, 0, \"Arial\", 32, $FFFF00, 0, 5, 0)\r\n";

    // try to split file name into 1 or 3 lines
    QString title = movie.baseName();
    QStringList parts = title.split(" ",QString::SkipEmptyParts);
    switch(parts.count()) {
    case 2:
        script += "Subtitle(\""+parts.at(0)+"\", 800, 240, 0, 0, \"Arial\", 90, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(1)+"\", 800, 320, 0, 0, \"Arial\", 90, $FFFFFF, 0, 5, 0)\r\n";
        break;
    case 3 :
        script += "Subtitle(\""+parts.at(0)+"\", 800, 186, 0, 0, \"Arial\", 80, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(1)+"\", 800, 273, 0, 0, \"Arial\", 80, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(2)+"\", 800, 360, 0, 0, \"Arial\", 80, $FFFFFF, 0, 5, 0)\r\n";
        break;
    case 4 :
        script += "Subtitle(\""+parts.at(0)+"\", 800, 173, 0, 0, \"Arial\", 73, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(1)+"\", 800, 240, 0, 0, \"Arial\", 73, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(2)+"\", 800, 307, 0, 0, \"Arial\", 73, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(3)+"\", 800, 373, 0, 0, \"Arial\", 73, $FFFFFF, 0, 5, 0)\r\n";
        break;
    case 1:
        if (title.length()<20) {
            script += "Subtitle(\""+title.replace("_"," ")+"\", 800, 280, 0, 0, \"Arial\", 80, $FFFFFF, 0, 5, 0)\r\n";
            break;
        } // else fall thru to default
    default:
        script += "# The suggested title is : " + title + "\r\n";
        script += "Subtitle(\"Please manually\",  800, 173, 0, 0, \"Arial\", 73, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\"edit these lines\", 800, 240, 0, 0, \"Arial\", 73, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\"in the avs file!\", 800, 307, 0, 0, \"Arial\", 73, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\"\", 800, 373, 0, 0, \"Arial\", 73, $FFFFFF, 0, 5, 0)\r\n";
        break;
    }

    /*
    script += "Subtitle(\"More about this 3-D method on\", 800, 420, 0, 0, \"Arial\", 50, $FFFFFF, 0, 5, 0)\r\n";
    script += "Subtitle(\"http://openMy3D.blogspot.com\", 800, 420, 0, 0, \"Arial\", 50, $FFFFFF, 0, 5, 0)\r\n";
    */
    //script += "#ImageWriter(\"C:\\\", 100, 100, \"PNG\")\r\n";

    // output avs file
    QString avsName = newName + ".avs";
    QFile avsFile(avsName);
    avsFile.open(QIODevice::WriteOnly);
    QTextStream avsStream(&avsFile);
    avsStream << script;
    avsFile.close();

    // output conversion batch file
    QString mp4 = newName + ".mp4";
#ifdef Q_WS_WIN
    QString batName = newName + ".bat";
    QFile batFile(batName);
    batFile.open(QIODevice::WriteOnly);
    QTextStream batStream(&batFile);
    QString line;
    line = "ffmpeg -i \""+avsName+"\" -b:v 4500K -y";
    if(fps != tr("keep"))
        line += " -r " + fps;
    line += " \""+mp4+"\"\r\n";
    batStream << line;
    *s << line;
    batFile.close();
#endif
#ifdef Q_WS_LINUX
#endif

}

void MainWindow::createGRilli( QTextStream *s, int index, int w, int h )
{
    QString fps = moviesTable->item(index,2)->text();
    QString oldName = moviesTable->item(index,3)->text().replace(QString("\\"),QString("/"));
    QFileInfo movie(oldName);
    QString widthStr = QString::number(w);
    QString heightStr = QString::number(h);
    QString height2Str;
    QString border2Str = QString::number((h-(w*9/16))/2);

    QString newName = movie.absolutePath() + QString("/") + movie.baseName() + "_GRilli" + widthStr;

    QString script(8000);
    script = "# This is processed on the fly by the DirectShow pluging AviSynth\r\n";
    script += "LoadPlugin(\"C:\\Programme\\AviSynth 2.5\\plugins\\MaskTools.dll\")\r\n";
    script += "in = DirectShowSource(\"" + oldName +"\"";
    if (!fps.isEmpty() && (fps != tr("keep"))) script += ", fps="+fps;
    script += ")\r\n";
    if( moviesTable->item(index,1)->text()==tr("costum") ) {
        height2Str = QString::number(w*9/16);
        script += "# Please manually change "+ widthStr+"(width), "+height2Str+"(height)\r\n";
        script += "left = BilinearResize(in, "+ widthStr+", "+height2Str+", 0, 0, in.width/2, in.height)\r\n";
        script += "right = BilinearResize(in, "+ widthStr+", "+height2Str+", in.width/2, 0, in.width/2, in.height)\r\n";
     } else if( moviesTable->item(index,1)->text()==tr("16:9") ) {
        height2Str = QString::number(w*9/16);
        if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
         || (moviesTable->item(index,0)->text()==tr("SBS LR")) ){
            script += "left = BilinearResize(in, "+ widthStr+", "+height2Str+", 0, 0, in.width/2, in.height)\r\n";
            script += "right = BilinearResize(in, "+ widthStr+", "+height2Str+", in.width/2, 0, in.width/2, in.height)\r\n";
        } else {
            script += "left = BilinearResize(in, "+ widthStr+", "+height2Str+", 0, 0, in.width, in.height/2)\r\n";
            script += "right = BilinearResize(in, "+ widthStr+", "+height2Str+", 0, in.height/2, in.width, in.height/2)\r\n";
        }
    } else { // 4:3
        height2Str = QString::number(w*3/4);
        if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
         || (moviesTable->item(index,0)->text()==tr("SBS LR")) ){
            script += "left = BilinearResize(in, "+ widthStr+", "+height2Str+", 0, 0, in.width/2, in.height)\r\n";
            script += "right = BilinearResize(in, "+ widthStr+", "+height2Str+", in.width/2, 0, in.width/2, in.height)\r\n";
        } else {
            script += "left = BilinearResize(in, "+ widthStr+", "+height2Str+", 0, 0, in.width, in.height/2)\r\n";
            script += "right = BilinearResize(in, "+ widthStr+", "+height2Str+", 0, in.height/2, in.width, in.height/2)\r\n";
        }
    }

    // prepare L/R stripe mask for lenticular separation
    if(w==1024)
        script += "mask = ImageSource(\""+QDir::currentPath()+"/GRilli3D_iPad_Mask.png\", end=FrameCount(in), fps=FrameRate(in))\r\n";
    else
        script += "mask = ImageSource(\""+QDir::currentPath()+"/GRilli3D_iPod_Mask.png\", end=FrameCount(in), fps=FrameRate(in))\r\n";
    script += "# In case the movie is of size 0 KB, try to move the # in next line to the line below, save and retry.\n\r";
    script += "#mask2 = BilinearResize(mask, "+ widthStr+", "+height2Str+")\r\n";
    script += "mask2 = ConvertToYV12(BilinearResize(mask, "+ widthStr+", "+height2Str+"))\r\n";
    if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
     || (moviesTable->item(index,0)->text()==tr("OU RL")) ){
        script += "MaskedMerge (left, right, mask2)\r\n";
    } else {
        script += "MaskedMerge (right, left, mask2)\r\n";
    }

    if (w==1024)
    {
        // scale for 576px total hight

        // Add title on first frame
        script += "Subtitle(\"3-D\", 512, 140, 0, 0, \"Arial\", 180, $FFFFFF, 0, 5, 0)\r\n";

        // try to split file name into 1 or 3 lines
        QString title = movie.baseName();
        QStringList parts = title.split(" ",QString::SkipEmptyParts);
        switch(parts.count()) {
        case 2:
            script += "Subtitle(\""+parts.at(0)+"\", 512, 300, 0, 0, \"Arial\", 80, $FFFFFF, 0, 5, 0)\r\n";
            script += "Subtitle(\""+parts.at(1)+"\", 512, 360, 0, 0, \"Arial\", 80, $FFFFFF, 0, 5, 0)\r\n";
            break;
        case 3 :
            script += "Subtitle(\""+parts.at(0)+"\", 512, 260, 0, 0, \"Arial\", 80, $FFFFFF, 0, 5, 0)\r\n";
            script += "Subtitle(\""+parts.at(1)+"\", 512, 320, 0, 0, \"Arial\", 80, $FFFFFF, 0, 5, 0)\r\n";
            script += "Subtitle(\""+parts.at(2)+"\", 512, 380, 0, 0, \"Arial\", 80, $FFFFFF, 0, 5, 0)\r\n";
            break;
        case 1:
            if (title.length()<20) {
                script += "Subtitle(\""+title.replace("_"," ")+"\", 512, 320, 0, 0, \"Arial\", 80, $FFFFFF, 0, 5, 0)\r\n";
                break;
            } // else fall thru to default
        default:
            script += "# The suggested title is : " + title + "\r\n";
            script += "Subtitle(\"Please manually\",  512, 260, 0, 0, \"Arial\", 80, $FFFFFF, 0, 5, 0)\r\n";
            script += "Subtitle(\"edit these lines\", 512, 320, 0, 0, \"Arial\", 80, $FFFFFF, 0, 5, 0)\r\n";
            script += "Subtitle(\"in the avs file!\", 512, 380, 0, 0, \"Arial\", 80, $FFFFFF, 0, 5, 0)\r\n";
            break;
        }

        script += "Subtitle(\"More about this 3-D method on\", 512, 490, 0, 0, \"Arial\", 50, $FFFF00, 0, 5, 0)\r\n";
        script += "Subtitle(\"http://openKMQ.blogspot.com\", 512, 550, 0, 0, \"Arial\", 50, $FFFF00, 0, 5, 0)\r\n";
    } else {
        // scale for 480 x 270px total size

        // Add title on first frame
        script += "Subtitle(\"3-D\", 240, 66, 0, 0, \"Arial\", 85, $FFFFFF, 0, 5, 0)\r\n";

        // try to split file name into 1 or 3 lines
        QString title = movie.baseName();
        QStringList parts = title.split(" ",QString::SkipEmptyParts);
        switch(parts.count()) {
        case 2:
            script += "Subtitle(\""+parts.at(0)+"\", 240, 140, 0, 0, \"Arial\", 36, $FFFFFF, 0, 5, 0)\r\n";
            script += "Subtitle(\""+parts.at(1)+"\", 240, 168, 0, 0, \"Arial\", 36, $FFFFFF, 0, 5, 0)\r\n";
            break;
        case 3 :
            script += "Subtitle(\""+parts.at(0)+"\", 240, 121, 0, 0, \"Arial\", 36, $FFFFFF, 0, 5, 0)\r\n";
            script += "Subtitle(\""+parts.at(1)+"\", 240, 150, 0, 0, \"Arial\", 36, $FFFFFF, 0, 5, 0)\r\n";
            script += "Subtitle(\""+parts.at(2)+"\", 240, 180, 0, 0, \"Arial\", 36, $FFFFFF, 0, 5, 0)\r\n";
            break;
        case 1:
            if (title.length()<20) {
                script += "Subtitle(\""+title.replace("_"," ")+"\", 240, 150, 0, 0, \"Arial\", 36, $FFFFFF, 0, 5, 0)\r\n";
                break;
            } // else fall thru to default
        default:
            script += "# The suggested title is : " + title + "\r\n";
            script += "Subtitle(\"Please manually\",  240, 121, 0, 0, \"Arial\", 36, $FFFFFF, 0, 5, 0)\r\n";
            script += "Subtitle(\"edit these lines\", 240, 150, 0, 0, \"Arial\", 36, $FFFFFF, 0, 5, 0)\r\n";
            script += "Subtitle(\"in the avs file!\", 240, 180, 0, 0, \"Arial\", 36, $FFFFFF, 0, 5, 0)\r\n";
            break;
        }

        script += "Subtitle(\"More about this 3-D method on\", 240, 230, 0, 0, \"Arial\", 24, $FFFF00, 0, 5, 0)\r\n";
        script += "Subtitle(\"http://openKMQ.blogspot.com\", 240, 256, 0, 0, \"Arial\", 24, $FFFF00, 0, 5, 0)\r\n";
    }
    //script += "#ImageWriter(\"C:\\\", 100, 100, \"PNG\")\r\n";
    /**/

    // output avs file
    QString avsName = newName + ".avs";
    QFile avsFile(avsName);
    avsFile.open(QIODevice::WriteOnly);
    QTextStream avsStream(&avsFile);
    avsStream << script;
    avsFile.close();

    // output conversion batch file
    QString mp4 = newName + ".mp4";
#ifdef Q_WS_WIN
    QString batName = newName + ".bat";
    QFile batFile(batName);
    batFile.open(QIODevice::WriteOnly);
    QTextStream batStream(&batFile);
    QString line;
    line = "ffmpeg -i \""+avsName+"\" -b:v 4500K -y";
    if(fps != tr("keep"))
        line += " -r " + fps;
    line += " \""+mp4+"\"\r\n";
    batStream << line;
    *s << line;
    batStream << "ECHO If you want to transfer the movie to an iPad\r\n";
    batStream << "ECHO using the Camera Connection Kit, please manually\r\n";
    batStream << "ECHO rename the movie file to an XXXXXXXX.MP4 name (exact 8.3 capital chars!)\r\n";
    batStream << "PAUSE Press any Key\r\n";
    batFile.close();
#endif
#ifdef Q_WS_LINUX
#endif

}

void MainWindow::createXPol( QTextStream *s, int index, int w, int h )
{
    QString fps = moviesTable->item(index,2)->text();
    QString oldName = moviesTable->item(index,3)->text().replace(QString("\\"),QString("/"));
    QFileInfo movie(oldName);
    QString widthStr = QString::number(w);
    QString heightStr = QString::number(h);
    QString height2Str;
    QString border2Str = QString::number((h-(w*9/16))/2);

    QString newName = movie.absolutePath() + QString("/") + movie.baseName() + "_xPol" + widthStr;

    QString script(8000);
    script = "# This is processed on the fly by the DirectShow pluging AviSynth\r\n";
    script += "LoadPlugin(\"C:\\Programme\\AviSynth 2.5\\plugins\\MaskTools.dll\")\r\n";
    script += "in = DirectShowSource(\"" + oldName +"\"";
    if (!fps.isEmpty() && (fps != tr("keep"))) script += ", fps="+fps;
    script += ")\r\n";
    if( moviesTable->item(index,1)->text()==tr("costum") ) {
        height2Str = QString::number(w*9/16);
        script += "# Please manually change "+ widthStr+"(width), "+height2Str+"(height)\r\n";
        script += "left = BilinearResize(in, "+ widthStr+", "+height2Str+", 0, 0, in.width/2, in.height).AddBorders(0, 0, 0, 0, color_black )\r\n";
        script += "right = BilinearResize(in, "+ widthStr+", "+height2Str+", in.width/2, 0, in.width/2, in.height).AddBorders(0, 0, 0, 0, color_black )\r\n";
     } else if( moviesTable->item(index,1)->text()==tr("16:9") ) {
        height2Str = QString::number(w*9/16);
        if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
         || (moviesTable->item(index,0)->text()==tr("SBS LR")) ){
            script += "left = BilinearResize(in, "+ widthStr+", "+height2Str+", 0, 0, in.width/2, in.height)\r\n";
            script += "right = BilinearResize(in, "+ widthStr+", "+height2Str+", in.width/2, 0, in.width/2, in.height)\r\n";
        } else {
            script += "left = BilinearResize(in, "+ widthStr+", "+height2Str+", 0, 0, in.width, in.height/2)\r\n";
            script += "right = BilinearResize(in, "+ widthStr+", "+height2Str+", 0, in.height/2, in.width, in.height/2)\r\n";
        }
    } else { // 4:3
        height2Str = QString::number(w*3/4);
        if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
         || (moviesTable->item(index,0)->text()==tr("SBS LR")) ){
            script += "left = BilinearResize(in, "+ widthStr+", "+heightStr+", 0, 0, in.width/2, in.height)\r\n";
            script += "right = BilinearResize(in, "+ widthStr+", "+heightStr+", in.width/2, 0, in.width/2, in.height)\r\n";
        } else {
            script += "left = BilinearResize(in, "+ widthStr+", "+heightStr+", 0, 0, in.width, in.height/2)\r\n";
            script += "right = BilinearResize(in, "+ widthStr+", "+heightStr+", 0, in.height/2, in.width, in.height/2)\r\n";
        }
    }

    // prepare L/R stripe mask for lenticular separation
    if(w==1366)
        script += "mask = ImageSource(\""+QDir::currentPath()+"/1366_Mask.png\", end=FrameCount(in), fps=FrameRate(in))\r\n";
    script += "# In case the movie is of size 0 KB, try to move the # in next line to the line below, save and retry.\n\r";
    script += "#mask2 = BilinearResize(mask, "+ widthStr+", "+height2Str+")\r\n";
    script += "mask2 = ConvertToYV12(BilinearResize(mask, "+ widthStr+", "+height2Str+"))\r\n";
    if( (moviesTable->item(index,0)->text()==tr("SBS RL"))
     || (moviesTable->item(index,0)->text()==tr("OU RL")) ){
        script += "MaskedMerge (left, right, mask2)\r\n";
    } else {
        script += "MaskedMerge (right, left, mask2)\r\n";
    }

    /*
    // Add title on first frame
    script += "Subtitle(\"3-D\", 383, 190, 0, 0, \"Arial\", 180, $FFFFFF, 0, 5, 0)\r\n";

    // try to split file name into 1 or 3 lines
    QString title = movie.baseName();
    QStringList parts = title.split(" ",QString::SkipEmptyParts);
    switch(parts.count()) {
    case 2:
        script += "Subtitle(\""+parts.at(0)+"\", 512, 380, 0, 0, \"Arial\", 100, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(1)+"\", 512, 520, 0, 0, \"Arial\", 100, $FFFFFF, 0, 5, 0)\r\n";
        break;
    case 3 :
        script += "Subtitle(\""+parts.at(0)+"\", 512, 320, 0, 0, \"Arial\", 100, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(1)+"\", 512, 430, 0, 0, \"Arial\", 100, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\""+parts.at(2)+"\", 512, 560, 0, 0, \"Arial\", 100, $FFFFFF, 0, 5, 0)\r\n";
        break;
    case 1:
        if (title.length()<20) {
            script += "Subtitle(\""+title.replace("_"," ")+"\", 512, 430, 0, 0, \"Arial\", 100, $FFFFFF, 0, 5, 0)\r\n";
            break;
        } // else fall thru to default
    default:
        script += "# The suggested title is : " + title + "\r\n";
        script += "Subtitle(\"Please manually\", 512, 320, 0, 0, \"Arial\", 100, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\"edit these lines\", 512, 430, 0, 0, \"Arial\", 100, $FFFFFF, 0, 5, 0)\r\n";
        script += "Subtitle(\"in the avs file!\", 512, 560, 0, 0, \"Arial\", 100, $FFFFFF, 0, 5, 0)\r\n";
        break;
    }

    script += "Subtitle(\"More about this 3-D method on\", 512, 700, 0, 0, \"Arial\", 50, $FFFFFF, 0, 5, 0)\r\n";
    script += "Subtitle(\"http://openKMQ.blogspot.com\", 512, 760, 0, 0, \"Arial\", 50, $FFFFFF, 0, 5, 0)\r\n";
    //script += "#ImageWriter(\"C:\\\", 100, 100, \"PNG\")\r\n";
    */

    // output avs file
    QString avsName = newName + ".avs";
    QFile avsFile(avsName);
    avsFile.open(QIODevice::WriteOnly);
    QTextStream avsStream(&avsFile);
    avsStream << script;
    avsFile.close();

    // output conversion batch file
    QString mp4 = newName + ".mp4";
#ifdef Q_WS_WIN
    QString batName = newName + ".bat";
    QFile batFile(batName);
    batFile.open(QIODevice::WriteOnly);
    QTextStream batStream(&batFile);
    QString line;
    line = "ffmpeg -i \""+avsName+"\" -b:v 8500K -y";
    if(fps != tr("keep"))
        line += " -r " + fps;
    line += " \""+mp4+"\"\r\n";
    batStream << line;
    *s << line;
    batFile.close();
#endif
#ifdef Q_WS_LINUX
#endif

}

void MainWindow::create()
{
    int movieIdx;
    int moviesCount = moviesTable->rowCount();
#ifdef Q_WS_WIN
    QTemporaryFile batFile("XXXXXX.bat");
#else
    QTemporaryFile batFile;
#endif
    batFile.open();
    QTextStream allScripts(&batFile);

    for( movieIdx = 0; movieIdx < moviesCount; movieIdx++ ) {
        // ffmpeg only
        if(checkKmqP->isChecked())          createKMQiPad(&allScripts, movieIdx, false);
        if(checkKmqL->isChecked())          createKMQiPad(&allScripts, movieIdx, true);
        if(checkKmq1080p->isChecked())      createKMQ1080p(&allScripts, movieIdx);
        if(checkKmq1280->isChecked())       createKMQ1280(&allScripts, movieIdx);
        if(checkMy3D480p->isChecked())      createMy3D480p(&allScripts, movieIdx);
        if(checkMy3D720p->isChecked())      createMy3D720p(&allScripts, movieIdx);
        if(checkMy3D960->isChecked())       createMy3D960(&allScripts, movieIdx);
#ifdef Q_WS_WIN
        // needing AviSynth
        if(checkGRilliPad->isChecked())     createGRilli(&allScripts, movieIdx, 1024, 768);
        if(checkGRilliTouch->isChecked())   createGRilli(&allScripts, movieIdx, 480, 320);
        if(checkFeed->isChecked())          create3df33d(&allScripts, movieIdx);
        if(check3DTV->isChecked())          create3dtv(&allScripts, movieIdx);
        if(checkXPol1366->isChecked())      createXPol(&allScripts, movieIdx, 1366, 768);
        if(checkMy3D960avs->isChecked())    createMy3D960avs(&allScripts, movieIdx);
        if(checkMy3D1280->isChecked())      createMy3D1280(&allScripts, movieIdx);
#endif
    }

#ifdef Q_WS_WIN
    QString cmd = "call "+batFile.fileName();
#else
    QString cmd = "sh "+batFile.fileName();
#endif
    QByteArray ba = cmd.toLocal8Bit();
    const char *c_cmd = ba.data();
    system(c_cmd);
}
