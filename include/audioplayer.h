#pragma once

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QMediaMetaData>
#include <QImage>
#include <vector>

class AudioPlayer : public QObject
{
    Q_OBJECT

public:
    explicit AudioPlayer(QObject *parent = nullptr);

    void addTrack(const QUrl &url);
    void playTrack(int index);
    void togglePlayPause();
    void nextTrack();
    void previousTrack();

    QMediaPlayer* getPlayer() const { return player; }

signals:
    void trackChanged(const QString &fileName);
    void playbackStateChanged(bool isPlaying);
    void coverArtChanged(const QImage &image);

private slots:
    void handleMediaStatus(QMediaPlayer::MediaStatus status);
    void extractMetaData();

private:
    QMediaPlayer *player;
    QAudioOutput *audioOutput;

    std::vector<QUrl> playlist;
    int currentIndex;
};