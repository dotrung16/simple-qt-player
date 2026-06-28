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
    void removeSelectedTrack();
    void clearPlaylist();
    void update_UI_State(bool isPlaying);
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void updateCoverArt(const QImage &image);
    void formatTime(qint64 position, qint64 duration);
    void handlePlaybackError(const QString &errorMsg);
    void changeVolume(int value);

private:
    AudioPlayer *audio_conf;

    QPushButton *playButton;
    QPushButton *nextButton;
    QPushButton *prevButton;
    QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *clearButton;
    QSlider *progressSlider;
    QSlider *volumeSlider;
    QListWidget *playlistWidget;
    QLabel *nowPlayingLabel;
    QLabel *coverLabel;
    QLabel *timeLabel;
    
    qint64 currentDuration;
};