#include <QMimeData>
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
    m_bFullScreen(false),
    m_bPlaying(false),
    m_nCurrentIndex(-1)
{
    ui->setupUi(this);

    setAcceptDrops(true);
    statusBar()->setVisible(false);
    ui->playlist->setVisible(false);

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
    connect(ui->actionStausBar, SIGNAL(triggered(bool)), this, SLOT(onShowStatusBar(bool)));
    connect(ui->actionPlaylist, SIGNAL(triggered(bool)), this, SLOT(onShowPlaylist(bool)));

    connect(ui->action_About, SIGNAL(triggered()), this, SLOT(onAbout()));

    connect(ui->playlist, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(onPlaylistDoubleClicked(QListWidgetItem *)));

    ui->actionOpen_Local->setIcon(QIcon(":/image/open_file.png"));
    ui->mainToolBar->addAction(ui->actionOpen_Local);
    ui->actionOpen_URL->setIcon(QIcon(":/image/open_url.png"));
    ui->mainToolBar->addAction(ui->actionOpen_URL);
    ui->action_Exit->setIcon(QIcon(":/image/exit.png"));
    ui->mainToolBar->addSeparator();
    ui->action_Play->setIcon(QIcon(":/image/play.png"));
    ui->mainToolBar->addAction(ui->action_Play);
    ui->action_Pause->setIcon(QIcon(":/image/pause.png"));
    ui->mainToolBar->addAction(ui->action_Pause);
    ui->action_Stop->setIcon(QIcon(":/image/stop.png"));
    ui->mainToolBar->addAction(ui->action_Stop);
    ui->mainToolBar->addSeparator();
    ui->action_SnapShot->setIcon(QIcon(":/image/snapshot.png"));
    ui->mainToolBar->addAction(ui->action_SnapShot);
    ui->action_Full_Screen->setIcon(QIcon(":/image/fullscreen.png"));
    ui->mainToolBar->addAction(ui->action_Full_Screen);
    ui->actionStausBar->setIcon(QIcon(":/image/statusbar.png"));
    ui->mainToolBar->addAction(ui->actionStausBar);
    ui->actionPlaylist->setIcon(QIcon(":/image/playlist.png"));
    ui->mainToolBar->addAction(ui->actionPlaylist);
    ui->mainToolBar->addSeparator();
    ui->action_About->setIcon(QIcon(":/image/about.png"));
    ui->mainToolBar->addAction(ui->action_About);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Space:
        m_bPlaying ? onPause() : onPlay();
        break;
    case Qt::Key_F:
        ui->action_Full_Screen->trigger();
        break;
    case Qt::Key_S:
        ui->action_SnapShot->trigger();
        break;
    case Qt::Key_L:
        ui->actionPlaylist->trigger();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    e->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *e)
{
    QList<QUrl> urls = e->mimeData()->urls();
    if(urls.isEmpty())
        return ;

    foreach (QUrl u, urls) {
        qDebug() << u.toLocalFile();
        m_lstPlayList << u.toLocalFile();
    }

    if(urls.count() == m_lstPlayList.count())
    {
        m_nCurrentIndex = 0;
        openFile(m_nCurrentIndex);
    }

    updatePlaylist();
}

void MainWindow::onOpenLocal()
{
    QStringList lstPath = QFileDialog::getOpenFileNames(this, tr("Open file"), QDir::homePath(), tr("Videos (*.avi *.mkv *.wmv *.rmvb *.mp4)"));
    if(lstPath.count() == 0)
        return;
    m_lstPlayList = lstPath;
    m_nCurrentIndex = 0;
    openFile(m_nCurrentIndex);
    updatePlaylist();
}

void MainWindow::onOpenUrl()
{
    QString url = QInputDialog::getText(this, tr("Open Url"), tr("Enter the URL you want to play"));
    url = url.trimmed();
    if(url.isEmpty())
        return;
    openUrl(url);
}

void MainWindow::onPlay()
{
    if(m_pPlayer == NULL)
        return;
    m_pPlayer->play();
    m_bPlaying = true;
}

void MainWindow::onPause()
{
    if(m_pPlayer == NULL)
        return;
    m_pPlayer->pause();
    m_bPlaying = false;
}

void MainWindow::onStop()
{
    if(m_pPlayer == NULL)
        return;
    m_pPlayer->stop();
    m_bPlaying = false;
}

void MainWindow::onAbout()
{
    QMessageBox msg(this);
    msg.setIcon(QMessageBox::Information);
    msg.setWindowTitle(tr("About"));
    msg.setText("ZVideoPlay\nBase on VLC by zhaohuarong@gmail.com");
    msg.exec();
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

void MainWindow::onShowStatusBar(bool checked)
{
    ui->statusBar->setVisible(checked);
}

void MainWindow::onShowPlaylist(bool checked)
{
    ui->playlist->setVisible(checked);
    if(checked && ui->playlist->count() > 0)
    {
        ui->playlist->setFocus();
        if(m_nCurrentIndex < 0 || m_nCurrentIndex >= ui->playlist->count())
            m_nCurrentIndex = 0;
        ui->playlist->item(m_nCurrentIndex)->setSelected(true);
    }
}

void MainWindow::onPlaylistDoubleClicked(QListWidgetItem *item)
{
    if(item == NULL)
        return;
    int row = ui->playlist->row(item);
    if(row < 0 || row >= ui->playlist->count())
        return;
    openFile(row);
}

void MainWindow::addToPlaylist(const QString &strPath)
{
    if(!QFileInfo(strPath).exists())
        return;
    m_lstPlayList << strPath;
    if(m_lstPlayList.count() == 1)
    {
        m_nCurrentIndex = 0;
        openFile(m_nCurrentIndex);
    }

    updatePlaylist();
}

void MainWindow::openFile(int index)
{
    if(m_pMedia != NULL)
    {
        delete m_pMedia;
        m_pMedia = NULL;
    }
    m_nCurrentIndex = index;
    QString strPath = m_lstPlayList.at(index);
    m_pMedia = new VlcMedia(strPath, true, m_pInstance);
    setWindowTitle(QFileInfo(strPath).fileName());
    m_pPlayer->open(m_pMedia);
    m_bPlaying = true;
}

void MainWindow::openUrl(const QString &strUrl)
{
    if(m_pMedia != NULL)
    {
        delete m_pMedia;
        m_pMedia = NULL;
    }
    m_pMedia = new VlcMedia(strUrl, m_pInstance);
    setWindowTitle(QFileInfo(strUrl).fileName());
    m_pPlayer->open(m_pMedia);
    m_bPlaying = true;
}

void MainWindow::updatePlaylist()
{
    ui->playlist->clear();

    foreach(QString path, m_lstPlayList)
    {
        QListWidgetItem *item = new QListWidgetItem(ui->playlist);
        item->setText(QFileInfo(path).fileName());
        ui->playlist->addItem(item);
    }
}
