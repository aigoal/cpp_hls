#pragma once
#include <string>

//PCMU g711u
//PCMA g711a
//AAC MPEG4-Generic
class RTCPUnpacket;

struct FrameInfo
{
	unsigned char frameType;
	unsigned int timeStamp;//ʱ���
	unsigned int samplingRate;//������
	int reserver;
	std::string mediaType;//video or audio
	std::string codecType;//��������
	std::basic_string<std::uint8_t> data;//media data
	FrameInfo()
	{
		frameType = 0;
		timeStamp = 0;
		samplingRate = 0;
		mediaType = "video";
		codecType = "H264";
	}
};

typedef unsigned(*RawCallback)(FrameInfo& f, void* arg);

class RtpUnpacket
{
public:
	RtpUnpacket();
	~RtpUnpacket();

	int InputRtpData(unsigned char* data, unsigned short sz, const std::string& type);

	void SetRawCallback(RawCallback cb, void* usr);

	void SetVideoCodecInfo(const std::string& _type, unsigned int _samplingRate = 900000)
	{
		m_videoCodec = _type;
		m_videoSampleRate = _samplingRate;
	}

	void SetAudioCodecInfo(const std::string& _type, unsigned int _samplingRate, unsigned char _soundTrack = 1)
	{
		m_audioCodec = _type;
		m_audioSampleRate = _samplingRate;
		m_soundTrack = _soundTrack;
	}
private:
	
	int ParseAVCRTP(unsigned char* data, unsigned short sz, unsigned int timeStamp, bool mark);

	int ParseHEVCRTP(unsigned char* data, unsigned short sz, unsigned int timeStamp, bool mark);

	int ParseG711RTP(unsigned char* data, unsigned short sz, unsigned int timeStamp, bool mark);

	int ParseAACRTP(unsigned char* data, unsigned short sz, unsigned int timeStamp, bool mark);
private:
	std::string m_videoCodec;//��Ƶ��������
	unsigned int m_videoSampleRate;//��Ƶ������

	std::string m_audioCodec;//��Ƶ��������
	unsigned int m_audioSampleRate;//��Ƶ������
	unsigned char m_soundTrack;//����

	std::basic_string<unsigned char> frameData;
	unsigned char naluType;
private:
	unsigned int videoRtpPackets = 0;//���յ�����Ƶrtp������
	unsigned int videoCycleCount = 1;//sequence number cycles count
	unsigned int audioRtpPackets = 0;//���յ�����Ƶrtp������
	unsigned int audioCycleCount = 1;//sequence number cycles count
	unsigned short lastVideoSeq = 0;
	unsigned short lastAudioSeq = 0;
	friend RTCPUnpacket;
private:
	void* pUsr;
	RawCallback m_cb;
};

