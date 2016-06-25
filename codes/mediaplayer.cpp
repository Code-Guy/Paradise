#include "mediaplayer.h"
#include "tool.h"

using namespace std;

MediaPlayer *MediaPlayer::instance = NULL;

MediaPlayer::MediaPlayer(QObject *parent)
	: QObject(parent)
{
	//初始化audioPlayer
	audioPlayer = new QMediaPlayer;
	//初始化audio files
	audioURLs[PutCell] = QUrl::fromLocalFile("Resources/sound/putcell.mp3");
	audioURLs[DeleteCell] = QUrl::fromLocalFile("Resources/sound/deletecell.mp3");

	//初始化musicPlayer
	musicPlayer = new QMediaPlayer;
	//初始化music files
	vector<string> musicPaths;
	TraverseFilesInDirectory("Resources/music", musicPaths, true);
	for (int i=0; i<musicPaths.size(); i++)
	{
		musicURLs.push_back(QUrl::fromLocalFile(QString::fromStdString(musicPaths[i])));
	}
	connect(musicPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(StateChanged(QMediaPlayer::State)));

	currentMusicIndex = -1;
}

MediaPlayer::~MediaPlayer()
{
	audioURLs.clear();
	audioPlayer->stop();
	delete audioPlayer;

	musicURLs.clear();
	musicPlayer->stop();
	delete musicPlayer;
}

void MediaPlayer::PlayAudio(AudioType audioType)
{
	audioPlayer->pause();
	audioPlayer->setMedia(audioURLs[audioType]);
	audioPlayer->play();
}

void MediaPlayer::PlayMusic()
{
	if (musicPlayer->state() == QMediaPlayer::PausedState)
	{
		musicPlayer->play();
	}
	else
	{
		musicPlayer->setMedia(musicURLs[GetRandomMusicIndex()]);
		musicPlayer->play();
	}
}

void MediaPlayer::PauseMusic()
{
	musicPlayer->pause();
}

void MediaPlayer::SetAudioVolume(int volume)
{
	audioPlayer->setVolume(volume);
}

void MediaPlayer::SetMusicVolume(int volume)
{
	musicPlayer->setVolume(volume);
}

MediaPlayer *MediaPlayer::GetInstance()
{
	if (instance == NULL)
	{
		instance = new MediaPlayer;
	}
	return instance;
}

void MediaPlayer::KeyPressEvent(QKeyEvent *evt)
{
	switch(evt->key())
	{
	case Qt::Key_Space:
		musicPlayer->pause();
		musicPlayer->setMedia(musicURLs[GetRandomMusicIndex()]);
		musicPlayer->play();
		break;
	}
}

void MediaPlayer::StateChanged(QMediaPlayer::State state)
{
	if (state == QMediaPlayer::StoppedState && !musicURLs.empty())
	{
		musicPlayer->pause();
		musicPlayer->setMedia(musicURLs[GetRandomMusicIndex()]);
		musicPlayer->play();
	}
}

int MediaPlayer::GetRandomMusicIndex()
{
	int newMusicIndex = currentMusicIndex;
	while(newMusicIndex == currentMusicIndex)
	{
		newMusicIndex = RandIntToInt(0, musicURLs.size());
	}
	currentMusicIndex = newMusicIndex;
	return newMusicIndex;
}