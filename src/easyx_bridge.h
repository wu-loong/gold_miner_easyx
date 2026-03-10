#ifndef EASYX_BRIDGE_H
#define EASYX_BRIDGE_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

int ex_initgraph(int width, int height);
void ex_closegraph(void);
void ex_begin_batch_draw(void);
void ex_end_batch_draw(void);
void ex_flush_batch_draw(void);
void ex_cleardevice(void);
void ex_setbkcolor(COLORREF color);
void ex_setlinecolor(COLORREF color);
void ex_setfillcolor(COLORREF color);
void ex_settextcolor(COLORREF color);
void ex_setbkmode(int mode);
void ex_setlinestyle(int style, int thickness);
void ex_settextstyle(int height, int width, const wchar_t *face_name);
int ex_textwidth(const wchar_t *text);
void ex_outtextxy(int x, int y, const wchar_t *text);
void ex_line(int x1, int y1, int x2, int y2);
void ex_rectangle(int left, int top, int right, int bottom);
void ex_solidrectangle(int left, int top, int right, int bottom);
void ex_solidcircle(int x, int y, int radius);

#ifdef __cplusplus
}
#endif

#endif
