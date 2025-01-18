#ifndef TTS_API_H_
#define TTS_API_H_

//MODE 0 语音文件  1 TTS, 如果0， 取wav_file 播放WAV文件 只能播放16bit的WAV文件， 如果 mode 1 ，用 tts_content 进行TTS播放
//注意:语音文件 只能播放16bit的WAV文件
//sys_volume 音量 5级 1-5 (如果为0，默认为3级)
int dac_play_audio(int mode, int sys_volume, char *wav_file, char *tts_content);

#endif

