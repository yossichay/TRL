
#include "ArduiPi_SSD1306.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#include <mpd/client.h>
#include <mpd/status.h>
#include <mpd/entity.h>
#include <mpd/search.h>
#include <mpd/tag.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


#include <getopt.h>

#define PRG_NAME        "oled"
#define PRG_VERSION     "1.0"

// Instantiate the display
Adafruit_SSD1306 display;

// Config Option
struct s_opts
{
	int oled;
	int verbose;
} ;

// default options values
s_opts opts = {
	OLED_ADAFRUIT_SPI_128x32,	// Default oled
  false										// Not verbose
};

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define TIMULE_FULL_H 64
#define TIMULE_FULL_W  128

#define TIMULE_SS_W	24
#define TIMULE_SS_H	24
#define TIMULE_LG_W	96
#define TIMULE_LG_H	28



 // 'timule-spaceship'
static unsigned char timule_spaceship_bmp[] = 
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x03, 0xf8, 0x07, 0x87, 0xfc, 0x0f, 
	0xcf, 0x1e, 0x1f, 0x9e, 0x0e, 0x3f, 0x3e, 0x0e, 0x1e, 0x7e, 0x0e, 0x0c, 0xfe, 0x1e, 0x01, 0xff, 
	0xfc, 0x03, 0xff, 0xf8, 0x07, 0xff, 0xf0, 0x07, 0xff, 0xe0, 0x07, 0xff, 0xc0, 0x07, 0xff, 0x90, 
	0x07, 0xff, 0x30, 0x07, 0xfe, 0x78, 0x03, 0xfc, 0xf8, 0x01, 0xf9, 0xf0, 0x00, 0xe3, 0xf0, 0x00, 
	0x01, 0xe0, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x0
};
 // 'timule-logo-text'
static unsigned char timule_logo_text_bmp[] = 
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
	0xe0, 0x00, 0x00, 0x01, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x03, 
	0xff, 0xfe, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xff, 0xfe, 0xf8, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xff, 0xfe, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x01, 
	0xe0, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 
	0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x73, 0xcf, 
	0x87, 0xc1, 0xe0, 0x79, 0xe0, 0x7c, 0x00, 0x00, 0x03, 0xc0, 0x73, 0xdf, 0xcf, 0xf1, 0xe0, 0x79, 
	0xe1, 0xff, 0x00, 0x00, 0x03, 0xc0, 0x73, 0xff, 0xff, 0xf9, 0xe0, 0x79, 0xe3, 0xff, 0x80, 0x00, 
	0x03, 0xc0, 0x73, 0xfb, 0xfd, 0xf9, 0xe0, 0x79, 0xe7, 0xc7, 0xc0, 0x00, 0x03, 0xc0, 0x73, 0xe1, 
	0xf8, 0x79, 0xe0, 0x79, 0xe7, 0x81, 0xe0, 0x00, 0x03, 0xc0, 0x73, 0xc0, 0xf0, 0x79, 0xe0, 0x79, 
	0xef, 0x01, 0xe0, 0x00, 0x03, 0xc0, 0x73, 0xc0, 0xf0, 0x79, 0xe0, 0x79, 0xef, 0xff, 0xe0, 0x00, 
	0x03, 0xc0, 0x73, 0xc0, 0xf0, 0x79, 0xe0, 0x79, 0xef, 0xff, 0xe0, 0x00, 0x03, 0xc0, 0x73, 0xc0, 
	0xf0, 0x79, 0xe0, 0x79, 0xef, 0xff, 0xe0, 0x00, 0x03, 0xc0, 0x73, 0xc0, 0xf0, 0x79, 0xe0, 0x79, 
	0xef, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x73, 0xc0, 0xf0, 0x79, 0xe0, 0x79, 0xef, 0x00, 0x00, 0x00, 
	0x03, 0xc0, 0x73, 0xc0, 0xf0, 0x79, 0xe0, 0xf9, 0xe7, 0x81, 0xe1, 0x26, 0x03, 0xc0, 0x73, 0xc0, 
	0xf0, 0x79, 0xff, 0xf9, 0xe7, 0xe7, 0xc1, 0x36, 0x03, 0xc0, 0x73, 0xc0, 0xf0, 0x78, 0xff, 0xf9, 
	0xe3, 0xff, 0xc1, 0x3a, 0x03, 0xc0, 0x73, 0xc0, 0xf0, 0x78, 0xff, 0xf9, 0xe1, 0xff, 0x01, 0x2a, 
	0x03, 0xc0, 0x73, 0xc0, 0xf0, 0x38, 0x3e, 0x79, 0xe0, 0x7c, 0x01, 0x22, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


 // 'timule-logo'
