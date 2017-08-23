#pragma once
#include <vector>
#include <bass/bass.h>

class Music
{
private:
    HSTREAM m_stream;
	bool &m_drawMusic;
	bool &m_drawSpectrum;
	bool &m_spectrumPrecise;
    std::vector<float> m_spectrumHistory;
	float m_volume;
public:
    Music(bool &drawMusic, bool &drawSpectrum, bool &spectrumPrecise);
    ~Music();
    void LoadSoundFile(char *file);
    void Play();
    void Pause();
    void Stop();
    void SetVolume(float volume);
	void DrawMusicWindow();
	void DrawSpectrumWindow();
	void LoadMusic();
	float ReadAmplitude();
};
