#include <unistd.h>
#include <mandel_video.h>

int mandel_width = 0;
int mandel_height = 0;

SDL_Surface *surface = NULL;
Uint32 bytesPerPixel = 0;
Uint32 bytesPerLine = 0;
Uint32 pixelMask = 0;
Uint32 NpixelMask = 0;

int mandel_lock() {
	if (SDL_LockSurface(surface)) {
		fprintf(stderr, "Couldn't lock surface!\n");
		return MANDEL_ERR_VIDEO_LOCKSURFACE;
	}
}
void mandel_unlock() {
	SDL_UnlockSurface(surface);
}

int mandel_set_pixel(Uint32 x, Uint32 y, Uint32 pixel) {
	Uint32 *pixelPtr = NULL;
	pixel = pixel & pixelMask;
	pixelPtr = (Uint32*)(surface->pixels + y*bytesPerLine + x*bytesPerPixel);
	(*pixelPtr) &= NpixelMask;
	(*pixelPtr) |= pixel;
	return MANDEL_OK;
}

int mandel_set_pixel_RGB(Uint32 x, Uint32 y, Uint8 r, Uint8 g, Uint8 b) {
	Uint32 pixel = SDL_MapRGB(surface->format, r, g, b);
	return mandel_set_pixel(x, y, pixel);
}


int mandel_update() {
	SDL_UpdateRect(surface, 0, 0, 0, 0);
}

int mandel_video_init() {
	Uint32 whatInit = 0;
	Sint16 y = 0;
	Sint16 x = 0;
	Uint32 pixel = 0;
	char videoDriver[100] = "";
	const SDL_VideoInfo *vInfo = NULL;
	if (SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Error while initializing SDL video! (1)\n");
		return MANDEL_ERR_VIDEO_INITSDL;
	}
	whatInit = SDL_WasInit(0);
	if (SDL_VideoDriverName(videoDriver, 100)) {
		fprintf(stderr, "SDL video initalized! (%x, %s)\n", whatInit, videoDriver);
		vInfo = SDL_GetVideoInfo();
		if (vInfo) {
			if (vInfo->hw_available) {
				fprintf(stderr, "Video: Hardware surfaces available\n");
			} else {
				fprintf(stderr, "Video: Hardware surfaces NOT available\n");
			}
			if (vInfo->wm_available) {
				fprintf(stderr, "Video: Window manager available\n");
			} else {
				fprintf(stderr, "Video: Window manager NOT available\n");
			}
			if (vInfo->blit_hw) {
				fprintf(stderr, "Flag: Accelerated blits HW --> HW\n");
			}
			if (vInfo->blit_hw_CC) {
				fprintf(stderr, "Flag: Accelerated blits with Colorkey\n");
			}
			if (vInfo->blit_hw_A) {
				fprintf(stderr, "Flag: Accelerated blits with Alpha\n");
			}
			if (vInfo->blit_sw) {
				fprintf(stderr, "Flag: Accelerated blits SW --> HW\n");
			}
			if (vInfo->blit_sw_CC) {
				fprintf(stderr, "Flag: Accelerated blits with Colorkey\n");
			}
			if (vInfo->blit_sw_A) {
				fprintf(stderr, "Flag: Accelerated blits with Alpha\n");
			}
			if (vInfo->blit_fill) {
				fprintf(stderr, "Flag: Accelerated color fill\n");
			}
			fprintf(stderr, "Video memory: %d KByte\n", vInfo->video_mem);
			fprintf(stderr, "Bits-per-pixel: %d \n", vInfo->vfmt->BitsPerPixel);
			fprintf(stderr, "Current screen size: %dx%d\n", vInfo->current_w, vInfo->current_h);
		} else {
			fprintf(stderr, "Error getting video format information! (3)\n");
			return MANDEL_ERR_VIDEO_GETINFO;
		}
		if (!(surface = SDL_SetVideoMode(vInfo->current_w, vInfo->current_h, vInfo->vfmt->BitsPerPixel, SDL_HWSURFACE | SDL_FULLSCREEN))) {
//		if (!(surface = SDL_SetVideoMode(640, 480, vInfo->vfmt->BitsPerPixel, SDL_HWSURFACE))) {
//		if (!(surface = SDL_SetVideoMode(320, 240, vInfo->vfmt->BitsPerPixel, SDL_HWSURFACE))) {
			fprintf(stderr, "Error initializing video! (4)\n");
			return MANDEL_ERR_VIDEO_SETMODE;
		}
		if (surface->format->BitsPerPixel % 8) {
			fprintf(stderr, "This application requires byte-bounded pixel format! (5)\n");
			return MANDEL_ERR_VIDEO_PIXELFORMAT;
		}
		bytesPerPixel = surface->format->BytesPerPixel;
		bytesPerLine = bytesPerPixel * surface->w;
		if (bytesPerPixel > sizeof(Uint32)) {
			fprintf(stderr, "Can't handle that many colors! (6)\n");
			return MANDEL_ERR_VIDEO_MANYBYTECOLORS;
		}
		pixelMask = surface->format->Rmask | surface->format->Gmask | surface->format->Bmask | surface->format->Amask;
		NpixelMask = ~pixelMask;
		mandel_width = surface->w;
		mandel_height = surface->h;

/*
		for (y = 0; y < surface->h; y+=1) {
			pixel = SDL_MapRGB(surface->format, y%256, y%256, 0);
			for (x = 0; x < surface->w; x++) {
				mandel_set_pixel(x, y, pixel);
			}
			SDL_UpdateRect(surface, 0, y, surface->w, 1);
			usleep(1000);
//			mandel_update();
		}
		sleep(3);
*/
		return MANDEL_OK;
	} else {
		fprintf(stderr, "Error while initializing SDL video! (2)\n");
		return MANDEL_ERR_VIDEO_DRIVERNAME;
	}
	return MANDEL_ERR_STRANGE;
}

void mandel_video_quit() {
	SDL_Quit();
}


