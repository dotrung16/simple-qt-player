#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QWidget>
#include <QTime>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), currentDuration(0)
{
    audio_conf = new AudioPlayer(this);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    coverLabel = new QLabel("No Cover Art", this);
    coverLabel->setAlignment(Qt::AlignCenter);
    coverLabel->setFixedSize(250, 250);
    coverLabel->setObjectName("coverLabel"); 

    nowPlayingLabel = new QLabel("No track loaded", this);
    nowPlayingLabel->setAlignment(Qt::AlignCenter);

    playlistWidget = new QListWidget(this);
    progressSlider = new QSlider(Qt::Horizontal, this);
    timeLabel = new QLabel("00:00 / 00:00", this);

    QHBoxLayout *progressLayout = new QHBoxLayout();
    progressLayout->addWidget(progressSlider);
    progressLayout->addWidget(timeLabel);

    QHBoxLayout *controlsLayout = new QHBoxLayout();
    prevButton = new QPushButton("|<", this);
    playButton = new QPushButton("Play", this);
    nextButton = new QPushButton(">|", this);
    
    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);
    volumeSlider->setFixedWidth(100);

    controlsLayout->addWidget(prevButton);
    controlsLayout->addWidget(playButton);
    controlsLayout->addWidget(nextButton);
    controlsLayout->addStretch();
    controlsLayout->addWidget(new QLabel("Vol:", this));
    controlsLayout->addWidget(volumeSlider);

    QHBoxLayout *playlistControlsLayout = new QHBoxLayout();
    addButton = new QPushButton("Add Files", this);
    removeButton = new QPushButton("Remove", this);
    clearButton = new QPushButton("Clear All", this);
    
    playlistControlsLayout->addWidget(addButton);
    playlistControlsLayout->addWidget(removeButton);
    playlistControlsLayout->addWidget(clearButton);

    mainLayout->addWidget(coverLabel, 0, Qt::AlignCenter);
    mainLayout->addWidget(nowPlayingLabel);
    mainLayout->addLayout(progressLayout);
    mainLayout->addLayout(controlsLayout);
    mainLayout->addLayout(playlistControlsLayout);
    mainLayout->addWidget(playlistWidget);

    setCentralWidget(centralWidget);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::openFiles);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::removeSelectedTrack);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearPlaylist);
    
    connect(playButton, &QPushButton::clicked, audio_conf, &AudioPlayer::togglePlayPause);
    connect(nextButton, &QPushButton::clicked, audio_conf, &AudioPlayer::nextTrack);
    connect(prevButton, &QPushButton::clicked, audio_conf, &AudioPlayer::previousTrack);
    connect(playlistWidget, &QListWidget::currentRowChanged, audio_conf, &AudioPlayer::playTrack);
    
    connect(volumeSlider, &QSlider::valueChanged, this, &MainWindow::changeVolume);
    connect(progressSlider, &QSlider::sliderMoved, audio_conf, &AudioPlayer::setPosition);

    connect(audio_conf, &AudioPlayer::playbackStateChanged, this, &MainWindow::update_UI_State);
    connect(audio_conf, &AudioPlayer::trackChanged, nowPlayingLabel, &QLabel::setText);
    connect(audio_conf, &AudioPlayer::coverArtChanged, this, &MainWindow::updateCoverArt);
    connect(audio_conf, &AudioPlayer::positionChanged, this, &MainWindow::updatePosition);
    connect(audio_conf, &AudioPlayer::durationChanged, this, &MainWindow::updateDuration);
    connect(audio_conf, &AudioPlayer::trackIndexChanged, playlistWidget, [this](int row) {
        playlistWidget->setCurrentRow(row);
    });
    connect(audio_conf, &AudioPlayer::errorOccurred, this, &MainWindow::handlePlaybackError);
}

void MainWindow::openFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Open Audio File", "", "Audio File (*.mp3, *.wav, *.ogg, *.flac)");

    for (const QString &file : files)
    {
        audio_conf->addTrack(QUrl::fromLocalFile(file));
        QFileInfo fileInfo(file);
        playlistWidget->addItem(fileInfo.fileName());
    }
}

void MainWindow::removeSelectedTrack()
{
    int row = playlistWidget->currentRow();
    if (row >= 0) {
        delete playlistWidget->takeItem(row);
        audio_conf->removeTrack(row);
    }
}

void MainWindow::clearPlaylist()
{
    playlistWidget->clear();
    audio_conf->clearPlaylist();
}

void MainWindow::update_UI_State(bool isPlaying)
{
    playButton->setText(isPlaying ? "Pause" : "Play");
}

void MainWindow::updatePosition(qint64 position)
{
    if (!progressSlider->isSliderDown()) {
        progressSlider->setValue(position);
    }
    formatTime(position, currentDuration);
}

void MainWindow::updateDuration(qint64 duration)
{
    currentDuration = duration;
    progressSlider->setRange(0, duration);
    formatTime(progressSlider->value(), duration);
}

void MainWindow::formatTime(qint64 position, qint64 duration)
{
    QTime currentTime(0, 0, 0);
    currentTime = currentTime.addMSecs(position);
    
    QTime totalTime(0, 0, 0);
    totalTime = totalTime.addMSecs(duration);

    QString format = duration > 3600000 ? "hh:mm:ss" : "mm:ss";
    timeLabel->setText(currentTime.toString(format) + " / " + totalTime.toString(format));
}

void MainWindow::updateCoverArt(const QImage &image)
{
    if (image.isNull()) {
        coverLabel->setPixmap(QPixmap());
        coverLabel->setText("No Cover Art");
    } else {
        QPixmap pixmap = QPixmap::fromImage(image);
        QPixmap scaledPixmap = pixmap.scaled(coverLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        coverLabel->setPixmap(scaledPixmap);
    }
}

void MainWindow::handlePlaybackError(const QString &errorMsg)
{
    QMessageBox::warning(this, "Playback Error", "Error playing media: " + errorMsg);
}

void MainWindow::changeVolume(int value)
{
    audio_conf->setVolume(value / 100.0f);
}