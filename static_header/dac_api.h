#ifndef TTS_API_H_
#define TTS_API_H_

//MODE 0 �����ļ�  1 TTS, ���0�� ȡwav_file ����WAV�ļ� ֻ�ܲ���16bit��WAV�ļ��� ��� mode 1 ���� tts_content ����TTS����
//ע��:�����ļ� ֻ�ܲ���16bit��WAV�ļ�
//sys_volume ���� 5�� 1-5 (���Ϊ0��Ĭ��Ϊ3��)
int dac_play_audio(int mode, int sys_volume, char *wav_file, char *tts_content);

#endif

