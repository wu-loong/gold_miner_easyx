#include "easyx_bridge.h"
#include <graphics.h>

extern "C" {

int ex_initgraph(int width, int height) {
  return initgraph(width, height) != NULL;
}

void ex_closegraph(void) { closegraph(); }
void ex_begin_batch_draw(void) { BeginBatchDraw(); }
void ex_end_batch_draw(void) { EndBatchDraw(); }
void ex_flush_batch_draw(void) { FlushBatchDraw(); }
void ex_cleardevice(void) { cleardevice(); }
void ex_setbkcolor(COLORREF color) { setbkcolor(color); }
void ex_setlinecolor(COLORREF color) { setlinecolor(color); }
void ex_setfillcolor(COLORREF color) { setfillcolor(color); }
void ex_settextcolor(COLORREF color) { settextcolor(color); }
void ex_setbkmode(int mode) { setbkmode(mode); }
void ex_setlinestyle(int style, int thickness) { setlinestyle(style, thickness); }
void ex_settextstyle(int height, int width, const wchar_t *face_name) {
  settextstyle(height, width, face_name);
}
int ex_textwidth(const wchar_t *text) { return textwidth(text); }
void ex_outtextxy(int x, int y, const wchar_t *text) { outtextxy(x, y, text); }
void ex_line(int x1, int y1, int x2, int y2) { line(x1, y1, x2, y2); }
void ex_rectangle(int left, int top, int right, int bottom) {
  rectangle(left, top, right, bottom);
}
void ex_solidrectangle(int left, int top, int right, int bottom) {
  solidrectangle(left, top, right, bottom);
}
void ex_solidcircle(int x, int y, int radius) { solidcircle(x, y, radius); }

}
