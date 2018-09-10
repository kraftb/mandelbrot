#ifndef MANDEL_VIDEO_H
#define MANDEL_VIDEO_H

#include <SDL.h>
#include <mandel_const.h>

extern int mandel_width;
extern int mandel_height;

int mandel_lock();
void mandel_unlock();
int mandel_set_pixel_RGB(Uint32 x, Uint32 y, Uint8 r, Uint8 g, Uint8 b);
int mandel_video_init();
void mandel_video_quit();
int mandel_update();

#endif
