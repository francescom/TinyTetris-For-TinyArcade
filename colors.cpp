
#include "common.h"


long rgbColor(int r,int g, int b) {return (long)r<<16 | (long)g<<8 | (long)b;}
int getRed(long srcColorRGB) {return (srcColorRGB & 0xff0000) >> 16;}
int getGreen(long srcColorRGB) {return (srcColorRGB & 0x00ff00) >> 8;}
int getBlue(long srcColorRGB) {return srcColorRGB & 0x0000ff;}


long colorTint(long srcColorRGB,int light) {
	int r,g,b;
 
	if(light>100) light=100;
	if(light<-100) light=-100;

	r=getRed(srcColorRGB);
	g=getGreen(srcColorRGB);
	b=getBlue(srcColorRGB);
 
	if(light>0) {
 
		r=(int)((((long)r*(100L-(long)light))+(long)light*255L)/100L);
		g=(int)((((long)g*(100L-(long)light))+(long)light*255L)/100L);
		b=(int)((((long)b*(100L-(long)light))+(long)light*255L)/100L);

 
	} else {
 
		r=(r*(100+light))/100;
		g=(g*(100+light))/100;
		b=(b*(100+light))/100;
	}
 
	return rgbColor(r,g,b);
}