static unsigned char timule_full_logo_bmp[] =
{ 


0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xe0, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xf0, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x07, 0xff, 0xfc, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x3f, 0x83, 0xfc, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x3f, 0x00, 0xfc, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x7f, 0x00, 0x7e, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0xfe, 0x00, 0x7e, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xe7, 0xfe, 0x00, 0x7c, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc7, 0xff, 0x00, 0xfc, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0f, 0xff, 0x80, 0xfc, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x3f, 0xff, 0xff, 0xf0, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xe0, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xc0, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xfe, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xfc, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xf8, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xf3, 0xc0, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xe7, 0xc0, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x8f, 0xc0, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x1f, 0xc0, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x7f, 0xc0, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0xff, 0xc0, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf1, 0xff, 0xc0, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x81, 0xff, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x7f, 0xff, 0xf9, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x7f, 0xff, 0xfb, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x7f, 0xff, 0xfb, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x7f, 0xff, 0xfb, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x78, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x78, 0x00, 0x00, 0x00, 0xe0, 0x0e, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x0c, 0x00, 0x00, 0x00, 
0x00, 0x78, 0x01, 0xf1, 0xe7, 0xfc, 0x7f, 0xe1, 0xf0, 0x07, 0xcf, 0x80, 0xff, 0xc0, 0x00, 0x00, 
0x00, 0x78, 0x01, 0xf1, 0xff, 0xff, 0xff, 0xf1, 0xf0, 0x07, 0xcf, 0x83, 0xff, 0xf0, 0x00, 0x00, 
0x00, 0x78, 0x01, 0xf1, 0xff, 0xff, 0xff, 0xf1, 0xf0, 0x07, 0xcf, 0x87, 0xff, 0xfc, 0x00, 0x00, 
0x00, 0x78, 0x01, 0xf1, 0xfc, 0x3f, 0xc1, 0xf9, 0xf0, 0x07, 0xcf, 0x87, 0xc0, 0xfc, 0x00, 0x00, 
0x00, 0x78, 0x01, 0xf1, 0xf8, 0x1f, 0x81, 0xf9, 0xf0, 0x07, 0xcf, 0x8f, 0x00, 0x7c, 0x00, 0x00, 
0x00, 0x78, 0x01, 0xf1, 0xe0, 0x1f, 0x80, 0xf9, 0xf0, 0x07, 0xcf, 0x8f, 0x00, 0x3e, 0x00, 0x00, 
0x00, 0x78, 0x01, 0xf1, 0xe0, 0x1f, 0x80, 0xf9, 0xf0, 0x07, 0xcf, 0x8f, 0xff, 0xfe, 0x00, 0x00, 
0x00, 0x78, 0x01, 0xf1, 0xe0, 0x1f, 0x80, 0xf9, 0xf0, 0x07, 0xcf, 0x8f, 0xff, 0xfe, 0x00, 0x00, 
0x00, 0x78, 0x01, 0xf1, 0xe0, 0x1f, 0x80, 0xf9, 0xf0, 0x07, 0xcf, 0x8f, 0xff, 0xfe, 0x00, 0x00, 
0x00, 0x78, 0x01, 0xf1, 0xe0, 0x1f, 0x80, 0xf9, 0xf0, 0x07, 0xcf, 0x8f, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x78, 0x01, 0xf1, 0xe0, 0x1f, 0x80, 0xf9, 0xf0, 0x07, 0xcf, 0x8f, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x78, 0x01, 0xf1, 0xe0, 0x1f, 0x80, 0xf9, 0xf0, 0x0f, 0xcf, 0x8f, 0xc0, 0x7e, 0x1f, 0xce, 
0x00, 0x78, 0x01, 0xf1, 0xe0, 0x1f, 0x80, 0xf9, 0xf8, 0x3f, 0xcf, 0x87, 0xe0, 0xfc, 0x08, 0xce, 
0x00, 0x78, 0x01, 0xf1, 0xe0, 0x1f, 0x80, 0xf9, 0xff, 0xff, 0xcf, 0x87, 0xff, 0xfc, 0x08, 0xbe, 
0x00, 0x78, 0x01, 0xf1, 0xe0, 0x1f, 0x80, 0xf8, 0x7f, 0xff, 0xcf, 0x83, 0xff, 0xf0, 0x08, 0xbe, 
0x00, 0x78, 0x01, 0xf1, 0xe0, 0x1f, 0x80, 0xf8, 0x3f, 0xf7, 0xcf, 0x80, 0xff, 0xc0, 0x08, 0xae, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


static void print_tag(const struct mpd_song *song, enum mpd_tag_type type, const char *label)
{
	unsigned i = 0;
	const char *value;

	while ((value = mpd_song_get_tag(song, type, i++)) != NULL)
		printf("%s: %s\n", label, value);
}


/* ======================================================================
Function: usage
Purpose : display usage
Input 	: program name
Output	: -
Comments: 
====================================================================== */
void usage( char * name)
{
	printf("%s\n", name );
	printf("Usage is: %s --oled type [options]\n", name);
	printf("  --<o>led type\nOLED type are:\n");
	for (int i=0; i<OLED_LAST_OLED;i++)
		printf("  %1d %s\n", i, oled_type_str[i]);
	
	printf("Options are:\n");
	printf("  --<v>erbose  : speak more to user\n");
	printf("  --<h>elp\n");
	printf("<?> indicates the equivalent short option.\n");
	printf("Short options are prefixed by \"-\" instead of by \"--\".\n");
	printf("Example :\n");
	printf( "%s -o 1 use a %s OLED\n\n", name, oled_type_str[1]);
	printf( "%s -o 4 -v use a %s OLED being verbose\n", name, oled_type_str[4]);
}


/* ======================================================================
Function: parse_args
Purpose : parse argument passed to the program
Input 	: -
Output	: -
Comments: 
====================================================================== */
void parse_args(int argc, char *argv[])
{
	static struct option longOptions[] =
	{
		{"oled"	  , required_argument,0, 'o'},
		{"verbose", no_argument,	  	0, 'v'},
		{"help"		, no_argument, 			0, 'h'},
		{0, 0, 0, 0}
	};

	int optionIndex = 0;
	int c;

	while (1) 
	{
		/* no default error messages printed. */
		opterr = 0;

    c = getopt_long(argc, argv, "vho:", longOptions, &optionIndex);

		if (c < 0)
			break;

		switch (c) 
		{
			case 'v': opts.verbose = true	;	break;

			case 'o':
				opts.oled = (int) atoi(optarg);
				
				if (opts.oled < 0 || opts.oled >= OLED_LAST_OLED )
				{
						fprintf(stderr, "--oled %d ignored must be 0 to %d.\n", opts.oled, OLED_LAST_OLED-1);
						fprintf(stderr, "--oled set to 0 now\n");
						opts.oled = 0;
				}
			break;

			case 'h':
				usage(argv[0]);
				exit(EXIT_SUCCESS);
			break;
			
			case '?':
			default:
				fprintf(stderr, "Unrecognized option.\n");
				fprintf(stderr, "Run with '--help'.\n");
				exit(EXIT_FAILURE);
		}
	} /* while */

	if (opts.verbose)
	{
		printf("%s v%s\n", PRG_NAME, PRG_VERSION);
		printf("-- OLED params -- \n");
		printf("Oled is    : %s\n", oled_type_str[opts.oled]);
		printf("-- Other Stuff -- \n");
		printf("verbose is : %s\n", opts.verbose? "yes" : "no");
		printf("\n");
	}	
}


/* ======================================================================
Function: main
Purpose : Main entry Point
Input 	: -
Output	: -
Comments: 
====================================================================== */
int main(int argc, char **argv)
{
	int i;
	
	
	// Oled supported display in ArduiPi_SSD1306.h
	// Get OLED type
	parse_args(argc, argv);

	// SPI
	if (display.oled_is_spi_proto(opts.oled))
	{
		// SPI change parameters to fit to your LCD
		if ( !display.init(OLED_SPI_DC,OLED_SPI_RESET,OLED_SPI_CS, opts.oled) )
			exit(EXIT_FAILURE);
	}
	else
	{
		// I2C change parameters to fit to your LCD
		if ( !display.init(OLED_I2C_RESET,opts.oled) )
			exit(EXIT_FAILURE);
	}

	display.begin();
	
  	// init done
  	display.clearDisplay();   // clears the screen and buffer


	for (i = 0 ; i < 25 ; i+=2)
	{
		display.drawBitmap(0, 35,  timule_logo_text_bmp, TIMULE_LG_W, TIMULE_LG_H, 1);
		display.drawBitmap((i*2)+70, 25-i,  timule_spaceship_bmp, TIMULE_SS_W, TIMULE_SS_H, 1);
		display.display();
//			sleep(0.3);
		display.clearDisplay();   // clears the screen and buffer
		}



	struct mpd_connection *conn;

	conn = mpd_connection_new(NULL, 0, 30000);

	if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) 
	{
		fprintf(stderr,"%s\n", mpd_connection_get_error_message(conn));
		mpd_connection_free(conn);
		return -1;
	}

	struct mpd_status * status;
	struct mpd_song *song;
//	const struct mpd_audio_format *audio_format;
	mpd_state currentStatus;
	int totalTime, elapsedTime; 
	const char *value;


	i = 0;
	display.setTextSize(1);
	display.setTextColor(WHITE);

//	mpd_command_list_begin(conn, true);
	while (1)
	{
		mpd_send_status(conn);


		status = mpd_recv_status(conn);
		if (status == NULL) 
		{
			fprintf(stderr,"%d: %s\n", i, mpd_connection_get_error_message(conn));
			mpd_connection_free(conn);
			return -1;
		}
		i++;
	
		currentStatus = mpd_status_get_state(status);
		if (currentStatus == MPD_STATE_PLAY || currentStatus == MPD_STATE_PAUSE)
		{
			totalTime = mpd_status_get_total_time(status);
			elapsedTime = mpd_status_get_elapsed_time(status);

			display.clearDisplay();
			display.setCursor(0,30);
			display.printf("%2d:%02d", elapsedTime / 60, elapsedTime % 60);
			display.setCursor(90,30);
			display.printf("%2d:%02d", (totalTime - elapsedTime) / 60, (totalTime - elapsedTime) % 60);
			if ((currentStatus == MPD_STATE_PAUSE && (i % 5) == 0) || currentStatus == MPD_STATE_PLAY)
				display.drawHorizontalBargraph(14,47,100,16,1, (elapsedTime * 100) / totalTime);

//			mpd_response_next(conn);

			mpd_send_current_song(conn);



			if ((song = mpd_recv_song(conn)) != NULL) 
			{
				
				if ((value = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0)) != NULL)
				{
					display.setCursor(0,0);
					display.printf("%s", value);
				}
				
				if ((value = mpd_song_get_tag(song, MPD_TAG_TITLE, 0)) != NULL)
				{
					display.setCursor(0,10);
					display.printf("%s", value);
				}

				if (opts.verbose)
				{
					print_tag(song, MPD_TAG_ARTIST, "artist");
					print_tag(song, MPD_TAG_TITLE, "title");
				}
			}
			if (!mpd_response_finish(conn)) 
			{
				fprintf(stderr,"%s\n", mpd_connection_get_error_message(conn));
				mpd_connection_free(conn);
				return -1;
            }
			mpd_status_free(status);
			display.display();

		}
		else
		{
			display.clearDisplay();
			display.drawBitmap(0, 0,  timule_full_logo_bmp, TIMULE_FULL_W, TIMULE_FULL_H, 1);
			display.display();
		}
	}

	// Free PI GPIO ports
	display.close();

}




