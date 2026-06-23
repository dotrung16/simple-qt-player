#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QListWidget>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include "audioplayer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;
private slots:
    void openFiles();
    void update_UI_State(bool isPlaying);
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void seekPosition(int position);
    void updateCoverArt(const QImage &image);
private:
    AudioPlayer *audio_conf;

    QPushButton *playButton;
    QPushButton *nextButton;
    QPushButton *prevButton;
    QPushButton *addButton;
    QSlider *progressSlider;
    QListWidget *playlistWidget;
    QLabel *nowPlayingLabel;
    QLabel *coverLabel;
};