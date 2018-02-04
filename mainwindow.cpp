#include <QKeyEvent>
#include <QTime>
#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>
#include <QFileDialog>
#include <QInputDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "VLCQtCore/Common.h"
#include "VLCQtCore/Instance.h"
#include "VLCQtCore/Media.h"
#include "VLCQtCore/Video.h"
#include "VLCQtCore/MediaPlayer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pInstance(NULL),
    m_pMedia(NULL),
    m_pPlayer(NULL),
    m_bFullScreen(false)
{
    ui->setupUi(this);

    m_pInstance = new VlcInstance(VlcCommon::args(), this);
    m_pPlayer = new VlcMediaPlayer(m_pInstance);
    m_pPlayer->setVideoWidget(ui->video);

    ui->video->setMediaPlayer(m_pPlayer);
    ui->seek->setMediaPlayer(m_pPlayer);

    connect(ui->action_Exit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionOpen_Local, SIGNAL(triggered()), this, SLOT(onOpenLocal()));
    connect(ui->actionOpen_URL, SIGNAL(triggered()), this, SLOT(onOpenUrl()));

    connect(ui->action_Play, SIGNAL(triggered()), this, SLOT(onPlay()));
    connect(ui->action_Pause, SIGNAL(triggered()), this, SLOT(onPause()));
    connect(ui->action_Stop, SIGNAL(triggered()), this, SLOT(onStop()));

    connect(ui->action_SnapShot, SIGNAL(triggered()), this, SLOT(onSnapShot()));
    connect(ui->action_Full_Screen, SIGNAL(triggered()), this, SLOT(onFullScreen()));

    connect(ui->action_About, SIGNAL(triggered()), this, SLOT(onAbout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_F:
        onFullScreen();
        break;
    case Qt::Key_P:
        onSnapShot();
        break;
    }
}

void MainWindow::onOpenLocal()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath(), tr("Videos (*.avi *.mkv *.wmv *.rmvb *.mp4)"));
    path = path.trimmed();
    if(path.isEmpty())
        return;
    qDebug() << path;
    if(m_pMedia != NULL)
    {
        delete m_pMedia;
        m_pMedia = NULL;
    }
    m_pMedia = new VlcMedia(path, true, m_pInstance);
    setWindowTitle(QFileInfo(path).fileName());
    m_pPlayer->open(m_pMedia);
}

void MainWindow::onOpenUrl()
{
    QString url = QInputDialog::getText(this, tr("Open Url"), tr("Enter the URL you want to play"));
    url = url.trimmed();
    if(url.isEmpty())
        return;
    if(m_pMedia != NULL)
    {
        delete m_pMedia;
        m_pMedia = NULL;
    }
    m_pMedia = new VlcMedia(url, m_pInstance);
    setWindowTitle(QFileInfo(url).fileName());
    m_pPlayer->open(m_pMedia);
}

void MainWindow::onPlay()
{
    if(m_pPlayer == NULL)
        return;
    m_pPlayer->play();
}

void MainWindow::onPause()
{
    if(m_pPlayer == NULL)
        return;
    m_pPlayer->pause();
}

void MainWindow::onStop()
{
    if(m_pPlayer == NULL)
        return;
    m_pPlayer->stop();
}

void MainWindow::onAbout()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::onSnapShot()
{
    if(m_pMedia == NULL || m_pPlayer == NULL)
        return;
    QString strPath = m_pMedia->currentLocation();
    QDir dir = QFileInfo(strPath).dir();
    bool is = m_pPlayer->video()->takeSnapshot(dir.absolutePath() + "/" + QTime::currentTime().toString("HH-mm-ss") + ".jpg");
    Q_UNUSED(is);
}

void MainWindow::onFullScreen()
{
    m_bFullScreen = !m_bFullScreen;
    m_bFullScreen ? showFullScreen() : showNormal();
    ui->mainToolBar->setVisible(!m_bFullScreen);
    menuBar()->setVisible(!m_bFullScreen);
    statusBar()->setVisible(!m_bFullScreen);
    ui->seek->setVisible(!m_bFullScreen);
}
