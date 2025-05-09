#ifndef __RENDER_H__
#define __RENDER_H__

#include "Span.h"

class Image;
class gles;
class Graphics;
class Entity;
class TGLVert;

class Render
{
private:

public:
	static constexpr int TEXELS_PER_FILE = 32768;
	static constexpr int RENDER_NORMAL = 0;
	static constexpr int RENDER_BLEND25 = 1;
	static constexpr int RENDER_BLEND50 = 2;
	static constexpr int RENDER_ADD = 3;
	static constexpr int RENDER_ADD75 = 4;
	static constexpr int RENDER_ADD50 = 5;
	static constexpr int RENDER_ADD25 = 6;
	static constexpr int RENDER_SUB = 7;
	static constexpr int RENDER_UNK = 8;
	static constexpr int RENDER_PERF = 9;
	static constexpr int RENDER_NONE = 10;
	static constexpr int RENDER_BLEND75 = 12; // New from IOS
	static constexpr int RENDER_BLENDSPECIALALPHA = 13; // New from IOS
	static constexpr int RENDER_MAX = 14;
	static constexpr int CHAT_ZOOM_SCALE = 32768;
	static constexpr int CHAT_ZOOM_ZOFS = 15;
	static constexpr int RENDER_FLAG_GREYSHIFT = 4;
	static constexpr int RENDER_FLAG_WHITESHIFT = 8;
	static constexpr int RENDER_FLAG_BLUESHIFT = 32;
	static constexpr int RENDER_FLAG_GREENSHIFT = 64;
	static constexpr int RENDER_FLAG_REDSHIFT = 128;
	static constexpr int RENDER_FLAG_BRIGHTREDSHIFT = 256;
	static constexpr int RENDER_FLAG_PULSATE = 512;
	static constexpr int RENDER_FLAG_SCALE_WEAPON = 1024; // [GEC] New
	static constexpr int RENDER_FLAG_MULTYPLYSHIFT = 2048; // [GEC] New
	static constexpr int VIEW_NUDGE = 9;
	static constexpr int SPRITE_Z_NUDGE = 84;
	static constexpr int MAP_SIZE = 32;
	static constexpr int MAPTILE_SIZE = 64;
	static constexpr int TILE_MASK = 63;
	static constexpr int MAPTILE_MIDDLE = 32;
	static constexpr int MAP_MAXWORLDVALUE = 2047;
	static constexpr int SPRITE_SIZE = 64;
	static constexpr int TILE_SIZE = 64;
	static constexpr int FLAT_SIZE = 64;
	static constexpr int MAX_CUSTOM_SPRITES = 48;
	static constexpr int MAX_DROP_SPRITES = 16;
	static constexpr int S_NUMFIELDS = 9;
	static constexpr int S_INDEX_X = 0;
	static constexpr int S_INDEX_Y = 1;
	static constexpr int S_INDEX_Z = 2;
	static constexpr int S_INDEX_RENDERMODE = 3;
	static constexpr int S_INDEX_NODE = 4;
	static constexpr int S_INDEX_NODENEXT = 5;
	static constexpr int S_INDEX_VIEWNEXT = 6;
	static constexpr int S_INDEX_ENT = 7;
	static constexpr int S_INDEX_SCALEFACTOR = 8;
	static constexpr short BASIC_SCALE_FACTOR = 64;
	static constexpr int SINFO_NUMFIELDS = 2;
	static constexpr int MAX_WALL_TEXTURES_MAP = 16;
	static constexpr int MAX_VISIBLE_NODES = 256;
	static constexpr int INVALID_NODE = -1;
	static constexpr int MAPEVENTSIZE = 2;
	static constexpr int MAX_LADDERS_PER_MAP = 10;
	static constexpr int MAX_KEEP_PITCH_LEVEL_TILES = 20;
	static constexpr int BITS_FOR_COORD = 5;
	static constexpr int KEEP_PITCH_LEVEL_COORDS_MASK = 1023;
	static constexpr int RENDER_OFF = 0;
	static constexpr int RENDER_TRANSFORM = 1;
	static constexpr int RENDER_CLIP = 2;
	static constexpr int RENDER_PROJECT = 4;
	static constexpr int RENDER_SPAN = 8;
	static constexpr int RENDER_RASTERIZE_SPAN = 16;
	static constexpr int RENDER_RASTERIZE_DEBUG = 32;
	static constexpr int RENDER_DEFAULT = 31;
	static constexpr int RENDER_DEBUG = 63;
	static constexpr int FADE_FLAG_NONE = 0;
	static constexpr int FADE_FLAG_FADEOUT = 1;
	static constexpr int FADE_FLAG_FADEIN = 2;
	static constexpr int FADE_FLAG_CHANGEMAP = 4;
	static constexpr int FADE_FLAG_SHOWSTATS = 8;
	static constexpr int FADE_FLAG_DENYSKIP = 16;
	static constexpr int FADE_FLAG_EPILOGUE = 32;
	static constexpr int FADE_SPECIAL_FLAG_MASK = -29;
	static constexpr int CHANGEMAP_FADE_TIME = 1000;
	static constexpr int ROCKTIMEDAMAGE = 200;
	static constexpr int ROCKTIMEDODGE = 800;
	static constexpr int ROCKDISTCOMBAT = 6;
	static constexpr int MEDIA_FLAG_REFERENCE = 0x80000000;
	static constexpr int MEDIA_PALETTE_REGISTERED = 0x40000000;
	static constexpr int MEDIA_TEXELS_REGISTERED = 0x40000000;
	static constexpr int MEDIA_MAX_MAPPINGS = 512;
	static constexpr int MEDIA_MAX_IMAGES = 1024;
	static constexpr int MEDIA_MAX_IMAGES_MASK = 1023;
	static constexpr int SKY_MAP_WIDTH = 128;
	static constexpr int SKY_MAP_HEIGHT = 128;
	static constexpr int SKY_MAP_SMASK = 127;
	static constexpr int SKY_MAP_TMASK = 16256;
	static constexpr int SKY_MAP_VIRTUAL_WIDTH = 384;
	static constexpr int SKY_MAP_VIRTUAL_HEIGHT = 384;
	static constexpr int SKY_MAP_HORIZONTAL_REPEATS = 2;
	static constexpr int SKY_MAP_SHIFT = 8;
	static constexpr int SKY_MAP_STEP = 85;
	static constexpr int MAX_BRIGHTNESS_FACTOR = 100;
	static constexpr int HOLD_BRIGHTNESS_TIME = 500;
	static constexpr int FADE_BRIGHTNESS_TIME = 500;
	static constexpr int V_SCROLL_CORRECTION = 5;
	static constexpr int MAX_VSCROLL_VELOCITY = 30;
	static constexpr int MIN_INITIAL_VSCROLL_VELOCITY = 0;
	static constexpr int MAX_INITIAL_VSCROLL_VELOCITY = 90;
	static constexpr int FOG_SHIFT = 8;
	static constexpr int FOG_ONE = 256;
	static constexpr int MAP_HEADER_SIZE = 46;
	static constexpr int MAX_SPLIT_SPRITES = 8;
	static constexpr int SPLIT_SPRITE_BOUNDS = 8;
	static constexpr int ANIM_IDLE_TIME = 8192;
	static constexpr int ANIM_IDLE_MASK = 8191;
	static constexpr int ANIM_IDLE_SWITCH_TIME = 256;
	static constexpr int ANIM_IDLE_DOWN_TIME = 7936;
	static constexpr int ADDITIVE_SHIFT = 488;
	static constexpr int MAX_DIZZY = 30;
	static constexpr int LATENCY_ADJUST = 50;
	static constexpr int portalMaxRadius = 66;
	static constexpr int PORTAL_DNE = 0;
	static constexpr int PORTAL_SPINNING_UP = 1;
	static constexpr int PORTAL_SPINNING = 2;
	static constexpr int PORTAL_SPINNING_DOWN = 3;
	static constexpr int PORTAL_GIVE_UP_CONTROL = 4;
	static constexpr int FEAR_EYE_SIZE = 8;

