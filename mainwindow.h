#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class VlcMedia;
class VlcInstance;
class VlcMediaPlayer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

private slots:
    void onOpenLocal();
    void onOpenUrl();
    void onPlay();
    void onPause();
    void onStop();
    void onAbout();
    void onSnapShot();
    void onFullScreen();
    void onShowStatusBar(bool checked);

private:
    inline void openFile(const QString &strPath);
    inline void openUrl(const QString &strUrl);

private:
    Ui::MainWindow *ui;
    VlcInstance *m_pInstance;
    VlcMedia *m_pMedia;
    VlcMediaPlayer *m_pPlayer;
    bool m_bFullScreen;
    bool m_bPlaying;
    QStringList m_lstPlayList;
};

#endif // MAINWINDOW_H
