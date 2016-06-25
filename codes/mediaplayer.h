#ifndef _MEDIAPLAYER_H
#define _MEDIAPLAYER_H

#include <QMediaPlayer>
#include <map>
#include <vector>
#include <QKeyEvent>

enum AudioType
{
	PutCell, DeleteCell
};

class MediaPlayer : QObject
{
	Q_OBJECT

public:
	MediaPlayer(QObject *parent = 0);
	~MediaPlayer();

	void PlayAudio(AudioType audioType);
	void PlayMusic();
	void PauseMusic();

	void SetAudioVolume(int volume);
	void SetMusicVolume(int volume);

	static MediaPlayer *GetInstance();

	void KeyPressEvent(QKeyEvent *evt);

private slots:
	void StateChanged(QMediaPlayer::State);

private:
	static MediaPlayer *instance;

	//audio²¥·ÅÆ÷
	QMediaPlayer *audioPlayer;
	std::map<AudioType, QUrl> audioURLs;

	//music²¥·ÅÆ÷
	QMediaPlayer *musicPlayer;
	std::vector<QUrl> musicURLs;

	int currentMusicIndex;

	int GetRandomMusicIndex();
};

#endif //_MEDIAPLAYER_H