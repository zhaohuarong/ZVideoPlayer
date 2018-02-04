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

private slots:
    void onOpenLocal();
    void onOpenUrl();
    void onPlay();
    void onPause();
    void onStop();
    void onAbout();
    void onSnapShot();
    void onFullScreen();

private:
    Ui::MainWindow *ui;
    VlcInstance *m_pInstance;
    VlcMedia *m_pMedia;
    VlcMediaPlayer *m_pPlayer;
    bool m_bFullScreen;
};

#endif // MAINWINDOW_H
