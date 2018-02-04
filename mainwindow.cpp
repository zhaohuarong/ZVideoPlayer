#include <QDebug>
#include <QFileDialog>
#include <QInputDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "VLCQtCore/Common.h"
#include "VLCQtCore/Instance.h"
#include "VLCQtCore/Media.h"
#include "VLCQtCore/MediaPlayer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pInstance(NULL),
    m_pMedia(NULL),
    m_pPlayer(NULL)
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpenLocal()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath(), tr("Videos (*.avi *.mkv *.wmv *.rmvb *.mp4)"));
    if(path.trimmed().isEmpty())
        return;
    qDebug() << path;
    if(m_pMedia != NULL)
    {
        delete m_pMedia;
        m_pMedia = NULL;
    }
    m_pMedia = new VlcMedia(path, true, m_pInstance);
    m_pPlayer->open(m_pMedia);
}

void MainWindow::onOpenUrl()
{
    QString url = QInputDialog::getText(this, tr("Open Url"), tr("Enter the URL you want to play"));
    if(url.trimmed().isEmpty())
        return;
    if(m_pMedia != NULL)
    {
        delete m_pMedia;
        m_pMedia = NULL;
    }
    m_pMedia = new VlcMedia(url, m_pInstance);
    m_pPlayer->open(m_pMedia);
}
