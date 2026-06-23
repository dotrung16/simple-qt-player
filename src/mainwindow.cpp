#include "mainwindow.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    audio_conf = new AudioPlayer(this);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    coverLabel = new QLabel("No Cover Art", this);
    coverLabel->setAlignment(Qt::AlignCenter);
    coverLabel->setFixedSize(250, 250);
    coverLabel->setStyleSheet("QLabel { background-color: #222; color: white; border-radius: 10px; }");

    nowPlayingLabel = new QLabel("No track loaded", this);
    playlistWidget = new QListWidget(this);
    progressSlider = new QSlider(Qt::Horizontal, this);

    QHBoxLayout *controlsLayout = new QHBoxLayout();
    addButton = new QPushButton("Add Files", this);
    prevButton = new QPushButton("|<", this);
    playButton = new QPushButton("Play", this);
    nextButton = new QPushButton(">|", this);

    controlsLayout->addWidget(addButton);
    controlsLayout->addWidget(prevButton);
    controlsLayout->addWidget(playButton);
    controlsLayout->addWidget(nextButton);

    mainLayout->addWidget(coverLabel, 0, Qt::AlignCenter);
    mainLayout->addWidget(nowPlayingLabel);
    mainLayout->addWidget(progressSlider);
    mainLayout->addLayout(controlsLayout);
    mainLayout->addWidget(playlistWidget);

    setCentralWidget(centralWidget);
    resize(400, 500);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::openFiles);
    connect(playButton, &QPushButton::clicked, audio_conf, &AudioPlayer::togglePlayPause);
    connect(nextButton, &QPushButton::clicked, audio_conf, &AudioPlayer::nextTrack);
    connect(prevButton, &QPushButton::clicked, audio_conf, &AudioPlayer::previousTrack);
    connect(playlistWidget, &QListWidget::currentRowChanged, audio_conf, &AudioPlayer::playTrack);

    connect(audio_conf, &AudioPlayer::playbackStateChanged, this, &MainWindow::update_UI_State);
    connect(audio_conf, &AudioPlayer::trackChanged, nowPlayingLabel, &QLabel::setText);
    connect(audio_conf, &AudioPlayer::coverArtChanged, this, &MainWindow::updateCoverArt);

    connect(audio_conf->getPlayer(), &QMediaPlayer::positionChanged, this, &MainWindow::updatePosition);
    connect(audio_conf->getPlayer(), &QMediaPlayer::durationChanged, this, &MainWindow::updateDuration);
    connect(progressSlider, &QSlider::sliderMoved, this, &MainWindow::seekPosition);
}

void MainWindow::openFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Open Audio File", "", "Audio File (*.mp3, *.wav, *.ogg, *flac)");

    for (const QString &file : files)
    {
        audio_conf->addTrack(QUrl::fromLocalFile(file));

        QFileInfo fileInfo(file);

        playlistWidget->addItem(fileInfo.fileName());
    }
}

void MainWindow::update_UI_State(bool isPlaying)
{
    playButton->setText(isPlaying ? "Pause" : "Play");
}

void MainWindow::updatePosition(qint64 position)
{
    if (!progressSlider->isSliderDown())
    {
        progressSlider->setValue(position);
    }
}

void MainWindow::updateDuration(qint64 duration)
{
    progressSlider->setRange(0, duration);
}

void MainWindow::seekPosition(int position)
{
    audio_conf->getPlayer()->setPosition(position);
}

void MainWindow::updateCoverArt(const QImage &image)
{
    if (image.isNull())
    {
        coverLabel->setPixmap(QPixmap());
        coverLabel->setText("No Cover Art");
    }
    else
    {
        QPixmap pixmap = QPixmap::fromImage(image);
        QPixmap scaledPixmap = pixmap.scaled(coverLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        coverLabel->setPixmap(scaledPixmap);
    }
}