	int screenWidth;
	int screenHeight;
	int S_X;
	int S_Y;
	int S_Z;
	int S_RENDERMODE;
	int S_NODE;
	int S_NODENEXT;
	int S_VIEWNEXT;
	int S_ENT;
	int S_SCALEFACTOR;
	int SINFO_SORTZ;
	short* mapSprites;
	int* mapSpriteInfo;
	int numMapSprites;
	int numSprites;
	int numNormalSprites;
	int numZSprites;
	short* nodeIdxs;
	int numVisibleNodes;
	int numNodes;
	uint8_t* nodeNormalIdxs;
	short* nodeOffsets;
	short* nodeChildOffset1;
	short* nodeChildOffset2;
	uint8_t* nodeBounds;
	short* nodeSprites;
	uint8_t* nodePolys;
	int numLines;
	uint8_t* lineFlags;
	uint8_t* lineXs;
	uint8_t* lineYs;
	int numNormals;
	short* normals;
	uint8_t* heightMap;
	int* tileEvents;
	int numTileEvents;
	int lastTileEvent;
	int* staticFuncs;
	uint8_t* mapByteCode;
	int mapByteCodeSize;
	uint8_t* mapFlags;
	short mapEntranceAutomap;
	short mapExitAutomap;
	short mapLadders[Render::MAX_LADDERS_PER_MAP];
	short mapKeepPitchLevelTiles[Render::MAX_KEEP_PITCH_LEVEL_TILES];
	int mapNameField;
	int mapNameID;
	int mapSpawnIndex;
	int mapSpawnDir;
	int mapCompileDate;
	uint8_t mapFlagsBitmask;
	int* customSprites;
	int* dropSprites;
	int firstDropSprite;
	int mapMemoryUsage;
	int texelMemoryUsage;
	int paletteMemoryUsage;
	int32_t* sinTable;
	bool skipCull;
	bool skipBSP;
	bool skipLines;
	bool skipFlats;
	bool skipSprites;
	bool skipDecals;
	bool skip2DStretch;
	bool skipViewNudge;
	int renderMode;
	int lineCount;
	int lineRasterCount;
	int nodeCount;
	int leafCount;
	int spriteCount;
	int spriteRasterCount;
	int viewX;
	int viewY;
	int viewZ;
	int viewAngle;
	int viewSin;
	int viewCos;
	int viewRightStepX;
	int viewRightStepY;
	short viewSprites;
	short viewNodes;
	int currentFrameTime;
	int frameTime;
	int spriteTime;
	int lineTime;
	int bspTime;
	int clearColorBuffer;
	int dclTime;
	int preFloorTime;
	int postFloorTime;
	int stringMem;
	int nodeMem;
	int spriteMem;
	int lineMem;
	int eventMem;
	int fadeTime;
	int fadeDuration;
	int fadeFlags;
	int rockViewTime;
	int rockViewDur;
	int rockViewX;
	int rockViewY;
	int rockViewZ;
	bool disableRenderActivate;
	int monsterIdleTime;
	bool chatZoom;
	bool shotsFired;
	SpanType* _spanTrans;
	SpanType* _spanTexture;
	bool fogAffectsSkyMap;
	int postProcessMode;
	bool brightenPostProcess;
	int brightenPostProcessBeginTime;
	int screenVScrollOffset;
	bool useMastermindHack;
	bool useCaldexHack;
	int delayedSpriteBuffer[3];
	uint8_t* skyMapTexels;
	uint16_t** skyMapPalette;
	bool isSkyMap;
	int skyMapX;
	int skyMapY;
	int maxLocalBrightness;
	int brightenPPMaxReachedTime;
	int brightnessInitialBoost;
	int vScrollVelocity;
	int lastScrollChangeTime;
	short* mediaMappings;
	int* mediaPalColors;
	int* mediaTexelSizes;
	uint8_t* mediaDimensions;
	short* mediaBounds;
	uint8_t** mediaTexels;
	int* mediaTexelSizes2;
	uint16_t*** mediaPalettes;
	int* mediaPalettesSizes;
	int fogTableColor;
	int fogTableFrac;
	int fogTableBaseSize; // Pc port only
	uint16_t* fogTableBase;
	uint16_t* fogTableDest;
	int* splitSprites;
	int numSplitSprites;
	int traceLine[4];
	int baseFogMin;
	int baseFogRange;
	int destFogMin;
	int destFogRange;
	int fogLerpStart;
	int fogLerpTime;
	int playerFogMin;
	int playerFogRange;
	int playerFogColor;
	int* temp;
	int imageFrameBounds[3][4];
	int baseDizzy;
	int destDizzy;
	int dizzyTime;
	int portalSpinTime;
	int currentPortalMod;
	bool portalModIncreasing;
	int nextPortalFrame;
	int currentPortalTheta;
	int portalCX;
	int portalCY;
	int portalScale;
	int portalState;
	int previousPortalState;
	bool portalScripted;
	bool portalInView;
	int closestPortalDist;
	int bltTime;
	gles* _gles;
	Image* imgPortal;

