// Sound.h
// Runs on LM3S1968 or LM3S8962
// Jonathan Valvano
// March 25, 2020

#ifndef SOUND_H
#define SOUND_H
void Sound_Init(void);
void Sound_Play(const unsigned char *pt, unsigned long count);
void Sound_Shoot(void);
void Sound_Killed(void);
void Sound_Explosion(void);

void Sound_Fastinvader1(void);
void Sound_Fastinvader2(void);
void Sound_Fastinvader3(void);
void Sound_Fastinvader4(void);
void Sound_Highpitch(void);

#endif
