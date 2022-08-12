// ----------------------------------- framebf.h ------------------------------------- 
void framebf_init(); 
void drawPixelARGB32(int x, int y, unsigned int attr); 
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill); 
void drawLineARGB32(int x, int x1, unsigned int attr, int fill);
void drawCircleARGB32(int x0, int y0, int radius, unsigned int attr, int fill);