#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include "SDLGL.h"
#include "ZipFile.h"

#include "CAppContainer.h"
#include "App.h"
#include "Image.h"
#include "Resource.h"
#include "Render.h"
#include "GLES.h"

#include "Canvas.h"
#include "Graphics.h"
#include "Player.h"
#include "Game.h"
#include "Graphics.h"
#include "Utils.h"
#include "TinyGL.h"
#include "Input.h"

#ifdef __vita__
#include <vitasdk.h>
int _newlib_heap_size_user = 128 * 1024 * 1024;
#endif

void drawView(SDLGL* sdlGL);

#ifdef __vita__
int real_main (unsigned int argc, void* argv);
int main(int argc, char **argv) {
	sceSysmoduleLoadModule(SCE_SYSMODULE_RAZOR_CAPTURE);

	// We need a bigger stack to run Wolfenstein RPG, so we create a new thread with a proper stack size
	SceUID main_thread = sceKernelCreateThread("Wolfenstein RPG", real_main, 0x40, 0x800000, 0, 0, NULL);
	if (main_thread >= 0){
		sceKernelStartThread(main_thread, 0, NULL);
	}
	return sceKernelExitDeleteThread(0);
}
int real_main (unsigned int argc, void* argv) {
#else
int main(int argc, char* args[]) {
#endif
    int		UpTime = 0;

    if (UpTime == 0) {
        UpTime = CAppContainer::getInstance()->getTimeMS();
    }
    
    ZipFile zipFile;
#ifdef __vita__
    zipFile.openZipFile("ux0:data/wolf-rpg/Wolfenstein RPG.ipa");
#else
    zipFile.openZipFile("Wolfenstein RPG.ipa");
#endif

	SDLGL sdlGL;
	sdlGL.Initialize();

    Input input;
    input.init(); // [GEC] Port: set default Binds

    CAppContainer::getInstance()->Construct(&sdlGL, &zipFile);
    sdlGL.updateVideo(); // [GEC]

    while (CAppContainer::getInstance()->app->closeApplet != true) {
        int currentTimeMillis = CAppContainer::getInstance()->getTimeMS();
        if (currentTimeMillis > UpTime) {
            input.handleEvents();
            UpTime = currentTimeMillis + 15;
            drawView(&sdlGL);
            input.consumeEvents();
        }
    }

    printf("APP_QUIT\n");
    CAppContainer::getInstance()->~CAppContainer();
    zipFile.closeZipFile();
    //sdlGL.~SDLGL();
    //input.~Input();
	return 0;
}


static uint32_t lastTimems = 0;

void drawView(SDLGL *sdlGL) {

    int cx, cy;
    int w = sdlGL->vidWidth;
    int h = sdlGL->vidHeight;

    if (lastTimems == 0) {
        lastTimems = CAppContainer::getInstance()->getTimeMS();
    }

    SDL_GetWindowSize(sdlGL->window, &cx, &cy);
    if (w != cx || h != cy) {
        w = cx; h = cy;
    }
#ifdef __vita__
	glViewport(0, 0, (GLsizei)960, (GLsizei)544);
#else
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
#endif
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, Applet::IOS_WIDTH, Applet::IOS_HEIGHT, 0.0, -1.0, 1.0);
    //glRotatef(90.0, 0.0, 0.0, 1.0);
    //glTranslatef(0.0, -320.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    

    uint32_t startTime = CAppContainer::getInstance()->getTimeMS();
    uint32_t passedTime = startTime - lastTimems;
    lastTimems = startTime;

    if (passedTime >= 125) {
        passedTime = 125;
    }
    //printf("passedTime %d\n", passedTime);

    CAppContainer::getInstance()->DoLoop(passedTime);

    SDL_GL_SwapWindow(sdlGL->window);  // Swap the window/pBmp to display the result.
    
}