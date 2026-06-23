#include "audioplayer.h"
#include <QFileInfo>

AudioPlayer::AudioPlayer(QObject *parent) : QObject(parent), currentIndex(-1)
{
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);

    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);

    connect(player, &QMediaPlayer::mediaStatusChanged, this, &AudioPlayer::handleMediaStatus);
    connect(player, &QMediaPlayer::metaDataChanged, this, &AudioPlayer::extractMetaData);
}

void AudioPlayer::addTrack(const QUrl &url)
{
    playlist.push_back(url);

    if (currentIndex == -1) currentIndex = 0;
}

void AudioPlayer::playTrack(int index)
{
    if (index >= 0 && index < playlist.size()) 
    {
        currentIndex = index;
        
        player->setSource(playlist[currentIndex]);
        player->play();

        QFileInfo fileInfo(playlist[currentIndex].toLocalFile());

        emit trackChanged(fileInfo.fileName());
        emit playbackStateChanged(true);
    }
}

void AudioPlayer::togglePlayPause()
{
    if (player->playbackState() == QMediaPlayer::PlayingState)
    {
        player->pause();

        emit playbackStateChanged(false);
    }
    else if (!playlist.empty())
    {
        if (player->source().isEmpty()) playTrack(currentIndex);
        else player->play();

        emit playbackStateChanged(true);
    }
}

void AudioPlayer::nextTrack()
{
    if (playlist.empty()) return;

    int nextIndex = (currentIndex + 1) % playlist.size();

    playTrack(nextIndex);
}

void AudioPlayer::previousTrack()
{
    if (playlist.empty()) return;

    int prevIndex = (currentIndex - 1 + playlist.size()) % playlist.size();

    playTrack(prevIndex);
}

void AudioPlayer::handleMediaStatus(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) 
    {
        nextTrack();
    }
}

void AudioPlayer::extractMetaData()
{
    QMediaMetaData metadata = player->metaData();

    QVariant coverVariant = metadata.value(QMediaMetaData::CoverArtImage);

    if (coverVariant.isNull())
    {
        coverVariant = metadata.value(QMediaMetaData::ThumbnailImage);
    }

    if (coverVariant.canConvert<QImage>())
    {
        QImage image = coverVariant.value<QImage>();

        emit coverArtChanged(image);
    }
    else
    {
        emit coverArtChanged(QImage());
    }
}