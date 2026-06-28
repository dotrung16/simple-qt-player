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
    
    connect(player, &QMediaPlayer::positionChanged, this, &AudioPlayer::positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &AudioPlayer::durationChanged);
    connect(player, &QMediaPlayer::errorOccurred, this, &AudioPlayer::handleError);
}

void AudioPlayer::addTrack(const QUrl &url)
{
    playlist.append(url);
    if (currentIndex == -1) currentIndex = 0;
}

void AudioPlayer::removeTrack(int index)
{
    if (index >= 0 && index < playlist.size()) 
    {
        playlist.removeAt(index);
        
        if (index < currentIndex) {
            currentIndex--;
        } else if (index == currentIndex) {
            if (playlist.isEmpty()) {
                player->stop();
                currentIndex = -1;
                emit trackChanged("No track loaded");
                emit coverArtChanged(QImage());
            } else {
                currentIndex = currentIndex % playlist.size();
                playTrack(currentIndex);
            }
        }
    }
}

void AudioPlayer::clearPlaylist()
{
    playlist.clear();
    player->stop();
    currentIndex = -1;
    emit trackChanged("No track loaded");
    emit coverArtChanged(QImage());
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
        emit trackIndexChanged(currentIndex); 
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
    else if (!playlist.isEmpty())
    {
        if (player->source().isEmpty()) playTrack(currentIndex);
        else player->play();
        emit playbackStateChanged(true);
    }
}

void AudioPlayer::nextTrack()
{
    if (playlist.isEmpty()) return;
    int nextIndex = (currentIndex + 1) % playlist.size();
    playTrack(nextIndex);
}

void AudioPlayer::previousTrack()
{
    if (playlist.isEmpty()) return;

    int prevIndex = (currentIndex - 1 + static_cast<int>(playlist.size())) % playlist.size();
    playTrack(prevIndex);
}

void AudioPlayer::setPosition(int position)
{
    player->setPosition(position);
}

void AudioPlayer::setVolume(float volume)
{
    audioOutput->setVolume(volume);
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

    if (coverVariant.isNull()) {
        coverVariant = metadata.value(QMediaMetaData::ThumbnailImage);
    }

    if (coverVariant.canConvert<QImage>()) {
        emit coverArtChanged(coverVariant.value<QImage>());
    } else {
        emit coverArtChanged(QImage());
    }
}

void AudioPlayer::handleError(QMediaPlayer::Error error, const QString &errorString)
{
    Q_UNUSED(error);
    emit errorOccurred(errorString);
}