/*
 * i18n.h - 国际化模块
 */

#ifndef I18N_H
#define I18N_H

typedef enum {
  LANG_ZH, /* 简体中文 (默认) */
  LANG_EN  /* English */
} Language;

/* 初始化 i18n 模块 */
void i18n_init(void);

/* 切换语言 */
void i18n_toggle_language(void);

/* 获取当前语言 */
Language i18n_get_language(void);

/* 根据 key 获取对应语言的字符串 */
const char *i18n_get(const char *key);

#endif /* I18N_H */
