#pragma once

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QMediaMetaData>
#include <QImage>
#include <QList>

class AudioPlayer : public QObject
{
    Q_OBJECT

public:
    explicit AudioPlayer(QObject *parent = nullptr);

    void addTrack(const QUrl &url);
    void removeTrack(int index);
    void clearPlaylist();
    void playTrack(int index);
    void togglePlayPause();
    void nextTrack();
    void previousTrack();

public slots:
    void setPosition(int position);
    void setVolume(float volume);

signals:
    void trackChanged(const QString &fileName);
    void trackIndexChanged(int index);
    void playbackStateChanged(bool isPlaying);
    void coverArtChanged(const QImage &image);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void errorOccurred(const QString &errorString);

private slots:
    void handleMediaStatus(QMediaPlayer::MediaStatus status);
    void extractMetaData();
    void handleError(QMediaPlayer::Error error, const QString &errorString);

private:
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QList<QUrl> playlist;
    int currentIndex;
};