#define SPECWIDTH 368
#define SPECHEIGHT 127

#include "music.h"
#include <numeric>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "filesystem.h"

Music::Music(bool &drawMusic, bool &drawSpectrum, bool &spectrumPrecise) : m_drawMusic(drawMusic), m_drawSpectrum(drawSpectrum), m_spectrumPrecise(spectrumPrecise)
{
	m_volume = 1.0;
	m_drawMusic = drawMusic;
	m_drawSpectrum = drawSpectrum;
	m_spectrumPrecise = spectrumPrecise;
	int device = -1;
	int freq = 44100;
	BASS_Init(device, freq, 0, 0, NULL);
}

Music::~Music()
{
    BASS_Free();
}

void Music::LoadSoundFile(char *file)
{
    m_stream = BASS_StreamCreateFile(FALSE, file, 0, 0, BASS_SAMPLE_LOOP);
}

void Music::Play()
{
    BASS_ChannelPlay(m_stream, FALSE);
}

void Music::Pause()
{
    BASS_ChannelPause(m_stream);
}

void Music::Stop()
{
    BASS_ChannelStop(m_stream);
}

float Music::ReadAmplitude()
{
    float fft[1024];
	BASS_ChannelGetData(m_stream, fft, BASS_DATA_FFT2048);
	int b0 = 0;
	std::vector<int> spectrum;

	for (int x = 0; x < SPECWIDTH; x++)
	{
		float peak = 0;
		int b1 = (int)pow(2, x * 10.0 / (SPECWIDTH - 1));
		if (b1 > 1023) b1 = 1023;
		if (b1 <= b0) b1 = b0 + 1;
		for (; b0<b1; b0++)
		{
			if (peak < fft[1 + b0]) peak = fft[1 + b0];
		}
		int y = (int)(sqrt(peak) * 3 * 255 - 4);
		if (m_spectrumPrecise)
		{
			if (y > 255) y = 255;
			if (y < 0) y = 0;
		}
		spectrum.push_back(y);
	}

	float average = accumulate(spectrum.begin(), spectrum.end(), 0.0) / spectrum.size();
	m_spectrumHistory.push_back(average);
	return average;
}

void Music::SetVolume(float volume)
{
    BASS_ChannelSetAttribute(m_stream, BASS_ATTRIB_VOL, volume);
}

void Music::DrawMusicWindow()
{
	if (m_drawMusic)
	{
		ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);
		ImGui::Begin(".: music :.");
		ImGui::SliderFloat("volume", &m_volume, 0.0, 1.0);
		if (ImGui::Button("Pause"))
		{
			Pause();
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			Stop();
		}
		ImGui::SameLine();
		if (ImGui::Button("Play"))
		{
			Play();
		}
		ImGui::End();
		SetVolume(m_volume);
	}
}

void Music::DrawSpectrumWindow()
{
	if (m_drawSpectrum)
	{
		ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
		ImGui::Begin(".: spectrum :.");
		float spectrum[120];
		std::copy(m_spectrumHistory.end() - 120, m_spectrumHistory.end(), spectrum); // todo: throw out too old data
		ImGui::PlotLines("spectrum", spectrum, IM_ARRAYSIZE(spectrum));
		ImGui::End();
	}
}

void Music::LoadMusic()
{
	char *path = Filesystem::RequestFile("mp3");
	LoadSoundFile(path);
}