	// [GEC] New 
	bool fixWaterAnim1;
	bool fixWaterAnim2;
	bool fixWaterAnim3;
	bool fixWaterAnim4;

	// Constructor
	Render();
	// Destructor
	~Render();

	bool startup();
	void shutdown();
	int getNextEventIndex();
	int findEventIndex(int n);
	void unloadMap();
	inline static int upSamplePixel(int pixel) { return (pixel >> 8 & 0xf800) | (pixel >> 5 & 0x07e0) | (pixel >> 3 & 0x001f); } // rgb888 to rgb565
	inline static int RGB888ToRGB565(int r, int g, int b) { return ((r >> 3 & 0x1f) << 11) | ((g >> 2 & 0x3f) << 5) | (b >> 3 & 0x1f); }; // rgb888 to rgb565
	void RegisterMedia(int n);
	void FinalizeMedia();
	bool beginLoadMap(int mapNameID);
	void draw2DSprite(int tileNum, int frame, int x, int y, int flags, int renderMode, int renderFlags, int scaleFactor);
	void renderSprite(int x, int y, int z, int tileNum, int frame, int flags, int renderMode, int scaleFactor, int renderFlags);
	void renderSprite(int x, int y, int z, int tileNum, int frame, int flags, int renderMode, int scaleFactor, int renderFlags, int palIndex);
	void occludeSpriteLine(int n);
	void drawNodeLines(short n);
	bool cullBoundingBox(int n, int n2, bool b);
	bool cullBoundingBox(int n, int n2, int n3, int n4, bool b);
	void addSprite(short n);
	void addSplitSprite(int n, int n2);
	void addNodeSprites(short n);
	int nodeClassifyPoint(int n, int n2, int n3, int n4);
	void drawNodeGeometry(short n);
	void walkNode(short n);
	int dot(int n, int n2, int n3, int n4);
	int CapsuleToCircleTrace(int* array, int n, int n2, int n3, int n4);
	int CapsuleToLineTrace(int* array, int n, int* array2);
	int traceWorld(int n, int* array, int n2, int* array2, int n3);
	bool renderStreamSpriteGL(TGLVert* array, int n); // Port
	void renderStreamSprite(int n);
	void renderSpriteObject(int n);
	void renderBSP();
	void snapFogLerp();
	void startFogLerp(int n, int n2, int fogLerpTime);
	void buildFogTable();
	void buildFogTable(int n, int n2, int n3);
	void buildFogTables(int fogColor);
	void setupPalette(uint16_t* spanPalette, int renderMode, int renderFlags);
	int(*getImageFrameBounds(int n, int n2, int n3, int n4))[4];
	uint16_t* getPalette(int n, int n2, int n3);
	void setupTexture(int n, int n2, int renderMode, int renderFlags);
	void drawSkyMap(int n2);
	void render(int viewX, int viewY, int viewZ, int viewAngle, int viewPitch, int viewRoll, int viewFov);
	void unlinkSprite(int n);
	void unlinkSprite(int n, int n2, int n3);
	void relinkSprite(int n);
	void relinkSprite(int n, int n2, int n3, int n4);
	short getNodeForPoint(int n, int n2, int n3, int n4);
	int getHeight(int x, int y);
	bool checkTileVisibilty(int n, int n2);
	void postProcessSprites();
	bool isFading();
	int getFadeFlags();
	void startFade(int fadeDuration, int fadeFlags);
	void endFade();
	bool fadeScene(Graphics* graphics);
	int convertToGrayscale(int color);
	void drawRGB(Graphics* graphics);
	void rockView(int rockViewDur, int x, int y, int z);
	bool isSoldat(int n);
	bool isWorker(int n);
	bool isSoldier(int n);
	bool isTrooper(int n);
	bool isOfficer(int n);
	bool isEGuard(int n);
	bool isZombie(int n);
	bool isSkeleton(int n);
	bool isNPC(int n);
	void renderSpriteAnim(int n, int frame, int x, int y, int z, int tileNum, int flags, int renderMode, int scaleFactor, int renderFlags);
	void renderHarbingerAnim(int n, int frame, int x, int y, int z, int flags, int renderMode, int scaleFactor, int renderFlags);
	bool hasPreAttackFrame(int n);
	void handleMonsterIdleSounds(Entity* entity);
	
	void Render3dScene(void);
	void fixTexels(int offset, int i, int mediaID, int* rowHeight); // [GEC] New
};

#endif